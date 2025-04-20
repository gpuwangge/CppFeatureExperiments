#ifndef __CUSTOMARRAY_H__
#define __CUSTOMARRAY_H__

#include "common.h"

//Reference: https://www.bilibili.com/video/BV1et411b73Z?spm_id_from=333.788.videopod.episodes&vd_source=e9d9bc8892014008f20c4e4027b98036&p=183

template<typename T>
class MyArray{
    T *pAddress;
    int m_capacity;
    int m_size;
public:
    MyArray(int capacity){
        //cout<<"Call MyArray Constructor."<<endl;
        m_capacity = capacity;
        m_size = 0;
        pAddress = new T[capacity];
    }

    ~MyArray(){
        //cout<<"Call MyArray Destructor."<<endl;
        if(this->pAddress != NULL){
            delete []pAddress;
            pAddress = NULL;
        }
    }

    MyArray(const MyArray &arr){
        //cout<<"Call MyArray Copy Constructor."<<endl;
        m_capacity = arr.m_capacity;
        m_size = arr.m_size;
        //pAddress = arr.pAddress;//shallow copy

        //deep copy
        this->pAddress = new T[arr.m_capacity]; 
        for(int i = 0; i < m_size; i++) this->pAddress[i] = arr.pAddress[i];
    }

    MyArray& operator=(const MyArray &arr){ //add& to allaw a=b=c
        //cout<<"Call MyArray Assignment."<<endl;
        if(this->pAddress != NULL){
            delete[] this->pAddress;
            this->pAddress = NULL;
            this->m_capacity = 0;
            this->m_size = 0;
        }

        m_capacity = arr.m_capacity;
        m_size = arr.m_size;
        pAddress = new T[arr.m_capacity]; 
        for(int i = 0; i < m_size; i++) pAddress[i] = arr.pAddress[i];

        return *this;
    }

    void Push_Back(const T& val){
        if(m_capacity == m_size) return;

        this->pAddress[m_size] = val;
        m_size++;
    }

    void Pop_Back(){
        if(m_size == 0) return;
        m_size--;
    }

    T& operator[](int index){ //add & so the return value can be left value
        return this->pAddress[index];
    }

    int getCapacity(){
        return m_capacity;
    }

    int getSize(){
        return m_size;
    }

};

void printIntArray(MyArray <int>& arr){
    for(int i = 0; i < arr.getSize(); i++)
        cout<<arr[i]<<", ";
    cout<<endl;
}

class Person{
public:
    string m_name;
    int m_age;
public:
    Person() {};
    Person(string name, int age){
        m_name = name;
        m_age = age;
    }
};

void printPersonArray(MyArray <Person>& arr){
    for(int i = 0; i < arr.getSize(); i++){
        cout<<"Name: "<<arr[i].m_name<<", ";
        cout<<"Age: "<<arr[i].m_age<<endl;
    }
}


void CustomArrayTest_Int(){
    cout<<"=====Custom Array Test(Int)====="<< endl;
    MyArray<int> arr1(5);
    for(int i = 0; i < 5; i++) arr1.Push_Back(i);
    cout<<"arr1 = ";
    printIntArray(arr1);
    cout<<"arr1's capacity = "<<arr1.getCapacity()<<", arr1's size = "<<arr1.getSize()<<endl;

    MyArray<int> arr2(arr1);
    cout<<"arr2 = ";
    printIntArray(arr2);
    cout<<"arr2's capacity = "<<arr2.getCapacity()<<", arr2's size = "<<arr2.getSize()<<endl;
    arr2.Pop_Back();
    cout<<"arr2(after Pop_Back) = ";
    printIntArray(arr2);
    cout<<"arr2's capacity = "<<arr2.getCapacity()<<", arr2's size = "<<arr2.getSize()<<endl;
    cout<<endl;

    // MyArray<int> arr3(100);
    // arr3 = arr1;
    // cout<<endl;
}

void CustomArrayTest_Person(){
    cout<<"=====Custom Array Test(Person)====="<< endl;
    MyArray<Person> arr1(10);
    Person p1("Sun Wukong", 999);
    Person p2("Han Xin", 30);
    Person p3("Da Ji", 20);
    Person p4("Zhao Yun", 25);
    Person p5("An Qila", 26);

    arr1.Push_Back(p1);
    arr1.Push_Back(p2);
    arr1.Push_Back(p3);
    arr1.Push_Back(p4);
    arr1.Push_Back(p5);

    cout<<"arr1 = "<<endl;
    printPersonArray(arr1);
    cout<<"arr1's capacity = "<<arr1.getCapacity()<<", arr1's size = "<<arr1.getSize()<<endl;
}

#endif