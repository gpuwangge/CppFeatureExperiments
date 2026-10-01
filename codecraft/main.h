#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <memory>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <algorithm>

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
        std::cout<<"Create an array of smart pointer, print 1,2,3"<<std::endl;
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
        std::cout<<"Create a lambda function(count 1m with a shared variable), then create 2 threads, total count 2m"<<std::endl;
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


class HashtableTest: public CraftBase{
public:
    HashtableTest(){
        std::cout<<"Create a HashtableTest object. Try find/insert/erase operations"<<std::endl;
    }
    void TestCraft() override;
    void GoldCraft() override{
        std::unordered_map<std::string, int> age;
        age["Alice"] = 30;
        age["David"] = 25;
        age["Charlie"] = 35;
        age.insert(std::make_pair<std::string, int>("Bob", 99));

        std::cout << "Alice's age: " << age["Alice"] << std::endl; // 输出 30
        if(age.find("Bob") != age.end()) 
            std::cout << "Bob's age: " << age["Bob"] << std::endl; // 输出 99

        age.erase("Charlie");
        if(age.find("Charlie") == age.end()) 
            std::cout << "Charlie not found" << std::endl; // 输出 "Charlie not found"

        for(auto it = age.begin(); it != age.end(); it++)
            std::cout << it->first << "=" << it->second << " ";
        std::cout << std::endl;
    }
};

class SortTest: public CraftBase{
public:
    SortTest(){
        std::cout<<"Sort a vector of integer: arr, use lambda function"<<std::endl;
    }
    std::vector<int> arr{5, 2, 9, 1, 5, 6};
    void TestCraft() override;
    void GoldCraft() override{
        auto compare = [](int a, int b) { return a > b;};
        std::sort(arr.begin(), arr.end(), compare);
        std::cout << "Sorted array: ";
        for(auto& num : arr){
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
};




