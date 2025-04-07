# Build Instruction
Go to the project dir  
> mkdir build  
> cd build  
> cmake -G "MinGW Makefiles" ..  
> make  
  

# Output Example
指针指向的地址都是虚拟地址。其值在编译的时候由编译器分配。真正的物理地址由操作系统分配。  
指针的值是变量的地址。指针自己也是一个"变量"（指针变量）。  
既然指针也是变量，它自己也有大小(64位架构下为64位，比int的32位还大)和地址。可以通过指向指针的指针来获得指针变量的地址。  

```
Variable
a = 10
&a = 000000000069fdec
sizeof(a) = 4
typeid(a).name() = i

Pointer
*p_int = 10
p_int = 000000000069fdec
sizeof(p_int) = 8
typeid(p_int).name() = Pi

Pointer points to pointer
**p_p_int = 10
p_p_int = 000000000069fde0
sizeof(p_p_int) = 8
typeid(p_p_int).name() = PPi

Array
array[i] = 1 2 3 4 5
&array[i] = 000000000069fdc0 000000000069fdc4 000000000069fdc8 000000000069fdcc 000000000069fdd0
sizeof(array) = 20
typeid(array).name() = A5_i

Buffer
buffer[i] = 127 127 127 127 127 127 127 127
&buffer[i] = 0000000000f51680 0000000000f51681 0000000000f51682 0000000000f51683 0000000000f51684 0000000000f51685 0000000000f51686 0000000000f51687
sizeof(buffer) = 8
typeid(buffer).name() = Pc

Raw Pointer
PS E:\GitHubRepo\CppAddressExperiments\AddressExample\bin> .\addressProject.exe
Variable
a = 10
&a = 000000000069fdec
sizeof(a) = 4
typeid(a).name() = i

Pointer
*p_int = 10
p_int = 000000000069fdec
sizeof(p_int) = 8
typeid(p_int).name() = Pi

Pointer points to pointer
**p_p_int = 10
*p_p_int = 000000000069fdec
p_p_int = 000000000069fde0
sizeof(p_p_int) = 8
typeid(p_p_int).name() = PPi

Array
array[i] = 1 2 3 4 5
&array[i] = 000000000069fdc0 000000000069fdc4 000000000069fdc8 000000000069fdcc 000000000069fdd0
sizeof(array) = 20
typeid(array).name() = A5_i

Buffer
buffer[i] = 127 127 127 127 127 127 127 127 
&buffer[i] = 0000000001081680 0000000001081681 0000000001081682 0000000001081683 0000000001081684 0000000001081685 0000000001081686 0000000001081687
sizeof(buffer) = 8
typeid(buffer).name() = Pc
```

# 进程地址空间，堆和栈关系
在多任务操作系统中，每个进程运行在属于自己的内存沙盘(Virtual Address Space)中。  
32位下是一个4GB(2^32)的内存地址快。  
Linux下内核进程和用户进程所占的虚拟内存比例为1:3；Windows为2:2。根据需要，它们可以被映射到物理内存。  

内存按用途分类：
- 栈：局部变量(编译器分配)，函数参数，返回地址。进程中的每个线程都有自己的栈，size约2~10Mb。栈由专门的寄存器实现，效率高。  
- 堆：动态分配的内存，不能用名字访问，只能用指针访问，32位系统中将近2.9Gb。对由函数库提供，效率低。    
(以上为应用程序运行时维护的内存)  
- BSS段：未初始化或初值为零的全局变量和静态局部变量  
- 数据段：已初始化且初值为非零的全局变量和静态局部变量  
- 代码段：可执行代码、字符串字面值、只读变量  
(以上为操作系统加载应用程序的时候维护的内存)  
- 内核空间：总是驻留在内存中，不允许应用程序读写  

虚拟地址排布是连续的，但物理地址的排布是不连续的。  


# ASLR
= Address Space Layout Randomization=地址空间布局随机化  
一个编译器参数  
是一种针对缓冲区溢出的安全保护技术  
没有ASLR时每次进程执行，加载到内容中，代码所处堆栈stack的位置都是相同的，容易被识别出所在位置，容易被破解。  
如果开启了ASLR=机制：操作系统加载器会针对基地址再去加上一个随机生成的偏移地址，然后再去加载程序模块

# void指针
当有类型的指针转换为 void 指针时，保存的内存地址是不变的。然而，类型信息会丢失，所以你不能进行加减操作。  
(指针类型只影响加减操作，因为内存地址长度在同一构架里是固定的：32bit架构里是4字节；64bit架构里是8字节)  
这样的操作本身就是容易出错的，必须很谨慎。现代 C++ 几乎在所有场景中都不建议使用 void 指针。  

# raw指针
原始指针是指其生存期不受封装对象控制的指针，例如智能指针。

# 智能指针
## 动机
当一个class在堆上实例化之后，有可能忘记delete造成内存泄露。  
就算没有忘记delete，也有可能在未运行到delete那一行的时候程序crash，造成内存泄漏。  
一个解决办法是加上throw exception()，在catch里delete。但在一个大型项目里，这样做所增加的复杂度也是一个灾难。  
因此，我们希望有一种机制，使得堆上使用的内存也能像栈上的变量一样，用完之后就自动释放，完全杜绝内存泄露的风险。  
设计思路：智能指针将普通的指针封装成类对象指针。在类的析构函数里delete掉内存。当然这个智能指针要用模板，以支持不同的类型。  
## C++11支持的智能指针
shared_ptr, weak_ptr, unique_ptr是C++11支持的智能指针。(头文件：<memory>)  
(auto_ptr已经被C++11弃用)
- shared_ptr
采用引用计数的智能指针。 如果你想要将一个原始指针分配给多个所有者（例如，从容器返回了指针副本又想保留原始指针时），请使用该指针。 直至所有 shared_ptr 所有者超出了范围或放弃所有权，才会删除原始指针。 大小为两个指针；一个用于对象，另一个用于包含引用计数的共享控制块。   
- unique_ptr
只允许基础指针的一个所有者。 除非你确信需要 shared_ptr，否则请将该指针用作 POCO 的默认选项。 可以移到新所有者，但不会复制或共享。 替换已弃用的 auto_ptr。 与 boost::scoped_ptr 比较。 unique_ptr 很小且高效;大小是一个指针，它支持用于从 c + + 标准库集合快速插入和检索的右值引用。   
- weak_ptr
结合 shared_ptr 使用的特例智能指针。 weak_ptr 提供对一个或多个 shared_ptr 实例拥有的对象的访问，但不参与引用计数。 如果你想要观察某个对象但不需要其保持活动状态，请使用该实例。 在某些情况下，需要断开 shared_ptr 实例间的循环引用。   


# Reference
https://book.crifan.com/books/explore_underlying_mechanism_binary_security/website/windows/security_mechanism/alsr.html  
https://blog.csdn.net/icandoit_2014/article/details/53502046  
https://cloud.tencent.com/developer/article/2247349  
https://www.cnblogs.com/sunsky303/p/16497592.html  

