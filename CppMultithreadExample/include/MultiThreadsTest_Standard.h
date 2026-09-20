#define pragma once
#include "common.h"

#include <iostream>
#include <thread>
#include <mutex>

int counter = 0;        // 共享变量
std::mutex m;           // 互斥量

void add() {
    std::lock_guard<std::mutex> lock(m);  // 自动加锁、自动解锁
    counter++;                            // 临界区：只能一个线程进入
}

int MultiThreadsTest_Standard1() {
	std::cout << "=====MultiThreadsTest_Standard1=====" << std::endl;
    std::thread t1(add);
    std::thread t2(add);

    t1.join(); //让当前线程阻塞，直到目标线程执行结束。主线程（main()）停在这里不往下执行。等 t1 完成。
    t2.join(); //再等 t2 完成
    //两个线程都结束后，主线程才继续执行 

    std::cout << "MultiThreadsTest_Standard1: Final counter value: " << counter << std::endl;    // 输出 2

	return 0;
}



/*
两个线程共享 logBuffer
	std::vector<std::string> logBuffer;
如果不加锁，两个线程同时 push_back 会导致：
	数据竞争（race condition）
	vector 内部扩容时崩溃
	日志顺序混乱
*/

/*
用 mutex 保护共享资源
	std::lock_guard<std::mutex> lock(logMutex);
lock_guard 的好处：
	自动加锁
	自动解锁（作用域结束时）
	不会忘记 unlock（避免死锁）
*/

/*
两个线程各自执行任务
	A 线程每 100ms 产生一条数据
	B 线程每 150ms 写一条日志
它们的速度不同，但都安全地写入同一个 logBuffer。
*/

/*
主线程等待两个线程结束
	tA.join();
	tB.join();
*/

/*
这个例子体现了 mutex 的真实用途
	保护复杂共享资源（vector）
	多个线程执行不同任务
	避免数据竞争与崩溃
	RAII 风格的锁管理
	真实工程中的日志/队列/缓冲区模式
*/

std::vector<std::string> logBuffer;   // 共享资源：日志缓冲区
std::mutex logMutex;                  // 互斥量保护共享资源

void threadA() {
    for (int i = 0; i < 5; ++i) {
        {//括号的作用：std::lock_guard 是 RAII，它会在 作用域结束时自动 unlock。让 lock_guard 的生命周期只覆盖 push_back，而不是整个循环体。
            std::lock_guard<std::mutex> lock(logMutex);
			//lock 对象被创建，它的构造函数会自动去调用 logMutex.lock()。
			//如果此时没有其他线程在用 logMutex，当前线程拿到锁，继续往下执行。
			//如果其他线程已经拿到了锁，当前线程就会阻塞在这里原地等待，直到别的线程把锁释放

            logBuffer.push_back("A: data packet " + std::to_string(i));
        }
		//离开作用域时（自动解锁）：当代码执行到当前大括号 } 结束，或者中间遇到了 return、break 甚至是抛出了异常，
		//lock 对象的生命周期结束。
		//此时会自动触发它的析构函数，隐式调用 logMutex.unlock() 把锁还回去。

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//在这个代码片段中，sleep_for 的核心作用是让出 CPU 时间片，防止当前线程“独霸”互斥锁（避免线程饥饿），同时也是为了模拟真实的业务耗时。
		//加上 sleep_for(150ms) 后，threadB 在解锁后会强制休眠 150 毫秒。
		//这就明确地告诉操作系统：“我这会儿没事干了，你可以把 CPU 分配给其他线程了”。这样其他排队的线程就能顺利拿到锁。
    }
}

void threadB() {
    for (int i = 0; i < 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(logMutex);
            logBuffer.push_back("B: log entry " + std::to_string(i));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int MultiThreadsTest_Standard2() {
	std::cout << "=====MultiThreadsTest_Standard2=====" << std::endl;
    std::thread tA(threadA); //std::thread 的机制是“一旦构造，立即启动”。
    std::thread tB(threadB);

    tA.join(); //join() 的直观意思是：“主线程在这里停下（阻塞），等这个子线程彻底干完活，再接着往下走。”
    tB.join();

	/* Join（汇合）：当子线程执行完毕，它的生命周期走到尽头。此时，这条分支路线必须重新“汇入”主干道。
	在这段特定的代码中，必须写这两个 join()，有两个极其致命的原因：
	原因一：防止程序直接崩溃（C++ 语法铁律）
	在 C++ 中，如果一个子线程对象（如 tA）的作用域结束被销毁了，但它对应的底层线程还在后台偷偷跑，C++ 运行时会认为这是一个严重的失控错误，并强制调用 std::terminate()，直接把整个程序强行杀死（崩溃）。
	通过 join()，主线程会乖乖等待 tA 和 tB 正常死亡后，再去销毁它们的对象，保证了程序的安全退出。
	原因二：保证业务逻辑的正确性（等待结果）
	*/

	/* std::barrier (C++20 引入)介绍
	join() 在宏观概念上确实和 Barrier（屏障/栅栏） 有相似之处：它们都是一种同步机制，都会让代码在某一行“卡住”，等待某个条件满足后再放行。
	但在多线程编程的严格定义中，join() 和真正的 Barrier（比如 C++20 引入的 std::barrier）在语义和用法上有本质的区别。
	我们可以用一句话来概括它们的区别：
		join() 是“等待死亡”：一个线程等待另一个线程彻底结束生命周期。
		Barrier 是“集合点”：一群活着的线程互相等待，人齐了再一起往下走。
	*/

    // 主线程读取最终日志
    for (auto& s : logBuffer) {
        std::cout << s << std::endl;
    }

	return 0;
}


