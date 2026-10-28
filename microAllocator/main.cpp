#include <iostream>
#include <cstring>
/* Memory Allocator问题描述
给一个allocate 4kb的malloc，实现general malloc
考察点进货一整个切糕，但让顾客觉得按小段出售

用辅助list做记账本，记账本的目的是记录空段,list的元素就是空段地址。
这样的好处：用户可以去快速查memory，看哪里有空着的。
（这道题所有的所谓内存分割合并什么的都是指虚拟内存）
（知识点：malloc得到的内存是一定是要连续的；这里的地址说的都是虚拟地址；实际物理地址可能是不连续的）
（硬件最后是靠mmu和page table把虚拟内存和物理内存连起来的）

实现细节：
不要用std::list, 因为它本身就会用malloc manage 内存，用了就失去了实现malloc的初衷。
应该使用侵入式链表（intrusive list）：节点就是数据本身，不需要额外的内存。
*/

/* 完整流程
1. 拿到新页：在开头写一个 16 字节的头部，记下 size = 4080。这时整块都是空闲的，放进 free list。
2. allocator：从 free list 里找一个够大的块，把它从链表里摘下来，返回 node + 1 给用户。
3. 用户使用中：这个块不在 free list 里，但头部还在内存里，size 还保存着，因为 deallocator(free) 时要用。
4. deallocator：用 p - 1 找回头部，再把它挂回 free list。
*/

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
        size_t size;      // 用户可用大小（不含头部）。 8 字节
        Node* next;      // 只在空闲时使用。8 字节（64 位系统上指针是 8 字节）
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
        size_t aligned_size = align_up(size);

		// 不支持超过一页
        if (aligned_size > PAGE - sizeof(Node)) return nullptr;   //sizeof(Node)是16字节

		// 这里用二级指针，因为要修改 list_head本身。
		// 如果不用二级指针，那么就要用两个一级指针，一个指向当前块，一个指向prev块，并且修改list_head比较麻烦。
		// 参考：https://github.com/gpuwangge/CppFeatureExperiments/blob/main/FundamentalCPP.md
		// 当要移除某个node的时候，不管node是不是头节点，都可以用*pp = node->next;移除
        Node** pp = &list_head;

		// 1. 在free链表里找第一个够大的块
        while (*pp && (*pp)->size < aligned_size) pp = &(*pp)->next;

		//2. 处理节点：如果free链表里有足够的块(pp不是空)，就摘下放到node；如果链表没有合适，就新建一个页。不管哪种，node里面就是用户要的空间的tag；node+1就是用户要的空间
		Node* node = *pp;
        if (node) {
            *pp = node->next;                              // 从链表摘下
        } else {
            node = static_cast<Node*>(page_fn_());        // 没有就要新的一页
            if (!node) return nullptr;
            node->size = PAGE - sizeof(Node); 			  //sizeof(Node)是16字节
        }

		// 3. 剩余空间够大就切开，剩下的放回链表
        if (node->size >= aligned_size + sizeof(Node) + ALIGN) {
            Node* rest = reinterpret_cast<Node*>(
                reinterpret_cast<char*>(node + 1) + aligned_size);
            rest->size = node->size - aligned_size - sizeof(Node);

			//以下两行是更新node到free链表的标准操作，总是把新的node放在链表的头部而不是最后
			//主要原因是插到头部最简单也最快，而且对这个分配器来说，链表顺序不影响正确性。
			//还有一个附带的好处：缓存。插到头部之后，链表就是后进先出（LIFO），刚 free 的块会被下一次 malloc 最先拿到。
			//刚用过的内存很可能还在 CPU cache 里，再用它会更快。很多真实分配器（比如 tcmalloc 的 thread cache）就是故意这么做的。
            //但是，想做合并时，插到头部就不够了。要判断相邻块，最方便的做法是让链表按地址排序，deallocate时插到正确的位置
			//如果要使用排序方案的话，在这里放回链表的时候就要做有序插入
			rest->next = list_head;
            list_head = rest;

            node->size = aligned_size;
        }

        return node + 1;                                   // 头部后面是用户内存
	}

	void deallocate(void* p) {
		if (!p) return;
        Node* b = static_cast<Node*>(p) - 1;          // 退回到头部

		//如上所说的，下面两行是更新node到free list头部的标准操作
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

