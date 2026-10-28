#include <iostream>
#include <cstring>

// ---------- 模拟的 4KB 分配函数：从一个静态数组里一页一页地给 ----------
//const：侧重于“只读”（Run-time 或 Compile-time 均可）。
//被修饰的变量在初始化后不能被修改，但不一定能在编译时知道它的值。
//constexpr：侧重于“编译期常量”。
//它比 const 更严格：constexpr 必定是 const 的，但 const 变量不一定是 constexpr。
constexpr size_t kPageSize  = 4096;
constexpr size_t kPageCount = 16;

//static: 限制作用域：该函数的作用域被限制在当前的源文件
//unsigned char vs char: 同样的字节内容，转成整数后可能产生不同结果。
//alignas(16) 表示：要求 g_pool 的起始地址满足 16 字节对齐，也就是地址是 16 的整数倍。
//unsigned char 本身只要求 1 字节对齐，
//因此普通的 unsigned char 数组不保证起始地址满足更严格的对齐要求。
//而内存池后续可能用于存放需要更严格对齐的对象，alignas(16) 就为这块存储提供了更强的对齐保证。
alignas(16) static unsigned char g_pool[kPageSize * kPageCount];
static size_t g_used_pages = 0;

//尽管g_pool用的是unsigned char，但是返回的是void*，这样可以避免类型转换问题
//内部：用 unsigned char* 计算字节偏移
//对外：隐式转换成 void*，不规定用途
void* alloc_page() {
    if (g_used_pages == kPageCount) return nullptr;   // 页用完了
	unsigned char* page = g_pool + kPageSize * g_used_pages++;
    return page;
}

// ---------- 分配器 ----------
class Allocator {
public:
	//构造函数把传入的函数地址保存起来
	using PageFn = void* (*)(); //给一类函数起别名
	PageFn page_fn_;
    explicit Allocator(PageFn page_fn) : page_fn_(page_fn) {}

	struct alignas(16) Node {
        size_t size;      // 用户可用大小（不含头部）
        Node* next;      // 只在空闲时使用
    };
    static constexpr size_t PAGE  = kPageSize;
    static constexpr size_t ALIGN = 16;
	//把 n 向上对齐到 ALIGN 的整数倍
    static size_t align_up(size_t n) { return (n + ALIGN - 1) & ~(ALIGN - 1); }
	Node* list_head = nullptr; //free list

	//这两行是为了禁止拷贝：如果这个 Allocator 管理一块内存以及它的分配状态，就不能随便复制，
	//否则可能出现“两份独立的管理状态，却指向同一块内存”的问题。这是资源管理类常见的设计。
	Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;

	void* allocate(size_t size) {
		if (size == 0) return nullptr;
        size_t n = align_up(size);
        if (n > PAGE - sizeof(Node)) return nullptr;   // 不支持超过一页

		// 1. 在空闲链表里找第一个够大的块
		// 这里用二级指针，因为要修改 free_list_ 本身。
		// 如果不用二级指针，那么就要用两个一级指针，一个指向当前块，一个指向prev块，这样比较麻烦。
        Node** pp = &list_head;
        while (*pp && (*pp)->size < n) pp = &(*pp)->next;

		//2. 处理节点
		Node* b = *pp;
        if (b) {
            *pp = b->next;                              // 从链表摘下
        } else {
            b = static_cast<Node*>(page_fn_());        // 没有就要新的一页
            if (!b) return nullptr;
            b->size = PAGE - sizeof(Node);
        }

		// 3. 剩余空间够大就切开，剩下的放回链表
        if (b->size >= n + sizeof(Node) + ALIGN) {
            Node* rest = reinterpret_cast<Node*>(
                reinterpret_cast<char*>(b + 1) + n);
            rest->size = b->size - n - sizeof(Node);
            rest->next = list_head;
            list_head = rest;
            b->size = n;
        }

        return b + 1;                                   // 头部后面是用户内存
	}

	void deallocate(void* p) {
		if (!p) return;
        Node* b = static_cast<Node*>(p) - 1;          // 退回到头部
        b->next = list_head;
        list_head = b;
	}

	// 调试用：打印空闲链表
    void print_free_list() const {
        std::printf("free list:\n");
        for (Node* b = list_head; b; b = b->next)
            std::printf("\t [%p size=%zu]\n", static_cast<void*>(b), b->size);
        std::printf("\n");
    }

};



int main(){
	std::cout<<"===Allocator Test begin==="<<std::endl;

	Allocator alloc(alloc_page);

	// 分配一个 int 数组并使用
    int* arr = static_cast<int*>(alloc.allocate(10 * sizeof(int)));
    for (int i = 0; i < 10; ++i) arr[i] = i * i;
    std::printf("arr[9] = %d\n", arr[9]);

    // 分配一个字符串
    char* s = static_cast<char*>(alloc.allocate(32));
    std::strcpy(s, "hello allocator");
    std::printf("s = %s\n", s);

    // 分配一个结构体（用 placement new 构造对象）
    struct Point { float x, y; };
    void* mem = alloc.allocate(sizeof(Point));
    Point* pt = new (mem) Point{1.0f, 2.0f};
    std::printf("pt = (%.1f, %.1f)\n", pt->x, pt->y);

    alloc.print_free_list();

    // 释放后再分配同样大小，应该复用刚释放的那块
    alloc.deallocate(arr);
    int* arr2 = static_cast<int*>(alloc.allocate(10 * sizeof(int)));
    std::printf("reuse: %s\n", arr2 == arr ? "yes" : "no");

    // 检查对齐
    std::printf("aligned: %s\n",
        reinterpret_cast<size_t>(s) % 16 == 0 ? "yes" : "no");

    pt->~Point();
    alloc.deallocate(pt);
    alloc.deallocate(s);
    alloc.deallocate(arr2);
    alloc.print_free_list();
    std::printf("pages used: %zu\n", g_used_pages);
	
	std::cout<<"===Allocator Test end==="<<std::endl;
	return 0;
}

/* 运行结果
===Allocator Test begin===
arr[9] = 81
s = hello allocator
pt = (1.0, 2.0)
free list:
         [00007ff60099d0c0 size=3936]

reuse: yes
aligned: yes
free list:
         [00007ff60099d030 size=48]
         [00007ff60099d070 size=32]
         [00007ff60099d0a0 size=16]
         [00007ff60099d0c0 size=3936]

pages used: 1
===Allocator Test end===
*/

/* 结果说明
reuse: yes：释放 arr 后再申请同样大小，拿回的是同一个地址，说明空闲链表在正常工作。

pages used: 1：三次分配都是从同一页里切出来的，没有浪费页。

最后的空闲链表：48、32、16 这几块和后面 3936 那块，在内存里其实是挨着的，加起来是完整的一页。
但因为没有做合并，它们还是分散的几小块。这时候如果申请 4000 字节，就会失败或者再要一页新的。

所以下一步可以练合并：deallocate 时按地址顺序插入链表，再检查前后的块是否紧挨着（(char*)(b + 1) + b->size == (char*)next）。
紧挨着就合并成一块，合并后最后的空闲链表应该只剩一个 4080 字节的块。
*/

