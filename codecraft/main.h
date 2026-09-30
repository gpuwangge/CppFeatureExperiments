#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <memory>

#include <thread>
#include <mutex>

class CraftBase{
public:
    CraftBase(){
        std::cout<<"================================================"<<std::endl;
    }
    ~CraftBase(){}
    void StartCraft(){
        std::cout<<"Gold Craft: "; GoldCraft();
        std::cout<<"Test Craft: "; TestCraft();
        std::cout<<std::endl;
    }
    virtual void GoldCraft() = 0;
    virtual void TestCraft() = 0;
};


class SmartPointerCraft : public CraftBase{
public:
    SmartPointerCraft(){
        std::cout<<"Q: Create an array of smart pointer, print 1,2,3"<<std::endl;
    }
    void TestCraft() override;
    void GoldCraft() override{
        std::unique_ptr<int[]> arr;
        arr = std::make_unique<int[]>(3);
        arr[0] = 1;
        arr[1] = 2;
        arr[2] = 3;
        std::cout<<arr[0]<<","<<arr[1]<<","<<arr[2]<<std::endl;
    }
    
};

class MultiThreadTest: public CraftBase{
public:
    MultiThreadTest(){
        std::cout<<"Q: Create a lambda function(count 1m with a shared variable), then create 2 threads, total count 2m"<<std::endl;
    }
    void TestCraft() override;
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
};




