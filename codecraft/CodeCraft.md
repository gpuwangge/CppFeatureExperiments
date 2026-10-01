# Smart Pointer
RAII 是 Resource Acquisition Is Initialization 的缩写  
中文通常翻译为： 资源获取即初始化  
把资源的生命周期绑定到对象的生命周期上。   
```c++
#include <memory>
void GoldCraft() override{
    std::unique_ptr<int[]> arr;
    arr = std::make_unique<int[]>(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    std::cout<<arr[0]<<","<<arr[1]<<","<<arr[2]<<std::endl;
}
```
小知识：  
如果一个函数负责创建/分配动态数组，并且希望数组生命周期由调用者继续管理，  
那么用 std::unique_ptr 传入通常比裸指针更安全、更清晰。  
```c++
#include <memory>
void createArray(std::unique_ptr<int[]>& arr){
    arr = std::make_unique<int[]>(10);

    arr[0] = 100;
    arr[1] = 200;
}
int main(){
    std::unique_ptr<int[]> arr;
    createArray(arr);

    // arr 现在拥有这块数组
    // 可以继续使用
    // arr[0] == 100

} // main结束，arr自动释放数组
```
unique_pointer作为返回值：因为 unique_ptr 不能拷贝，只能移动，函数返回时编译器会自动进行 move/返回值优化。 
```c++
#include <memory>
std::unique_ptr<int[]> createArray(){
    auto arr = std::make_unique<int[]>(10);

    arr[0] = 100;
    arr[1] = 200;

    return arr;
}
int main(){
    auto arr = createArray();

    // 使用
    std::cout << arr[0];

} // arr 析构，数组自动释放
```

# Multithread
```c++
#include <thread>
#include <mutex>
void GoldCraft() override{
    int counter = 0;        // 共享变量
    std::mutex m;           // 锁, 有lock和unlock两种方法

    auto add = [&]() {
        m.lock();
        for (int i = 0; i < 1000000; ++i) counter++;
        m.unlock();
    };
    std::thread t1(add);
    std::thread t2(add);

    t1.join(); //让当前线程阻塞，直到目标线程执行结束。主线程（main()）停在这里不往下执行。等 t1 完成。
    t2.join(); //再等 t2 完成,两个线程都结束后，主线程才继续执行

    std::cout << counter << std::endl;    // 输出 2000000
}
```

lambda表达式，调用成员函数add
[&] —— 捕获外部变量,也就是counter和m

以下两种用法等价
```c++
std::lock_guard<std::mutex> lock(m);            // 自动加锁、自动解锁, lock_guard 是管理这个锁的 RAII 对象
for (int i = 0; i < 1000000; ++i) counter++;    // 临界区：只能一个线程进入
```
```c++
m.lock();
for (int i = 0; i < 1000000; ++i) counter++;
m.unlock();
```

如果add是成员函数，不能这样写
```c++
std::thread t1(add);
std::thread t2(add);
```
正确写法：
```c++
std::thread t1(&MultiThreadTest::add, this);
std::thread t2(&MultiThreadTest::add, this);
```

# Hashtable
平均情况下查找/插入/删除都是 O(1)。 

"="和"insert"都可以赋值，区别如下： 
| 操作                      | key 不存在 | key 已存在 |
| ----------------------- | ------- | ------- |
| `m["Bob"] = 25`         | 创建      | **覆盖**  |
| `m.insert({"Bob", 25})` | 创建      | **不覆盖** |

map和unordered_map的区别：前者自动保持key有序，代价如下：  
|        | `map`      | `unordered_map` |
| ------ | ---------- | --------------- |
| 是否排序   | ✅ 按 key 排序 | ❌ 不保证顺序         |
| 底层     | 通常红黑树      | Hash Table      |
| 查找     | O(log N)   | 平均 O(1)         |
| 插入     | O(log N)   | 平均 O(1)         |
| 删除     | O(log N)   | 平均 O(1)         |
| key 重复 | ❌          | ❌               |


```c++
#include <unordered_map>
void GoldCraft() override{
    std::unordered_map<std::string, int> age;
    age["Alice"] = 30;
    age["David"] = 25;
    age["Charlie"] = 35;
    
    age.insert(std::make_pair<std::string, int>("Bob", 99));

    std::cout << "Alice's age: " << age["Alice"] << std::endl; // 输出 30
    if(age.find("Bob") != age.end()) 
        std::cout << "Bob's age: " << age["Bob"] << std::endl; // 输出 25

    age.erase("Charlie");
    if(age.find("Charlie") == age.end()) 
        std::cout << "Charlie not found" << std::endl; // 输出 "Charlie not found"

    for(auto it = age.begin(); it != age.end(); it++)
        std::cout << it->first << "=" << it->second << " ";
    std::cout << std::endl;
}
```

# Sort
```c++
std::sort(v.begin(), v.end());
```
时间复杂度是：O(N log N)  
为什么？  
C++ 的 std::sort 通常基于 Introsort（内省排序），结合了：
- Quick Sort：平均 O(N log N)
- Heap Sort：保证最坏 O(N log N)
- Insertion Sort：小范围数据优化

所以标准要求 std::sort 的比较次数最坏情况下也是 O(N log N)。  
空间复杂度通常：O(log N)  
主要来自递归/排序内部的栈空间；具体实现可能有所不同。  

```c++
#include <algorithm>
void GoldCraft() override{
    auto compare = [](int a, int b) { return a > b;};
    std::sort(arr.begin(), arr.end(), compare);
    std::cout << "Sorted array: ";
    for(auto& num : arr){
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
```


