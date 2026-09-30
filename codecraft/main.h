#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <memory>


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
    void GoldCraft() override{
        std::unique_ptr<int[]> arr;
        arr = std::make_unique<int[]>(3);
        arr[0] = 1;
        arr[1] = 2;
        arr[2] = 3;
        std::cout<<arr[0]<<","<<arr[1]<<","<<arr[2]<<std::endl;
    }
    void TestCraft() override;
};




