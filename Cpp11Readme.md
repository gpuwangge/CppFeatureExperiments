# 右值引用(rvalue reference)
解释右值引用之前，首先要解释什么是左值、右值和引用  
左值(Lvalue)：有名字的对象。因为有名字，就可以在内存中找到位置，可以被访问。比如，变量x就是一个左值。  
右值(Rvalue)：没有名字的对象。即是一个临时对象。比如，数字42或表达式x+y都是右值。  
引用(Reference)：给一个已经有名字的对象(左值)创建一个别名(另一个左值)。比如&a = b  
如上可以很容易推导出右值引用的定义  
右值引用(Rvalue Reference): 给一个没有名字的对象(右值)赋予一个新的或已经存在的别名(左值)。比如&&a = 42，a是一个左值，&&a是一个右值引用。  

右值引用有什么用处  
值得一提的是，&&a = 42这种把右值直接绑定到右值引用的行为没有意义，仅仅是用于学习和理解什么是右值引用。  
在实际运用的时候，往往是将一个左值转换成右值后，在绑定到右值引用。  
因为如果把一个左值直接赋值给另一个左值，会出现拷贝。  
通过右值引用，可达到把一个左值赋值给另一个左值，又不引起拷贝。  

关于拷贝的一些实验  
观察程序在什么时候会出现拷贝。  
对于简单的数据类型，比如int，float，进行赋值的时候并不会出现拷贝，比如  
```c++
int a = 42;
```
这段代码把一个临时对象(右值)赋予左值，并没有出现拷贝。  
但是当=号右边也是左值的时候，会出现拷贝：  
```c++
int a = 42;
int b = a;
```
a和b都是左值(都有名字)，当处理到int b = a这条语句的时候，涉及到把名字为a内存里面的数据拷贝到名字为b的内存里。  
（这时候打印a和b的地址，观察结果: a和b的地址不同）  
如果不想发生拷贝，应该使用如下方式  
```c++
int a = 42;
int &&b = std::move(a);
```
std::move把a强行转化成右值，然后绑定到b上  
（这时候打印a和b的地址，观察结果：a和b的地址相同）  
另一个有趣的实验是不适用标准库函数std::move，而是使用强制转换，可以达到同样的效果  
```c++
int a = 42;
int &&b = (int &&)a;
```
（这时候打印a和b的地址，观察结果: a和b的地址相同）  
但是值得一提的是，虽然本例中std::move和(int &&)最后的结果相同，但是后者是更加低级的方法；如果是一个比较复杂的对象，后者容易出错，不太安全，还是推荐用std::move来进行安全的转换。  

以上例子说明了右值引用的好处。但int本身是简单的类型，拷贝本身开销也不大。  
然而，对于比较复杂的类的对象或容器，其拷贝的成本非常巨大，右值引用的好处就显现了。  

使用了int &&b = std::move(a)后，打印a的值会观察到什么？(其实还是42，因为int是简单类型，理论上资源移走了但还是原来的地址)  
如果是vector<int> int &&b = std::move(a)，输出a的值又会观察到什么呢？(其实还是没变化)  
在右值引用中，如果一个变量的资源被移走了，它仍然是有效的，但其状态会变成未定义。这时候它可以被安全销毁。  

# 在类中使用std::move
上面提到，std::move的作用是强制把左值转换成右值，从而实现赋值的时候避免拷贝资源(也就是"转移"资源)  

一个常见的应用场景是类中使用std::move实现高效率的类的资源转移(从而避免深拷贝)  
例如，原本类只有"拷贝构造函数"和"拷贝赋值运算符"进行深拷贝，对于两者来说，深拷贝的操作都无法避免  
现在，在类中添加"移动构造函数"(很像拷贝构造函数，但函数输入使用了&&而不是&，并且等号右边需要std::move)和"移动赋值运算符"(类似，函数输入也是&&,等号右边也是用std::move来转换)  

在使用的时候，如果使用MyClass obj1=obj2; 就会调用拷贝构造函数；如果使用obj1=std::move(obj2)就会使用移动构造函数，避免了拷贝过程  

同样，如果使用MyClass obj1; obj1=obj2; 就会调用拷贝赋值运算符；如果使用MyClass obj1; obj1=std::move(obj2)就会使用移动赋值运算符，避免了拷贝过程  

综上所述，在C++中，std::move是一个标准库函数，用于实现对象的移动语义。移动语义允许资源从一个对象转移到另一个对象，而不是进行昂贵的复制操作。这在处理动态内存分配、文件句柄等资源时特别有用，可以显著提高程序的性能。  
在C++11之前，复制对象通常会涉及深拷贝操作，这可能会非常昂贵，尤其是对于包含大量数据的对象。C++11引入了移动语义，通过移动构造函数和移动赋值运算符来优化这种情况。  

# 左值引用和右值引用的对比
左值引用：  
```c++
MyCustomDataType b = 10;
MyCustomDataType &a = b;
```
左值引用就是给b起了一个别名，a和b都是用同一个资源，并避免了拷贝  

右值引用：  
```c++
MyCustomDataType b = 10; 
MyCustomDataType &&a = std::move(b); 
```
右值引用在这里很有用处，它把b的资源转移到了a，也避免了拷贝  

区别是：  
- 左值引用中，不管修改a还是b，都可以修改同一个资源  
- 右值引用中，只有修改a才能修改资源，b的资源已经没有了  
- 在函数传参的过程中，常常使用左值引用，这样可以避免拷贝，并且函数内部可以改变传进来的资源  
- 如果有一个资源要从一个对象转移到另一个对象，那就需要右值引用了。因为左值引用只能“创建”别名，不能转移资源到一个已经存在的对象上  
- 另外，当类的成员函数返回临时对象的时候，编译器会尝试调用"移动构造函数"(如果有的话)；这样接收这个临时对象的变量就能在不进行拷贝的情况下接收该资源 

# Smart Pointer
Reference: https://github.com/gpuwangge/CppAddressExperiments  

# auto和decltype  
auto 关键字用于根据初始化表达式的值来推导变量的类型。  
```c++
auto x = 10;        // x 是 int
auto y = 3.14;      // y 是 double
auto z = x + y;     // z 是 double（因为 x + y 的结果是 double）
```

decltype 是用于获取一个表达式的类型，而不是值。  
```c++
int a = 5;
decltype(a) b = a;   // b 是 int
decltype((a)) c = a; // c 是 int&，因为 (a) 是左值表达式
```

decltype在模板中的用处  
```c++
template<typename T, typename U>
void multiply_and_print(T a, U b) {
    decltype(a * b) result = a * b;
    std::cout << result << std::endl;
}
```
这样你不需要手动声明 result 是 int、float、double、还是其他什么类型。  
上面也可以替换成auto result = a * b;  
区别是decltype可以精确获取表达式的“完整类型”（包括引用、const）  

# 转发函数（std::forward）
完美转发是 C++ 中一个重要的高级技巧，主要用于 模板函数中保持参数的精确类型和引用性（包括左值/右值、const 等），避免拷贝，提高效率。  

完美转发的核心问题：在模板中，我们经常写一个“包装函数”，把参数传给另一个函数，但如果不小心，左值会变成右值，右值变成拷贝。所以我们需要完美保留调用者传入的“类型+引用性”。  
解决方案：使用 std::forward + T&&  

完美转发函数模板的写法：  
```c++
Copy code
#include <iostream>
#include <utility>  // for std::forward

// 目标函数（可以接受左值或右值）
void process(int& x) {
    std::cout << "Lvalue reference\n";
}
void process(int&& x) {
    std::cout << "Rvalue reference\n";
}

// 转发函数模板
template<typename T>
void forwardToProcess(T&& arg) {
    process(std::forward<T>(arg));  // 完美转发
}
```

使用示例：
```c++
int main() {
    int a = 10;

    forwardToProcess(a);        // 输出：Lvalue reference
    forwardToProcess(42);       // 输出：Rvalue reference
}
```
什么这样写？  
🔸 T&& arg 是所谓的 万能引用（forwarding reference）  
当传左值时，T 推导为 int&，所以 T&& 变成 int& && → int&  

当传右值时，T 推导为 int，所以 T&& 就是 int&&  

🔸 std::forward<T>(arg)  
这个函数会根据 T 是左值还是右值 有条件地做引用转换  

它的作用是：让 arg 保持调用时的值类别（左值或右值）  

如果不使用 std::forward 会发生什么？  
```c++
template<typename T>
void badForward(T&& arg) {
    process(arg);  // 总是把 arg 当成左值
}
```
无论你传什么进去，arg 是一个变量 → 永远是左值 → process(int&) 总会被调用。  

# constexpr
constexpr（constant expression）表示“常量表达式”，用于修饰函数或变量，要求它们的值在编译期间就可以确定。  
```c++
constexpr int size = 10;
int arr[size];  // 编译时大小数组，合法  
```
🆚 const vs constexpr  
```
对比项		const		constexpr  
求值时间		运行时或编译时	编译时  
适用对象		变量		变量、函数、构造函数等  
函数修饰		不支持		支持  
是否必须编译期	否（可能是运行期）是（表达式必须能在编译时求值）  
```
# Lambda表达式
Lambda 表达式是 一个可以定义在表达式中的匿名函数对象，语法如下：  
```c++
[capture](parameter_list) -> return_type {
    function_body
};
```
但大多数部分是可选的，最常见的形式为：  
```cpp
[capture](parameters) {
    body;
};
```
1. 最基础用法  
```cpp
auto add = [](int a, int b) { return a + b; };
std::cout << add(3, 4);  // 输出 7
```
2. 与标准算法配合使用  
```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};
std::for_each(vec.begin(), vec.end(), [](int x) {
    std::cout << x << " ";
});
```
捕获方式（capture）  
捕获用于 lambda 使用外部变量，有以下几种方式：  
📌 1. 按值捕获  
```cpp
int x = 10;
auto f = [x]() { std::cout << x << "\n"; };  // 拷贝x
```
📌 2. 按引用捕获  
```cpp
int x = 10;
auto f = [&x]() { x = 20; };  // 修改外部变量
```
📌 3. 默认捕获  
```cpp
int a = 1, b = 2;
auto f1 = [=]() { return a + b; };   // 默认按值
auto f2 = [&]() { b = a + b; };      // 默认按引用
```
📌 4. 混合捕获  
```cpp
auto f = [=, &y]() { y += x; };  // 默认按值，但 y 按引用
```
为什么有参数传递了，还需要用捕获？  
🧠 类比解释：  
想象你在工厂写一个“机器”（Lambda），  
参数 就像你给机器喂进去的原材料。  
捕获 就像机器自己去仓库里拿东西（你没有喂进去，但它自己知道在哪儿）。  
📚 把捕获看作“记忆环境”的能力  
Lambda 的捕获机制让你：  
把“环境上下文”封装进函数  
不需要重复传值  
实现 闭包（closure）：函数+它的执行环境  
这对 异步执行、回调、算法组合 非常有用。  
示例：封装行为（回调函数）  
```cpp
int threshold = 10;
auto is_large = [threshold](int value) {
    return value > threshold;
};
```
这个 lambda 是一个“检查是否大于阈值”的函数，它自己记得 threshold，不需要每次传进去。  
如果你改成：  
```cpp
auto is_large = [](int value, int threshold) {
    return value > threshold;
};
```
那你必须每次都传 threshold，你就无法像下面这样用：  
```cpp
std::count_if(vec.begin(), vec.end(), is_large);  // ❌错误！不匹配参数
```

# 尾返回类型（Trailing Return Type）
尾返回类型是把返回值类型写在函数名和参数列表之后，用 -> 指定。  
基本语法：  
```cpp
auto function_name(parameter_list) -> return_type {
    // function body
}
```
和传统写法相比，尾返回类型把返回值类型从前面移到了后面。  
传统写法：
```cpp
int add(int a, int b) {
    return a + b;
}
```
尾返回类型写法：
```cpp
auto add(int a, int b) -> int {
    return a + b;
}
```
虽然看起来没差别，但这为复杂类型表达式打下了基础。  
🧠 为什么需要尾返回类型？  
它的最大优势是：当你想让返回类型依赖于参数的类型时，尾返回型可以让你在写返回类型时“已经知道参数”。  
例如：  
```cpp
template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}
```
这里用到了 decltype(a + b) 来推导返回类型。因为参数 a 和 b 已经写完了，所以 decltype(a + b) 是合法的。  
✨ 应用场景详解  
1. 模板函数中使用 decltype  
```cpp
template<typename T1, typename T2>
auto multiply(T1 a, T2 b) -> decltype(a * b) {
    return a * b;
}
```
如果用传统写法，就必须提前知道 a * b 的类型，但参数还没出现，编译器不知道你要干嘛。  
2. 与 auto 配合实现类型推导  
```cpp
auto get_value() -> int {
    return 42;
}
```
这在写泛型代码时统一语法风格、支持复杂返回类型时非常有用。  
3. 在类中配合模板和类型别名  
```cpp
template<typename T>
class Wrapper {
    T value;
public:
    auto get() const -> const T& {
        return value;
    }
};
```

# 委托构造函数（Delegating Constructors）
委托构造函数就是一个构造函数调用类中另一个构造函数，来复用已有的初始化逻辑。  
以前（C++11 之前），构造函数之间不能互相调用，只能通过提取公共初始化代码到私有函数来间接实现。  
举个例子（没有委托构造函数的写法）  
```cpp
class Person {
public:
    Person() {
        name = "Unknown";
        age = 0;
    }

    Person(std::string n) {
        name = n;
        age = 0;
    }

    Person(std::string n, int a) {
        name = n;
        age = a;
    }

private:
    std::string name;
    int age;
};
```
使用委托构造函数优化  
```cpp
class Person {
public:
    Person() : Person("Unknown") { }
    Person(std::string n) : Person(n, 0) { }
    Person(std::string n, int a) : name(n), age(a) { }
private:
    std::string name;
    int age;
};
```
Person() 委托给 Person(std::string)  
Person(std::string) 委托给 Person(std::string, int)  
最终的初始化工作由 Person(std::string, int) 完成  
这就实现了代码复用、减少重复、提高可读性和可维护性。  

注意事项（⚠️）  
不能循环委托：即构造函数不能相互无限递归调用，否则编译失败。  
```cpp
Person() : Person(0) { }
Person(int x) : Person() { } // ❌ 错误：无限递归
```
委托构造函数不能访问非静态成员变量，直到被委托的构造函数完成（因为对象还没完全初始化）。  
委托构造函数也可以和继承、成员初始化等一起使用，但要注意顺序。  

适用场景  
委托构造函数特别适合：  
有多个构造函数有部分重复逻辑  
想要定义“默认参数”的构造行为（但不使用默认参数语法）  
构造逻辑复杂，希望复用核心构造逻辑  

#  默认函数（= default）和删除函数（= delete） 
✅ 一、默认函数（= default）  
🔹 概念：  
告诉编译器：“我希望使用该函数的默认实现”，即便我声明了它。  

🔹 使用场景：
你希望显式说明：我没有手动写实现，但我确实希望使用默认的行为。  
与其他特性共用（比如构造函数被声明了另一个版本，导致默认构造函数不再自动生成）。  

🔹 示例：  
```cpp
class Person {
public:
    Person() = default;                   // 使用编译器默认的构造函数
    Person(const Person&) = default;      // 使用默认拷贝构造函数
    Person& operator=(const Person&) = default; // 使用默认赋值操作
    ~Person() = default;                  // 默认析构函数

private:
    std::string name;
    int age;
};
```

✅ 优势：  
代码更清晰  
更易于维护  
编译器可以生成比你手写更高效的代码（尤其是 trivial 类型）  

✅ 二、删除函数（= delete）  
🔹 概念：  
告诉编译器：“禁止这个函数被调用”，即使它本来可以被自动生成。  

🔹 使用场景：  
禁止拷贝/移动：  
```cpp
class NoCopy {
public:
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = delete;
};
```
禁止某些类型的隐式转换或不希望使用的重载：  
```cpp
void func(int) = delete;
void func(double); // 只允许 double
```
禁止默认构造：  
```cpp
class OnlyWithArgs {
public:
    OnlyWithArgs(int x) { }
    OnlyWithArgs() = delete; // 不允许无参构造
};
```
🔹 示例：  
```cpp
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

private:
    Singleton() = default;  // 允许私有构造
};
```

✅ 三、对比总结：default vs delete  
```
特性	= default	                    = delete  
作用	使用编译器默认生成的函数	        明确禁止函数的使用  
常用位置	构造函数、拷贝/移动构造、赋值操作	同上，以及普通函数  
编译行为	编译器提供默认实现	            编译器报错禁止调用  
```

✅ 四、特别注意  
= default 和 = delete 只能用于类成员函数（包括构造/析构/运算符重载）或普通函数。  
不适用于变量或非函数声明。  
它们的使用可以与 C++ 的其他控制机制（如私有构造、继承、模板）联合使用来构建更加安全的类型。  

✅ 五、扩展应用：配合模板与 SFINAE  
```cpp
template<typename T>
void process(T) = delete; // 通用模板被删除

void process(int x) {
    // 只允许 int 类型
}
```
这种做法可用于强制特定类型才可调用函数，提高类型安全。  

# 空指针常量 nullptr
C++11 引入的 nullptr 是一种专门的空指针字面量（literal），用于代替传统的 NULL 或 0，表示一个空指针值。  
它的类型是：  
```cpp
std::nullptr_t
```

为什么引入 nullptr？  
🔶 C++98/03 中的问题：  
在旧的 C++ 中，常用 NULL 或 0 表示空指针：  
```cpp
void func(int);
void func(char*);

func(NULL); // 问题：到底调用哪个？
```
在大多数实现中：  
```cpp
#define NULL 0
```
所以实际上 func(NULL) 变成了 func(0) —— 编译器倾向于调用 func(int)，这可能不是你想要的效果。
🔶 示例问题：  
```cpp
#include <iostream>

void func(int) { std::cout << "int\n"; }
void func(char*) { std::cout << "char*\n"; }

int main() {
    func(NULL);    // 输出 int ✅（但我们想要 char* ❌）
    func(0);       // 也是 int
    func(nullptr); // 输出 char* ✅
}
```

nullptr 的优点  
```
特性	                NULL	    0	        nullptr
类型	                int	        int	        std::nullptr_t
类型安全	                ❌ 否	    ❌ 否	    ✅ 是
可用于指针重载选择       ❌不安全    ❌ 不安全	✅ 正确选择
模板推导友好             ❌ 否	    ❌ 否	    ✅ 是
```

nullptr 的行为和语法示例  
🔹 基本用法：  
```cpp
int* p1 = nullptr;
double* p2 = nullptr;

if (p1 == nullptr) {
    std::cout << "p1 is null\n";
}
```
🔹 函数重载（避免歧义）：  
```cpp
void f(int);
void f(void*);

f(nullptr); // 调用 void* 版本（正确！）
```

与模板配合：  
```cpp
template<typename T>
void test(T val) {
    std::cout << typeid(val).name() << "\n";
}

int main() {
    test(nullptr); // T 被推导为 std::nullptr_t
}
```
🔹 std::nullptr_t 是什么？  
nullptr 的类型是 std::nullptr_t，定义在头文件 <cstddef> 中：  
```cpp
#include <cstddef>

std::nullptr_t np = nullptr; // OK
```
它唯一能隐式转换的目标类型是任意类型的指针（包括成员指针）。  

类型转换规则  
```cpp
int* p = nullptr;    // OK
int n = nullptr;     // ❌ 编译错误
```
也就是说：  
nullptr 可以自动转换为任意类型的指针  
但不能转换为整型，所以它比 0 和 NULL 更类型安全  

对比示例总结  
```cpp
void func(int);
void func(char*);

func(0);        // 调用 func(int)
func(NULL);     // 调用 func(int)，因为 NULL 是 0
func(nullptr);  // 调用 func(char*)
```

最佳实践建议  
在现代 C++ 中，始终使用 nullptr 代替 NULL 和 0 来表示空指针。  
将 NULL 视为历史遗留，不应在新代码中继续使用。  



