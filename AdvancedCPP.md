# Data Race 与 Race Condition 的区别
Data Race（数据竞争）: 当多个线程并发访问同一块内存，至少一个线程在写入，并且这些访问没有适当的同步，就可能构成 data race。  
在 C++ 中，data race 会导致 Undefined Behavior（未定义行为）。  

解决办法： 使用 std::atomic<int> 或 std::mutex 保护访问。  

Race Condition（竞态条件）: Race condition 指的是：程序的正确性依赖于多个操作的执行顺序或时机。  
例如，两个线程共享一个变量，但通过 mutex 保护各自的访问： 如果业务逻辑被拆成两个独立的临界区：  
```
假设两个线程都尝试取款 80：
线程 A 检查余额，发现有 100。
线程 B 也检查余额，发现有 100。
线程 A 扣除 80，余额变成 20。
线程 B 再扣除 80，余额变成 -60。
```
所有访问都使用了 mutex，因此这段代码没有上述 data race；但检查与扣款不是一个原子操作，仍然存在逻辑上的 race condition。  
解决方法是把检查和扣款放在同一个临界区内。  

临界区（Critical Section） 是指程序中访问共享资源、需要进行同步保护的一段代码，目的是避免多个线程同时操作资源而产生错误。

# 原子操作与互斥锁的区别
原子操作（Atomic Operation）保证单个操作不可被其他线程观察到中间状态；  
互斥锁（Mutex）允许你保护一段代码，让同一时刻只有一个线程进入受保护区域。  
一个很重要的区别：单次操作 vs. 多步操作  

原子操作：std::atomic  
```c++
#include <atomic>
std::atomic<int> counter{0};
void worker() {
    counter.fetch_add(1);
}
```


# Deadlock 的常见原因
Deadlock（死锁） 是指两个或多个线程互相等待对方持有的资源，导致所有相关线程都无法继续执行。  
假设有两把锁 mutexA 和 mutexB。  
```c++
std::mutex mutexA, mutexB;  
```
```c++
// Thread 1
void task1() {
    std::lock_guard<std::mutex> lockA(mutexA);
    std::lock_guard<std::mutex> lockB(mutexB);
}

// Thread 2
void task2() {
    std::lock_guard<std::mutex> lockB(mutexB);
    std::lock_guard<std::mutex> lockA(mutexA);
}
```
注意：并不是每次运行都会死锁。只有当两个线程以这种特定时序分别持有一把锁并等待另一把锁时，才会形成上述死锁。  



# Producer-Consumer 与 Ring Buffer 的基本设计
可以把它理解为：一个线程产生 GPU profiling events，另一个线程消费并处理这些 events，中间通过 Ring Buffer 传递数据。  

```c++
#include <array>
#include <condition_variable>
#include <cstddef>
#include <mutex>

class RingBuffer {
public:
    static constexpr std::size_t Capacity = 4;

    // Returns false if closed.
    bool push(int value) {
        std::unique_lock<std::mutex> lock(mutex_);

        notFull_.wait(lock, [&] {
            return count_ < Capacity || closed_;
        });

        if (closed_) return false;

        buffer_[tail_] = value;
        tail_ = (tail_ + 1) % Capacity;
        ++count_;

        notEmpty_.notify_one();
        return true;
    }

    // Returns false if closed and drained.
    bool pop(int& value) {
        std::unique_lock<std::mutex> lock(mutex_);

        notEmpty_.wait(lock, [&] {
            return count_ > 0 || closed_;
        });

        if (count_ == 0) return false;

        value = buffer_[head_];
        head_ = (head_ + 1) % Capacity;
        --count_;

        notFull_.notify_one();
        return true;
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        notEmpty_.notify_all();
        notFull_.notify_all();
    }

private:
    std::array<int, Capacity> buffer_{};
    std::size_t head_ = 0;
    std::size_t tail_ = 0;
    std::size_t count_ = 0;
    bool closed_ = false;

    std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
};

```
代码里最重要的 5 个点
| 成员 | 作用 |
|---|---|
| `head_` | 下一个读取位置 |
| `tail_` | 下一个写入位置 |
| `count_` | 当前元素数量，用于区分满与空 |
| `mutex_` | 保护 `buffer` 和所有状态变量 |
| `condition_variable` | 缓冲区满或空时，让线程等待而不是忙等 |



# 原子操作与内存模型  
load()：读取  
```c++
std::atomic<int> x{10};
int value = x.load();
// value == 10
```

store()：写入
```c++
std::atomic<int> x{10};
x.store(20);
// x 被写入 20
```

fetch_add()：加法，并返回旧值  
```c++
std::atomic<int> x{10};
int old = x.fetch_add(3);
// old == 10
// x == 13（没有其他线程修改时）
```

典型用途是计数或分配编号：
```c++
std::atomic<int> nextId{0};
int allocateId() {
    return nextId.fetch_add(1);
}
```

exchange()：无条件替换，并返回旧值
```c++
std::atomic<int> x{10};
int old = x.exchange(20);
// old == 10
// x == 20（没有其他线程修改时）
```  

compare_exchange_*()：满足条件才替换
```c++
bool success = x.compare_exchange_strong(expected, desired);

对于这里的整数例子，可以按下面的规则理解：
当前 x == expected：
    x ← desired
    返回 true
    expected 不变

当前 x != expected：
    x 不变
    expected ← 本次比较观察到的 x
    返回 false
```  

成功例子  
```c++
std::atomic<int> x{10};
int expected = 10;
bool ok = x.compare_exchange_strong(expected, 20);
// ok == true
// x == 20
// expected == 10

```
意思是：“只有当 x 仍然是我预期的 10 时，才把它改成 20。”检查和替换是同一次原子操作。  
失败例子
```c++
std::atomic<int> x{15};
int expected = 10;
bool ok = x.compare_exchange_strong(expected, 20);
// ok == false
// x == 15
// expected == 15
```
因为实际值是 15，不匹配预期的 10，所以不修改 x，并把观察到的实际值写回 expected。  
上述结果都假定没有其他线程继续修改 x；失败后得到的 expected 并不保证一直等于 x，它仍只是该次操作观察到的值  


# Blocking Queue  
Blocking queue（阻塞队列）是一个线程安全的队列：取数据时，如果队列为空，调用线程就等待，直到有数据可取；如果是有界队列，放数据时队列已满，调用线程也会等待，直到有空位。  
这里的 阻塞 指调用线程暂时等待，而不是不断循环检查队列，也不是把整个程序停住。   
它解决什么问题？  

消费者比生产者快：队列为空，消费者等待。  
生产者比消费者快：对于有界队列，队列满后生产者等待。  
多个生产者、多个消费者可以通过队列安全地交接任务。  

C++ 的基本实现：  
```c++
std::queue          保存数据
std::mutex          保护队列状态
condition_variable  没有数据时等待，有数据时唤醒
```

# 一个函数返回动态数据，为什么通常优先返回 std::vector<T>？
```c++
std::vector<int> makeData() {
    std::vector<int> result{10, 20, 30};
    return result;
}
```
这里可以通过 NRVO 或移动构造高效地返回，不需要为了避免深拷贝而返回局部引用。  
按值返回，通常不会深拷贝整个数组  
不要为了“优化”写 return std::move(result)  
如果在高频调用中需要复用已有缓冲区，输出参数也可能更合适： 
```c++
void makeData(std::vector<int>& output) {
    output.clear();
    // 填充 output，可能复用原有 capacity
}
```

# 拥有资源的类，应该允许复制吗？如果允许，复制的语义是什么？
拥有资源的类是否可复制，取决于所有权模型。独占资源通常禁止复制、允许移动；值类型的复制应得到独立且等价的值；共享句柄的复制则共享资源并管理其生命周期。不能只浅拷贝裸指针或句柄，却让多个对象各自释放同一资源  

# 错误路径或者异常发生时，资源由谁释放？
由资源当前的所有者负责释放。C++ 通常用 RAII 实现：把资源交给一个对象管理，由它的析构函数释放资源。  
正常返回、错误路径提前 return，以及异常传播发生栈展开时，已经构造完成的局部对象都会被析构。  
所以不是“发生异常后，catch 自动释放所有资源”，而是“离开作用域时，资源所有者的析构函数负责清理”。  



# 这些接口和它们的语义
## void inspect(const Resource& resource);           // 只读，不转移所有权
## void modify(Resource& resource);                  // 修改，不转移所有权
## void consume(std::unique_ptr<Resource> resource); // 接收所有权
```c++
void consume(std::unique_ptr<Resource> resource) {
    if (!resource) {
        return;
    }

    resource->use();
}

auto ptr = std::make_unique<Resource>();

consume(std::move(ptr));

// ptr == nullptr，已不再拥有 Resource
```
```c++
consume(ptr);            // 编译错误：需要复制 unique_ptr
consume(std::move(ptr)); // 正确：移动构造函数参数
```
## std::unique_ptr<Resource> create();               // 返回所有权
```c++
std::unique_ptr<Resource> create() {
    return std::make_unique<Resource>();
}

void example() {
    auto resource = create();

    resource->use();

} // resource 析构，自动释放 Resource
```


# Move Semantics
这三个问题要答准确：
1. std::move 自己执行了什么？  
std::move 本身是转换，不直接搬运数据；实际行为由后续选择的构造函数或赋值操作决定。  

2. 被 move 的对象还能不能使用？  
不要对一般对象的 moved-from 状态作未经接口保证的假设；对 unique_ptr，移动后原指针为空。  

3. 为什么 move constructor 经常写 noexcept？  
vector 重新安置元素时，移动是否可能抛异常会影响它选择 move 还是 copy；若没有复制操作，也不能简单说“一定回退到 copy”  

# 分清 reserve、resize、size、capacity
| 接口 | 含义 | 是否改变元素数量 |
|---|---|---|
| `size()` | 查询当前元素数量 | 不改变 |
| `capacity()` | 查询不重新分配内存时能容纳的元素数量 | 不改变 |
| `reserve(n)` | 保证容量至少为 `n`，必要时重新分配 | 不改变 |
| `resize(n)` | 把元素数量调整为 `n`，增加或删除元素 | 改变为 `n` |

# vector 扩容和 unordered_map rehash 的失效区别
vector 扩容并重新分配内存时，迭代器、指针和引用全部失效；  
unordered_map rehash 时，迭代器失效，但指向现有元素的指针和引用仍然有效。cppreference+1  



# 用 predicate 写等待，并解释 shutdown。  
可以把 blocking queue 想象成一个“等餐取餐”的窗口：
- 队列里的数据 = 做好的餐。
- 消费者线程 = 等餐的人。
- predicate = 判断“现在还需不需要继续等”的规则。
- shutdown = 窗口宣布关门。

predicate 中文通常叫“谓词”，通俗来说就是一个返回 true 或 false 的 判断条件。  
```c++
[this] {
    return !queue_.empty();
}
```
这段代码只回答一个问题：
队列里有没有数据？
- 有数据：返回 true。
- 没数据：返回 false。

放在 wait() 里：
```c++
cv.wait(lock, [this] {
    return !queue_.empty();
});
```
意思就是：没数据就等；有数据就不等了，继续执行。
```
shutdown_ == false：正常营业
shutdown_ == true ：已经关门
```

两者怎么结合？  
```c++
cv.wait(lock, [this] {
    return shutdown_ || !queue_.empty();
});
```
通俗翻译：等到“有餐可取”或者“窗口关门”，就停止等待。  
停止等待之后，再决定做什么：    
predicate 是“什么时候不用等”的判断规则；shutdown 是“已经停止营业”的状态。   

