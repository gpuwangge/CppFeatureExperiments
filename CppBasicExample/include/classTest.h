#ifndef __CLASSTEST_H__
#define __CLASSTEST_H__

#include "common.h"

//Demo of Inheritance/Polymorphism


//Example Base Class
class BaseClass{
public:
    string m_tag;
public:
    BaseClass(string tag){ m_tag = tag; cout<<"Base class construct. Tag: "<<m_tag<<endl;}
    ~BaseClass(){ cout<<"Base class deconstruct. Tag: "<<m_tag<<endl; }
    void Run(){ cout<<"Run base class Tag: "<<m_tag<<endl;}
    virtual void VirtualRun(){ cout<<"Run(virtual) base class Tag: "<<m_tag<<endl;}
};

//Example Derived Class
class DerivedClass: public BaseClass{
public:
    DerivedClass(string tag) : BaseClass(tag){ m_tag = tag; cout<<"Derived class construct. Tag: "<<m_tag<<endl; }
    ~DerivedClass(){ cout<<"Derived class deconstruct. Tag: "<<m_tag<<endl; }
    void Run(){ cout<<"Run derived class Tag: "<<m_tag<<endl; }
    void VirtualRun(){ cout<<"Run(virtual) derived class Tag: "<<m_tag<<endl; }
};


void ClassTextI_BaseClass_DerivedClass(){
    cout<<"=====Class Test I (Base Class and Derived Class)====="<<endl;

    cout<<"<Base Class Run Test>"<<endl;
    BaseClass baseClass("Base");
    baseClass.Run();
    cout<<endl;
    ///result：base construct，base run
    
    cout<<"<Derived Class Run Test>"<<endl;
    DerivedClass derivedClass("Derived");
    derivedClass.Run();
    cout<<endl;
    ///result：base construct, derived construct, derived run
    
    cout<<"<Base Pointer Run Test>"<<endl;
    BaseClass *p = &baseClass; //use base pointer points to base object
    p->Run();
    cout<<endl;
    ///result：base run
    
    cout<<"<Base Pointer to Derived Run Test>"<<endl;
    p = &derivedClass; //use base pointer points to derived object
    p->Run();
    cout<<endl;
    ///result：even the object is derived, still base run
    
    cout<<"<Base Pointer to Derived Virtual Run Test>"<<endl;
    p = &derivedClass; //use base pointer points to derived class
    p->VirtualRun(); //virtual run this time
    cout<<endl;
    ///result：derived run. Must use virtual if want to run derived member function
    
    cout<<"<Destructor Test>"<<endl;
    ///result：first object last destruct; last object first destruct
}

void ClassTextII_DestructorIssue(){
    cout<<"=====Class Test II (Destructor Issue)====="<<endl;
    cout<<"<New Derived Object Creation>"<<endl;
    BaseClass *p = new DerivedClass("Derived");
    cout<<endl;
    
    cout<<"<New Derived Object Destructor Test>"<<endl;
    delete p;
    ///result：only run base destructor
    ///If you create derived class with a base pointer, the derived destructor is not called
}


//another base class, with virtual destructor
class BaseClassVirtualDeconstructor{ 
public:
    string m_tag;
public:
    //Note: Constructor can not be virtual
    BaseClassVirtualDeconstructor(string tag){ m_tag = tag; cout<<"Base class construct. Tag: "<<m_tag<<endl; }
    virtual ~BaseClassVirtualDeconstructor(){ cout<<"Base class deconstruct. Tag: "<<m_tag<<endl;}
};

//another derived class
class DerivedClassVirtualDeconstructor: public BaseClassVirtualDeconstructor{
public:
    DerivedClassVirtualDeconstructor(string tag) : BaseClassVirtualDeconstructor(tag){
        cout<<"Derived class construct. Tag: "<<m_tag<<endl;
        m_tag = tag;
    }
    ~DerivedClassVirtualDeconstructor(){ cout<<"Derived class deconstruct. Tag: "<<m_tag<<endl;}
};

void ClassTextIII_VirtualDestructor(){
    cout<<"=====Class Test III (Virtual Destructor)====="<<endl;
    cout<<"<New Derived Object Creation(with virtual destructor)>"<<endl;
    BaseClassVirtualDeconstructor *p = new DerivedClassVirtualDeconstructor("BasePointer2DerivedObject");
    cout<<endl;
    
    cout<<"New Derived Object Destructor Test(with virtual destructor)>"<<endl;
    delete p;
    ///result：call derived destructor and base destructor. The issue is solved
}


class AbstractClass{
public:
    string m_tag;
    virtual void AbstractMemberFunction() = 0; //a special virtual function: make an abstract class; abstract class can't be instantialized
    //derived class must implement all base abstract function in order to be instantialized
};
class DerivedAbstractClass : AbstractClass{
public:
    DerivedAbstractClass(string tag){ m_tag = tag; }
    void AbstractMemberFunction(){ ///must implement this function to avoid compile failure
        cout<<"Call function of a derived class from an abstract class. Tag: "<<m_tag<<endl;
    }
};

void ClassTestIV_PureVirtualFunction(){
    cout<<"=====Class Test IV (Pure Virtual Function)====="<<endl;
    DerivedAbstractClass derivedAbstractClass("Abstract"); 
    derivedAbstractClass.AbstractMemberFunction();
    cout<<endl;
}



class CopyExampleClass{
public:
    int *m_id;
    CopyExampleClass(){ m_id = new int(1); }
    ~CopyExampleClass(){ cout<<"Destructor: "<<*m_id<<endl; delete m_id; }
    void PrintId(){ cout<<*m_id<<endl;}

    CopyExampleClass(const CopyExampleClass &c){
        cout<<"Call Copy Constructor"<<endl;
        m_id = new int(*(c.m_id));
    }

    CopyExampleClass& operator =(const CopyExampleClass &c){ 
        cout<<"Call Assign Operator to Copy"<<endl;
        if(this == &c) return *this;
        if(m_id != NULL) delete m_id;
        m_id = new int(*(c.m_id));
        return *this;
    }
    
    CopyExampleClass* Clone() const {  ///Clone: const means member variable can't be changed
        cout<<"Call Clone()"<<endl;
        return new CopyExampleClass(*this);
        //return new CopyExampleClass();///if use this way, it is considered create new object other than clone
    }
};

void ClassTextV_Copy(){
    cout<<"=====Class Test V (Copy)====="<<endl;

    cout<<"<Reference>"<<endl;
    CopyExampleClass copyExampleClass;
    copyExampleClass.PrintId();///result：1
    cout<<endl;
    
    cout<<"<Copy Constructor Function Test>"<<endl;
    CopyExampleClass copyExampleClass2 = copyExampleClass;///call copy constructor
    (*copyExampleClass2.m_id) = 2;
    copyExampleClass2.PrintId();///result：2
    copyExampleClass.PrintId();///result：1 (if not use copy constructor, this is also 2) 
    cout<<endl;
    
    cout<<"<Assign Operator Copy Test>"<<endl;
    CopyExampleClass copyExampleClass3;
    copyExampleClass3 = copyExampleClass;
    (*copyExampleClass3.m_id) = 3;
    copyExampleClass3.PrintId();///result：3
    copyExampleClass.PrintId();///result：1 (if not use assign operator, this is also 2) 
    cout<<endl;
    
    cout<<"<Clone Test>"<<endl;
    CopyExampleClass *copyExampleClass4 = copyExampleClass.Clone();
    (*copyExampleClass4->m_id) = 4;
    copyExampleClass4->PrintId();///result：4
    copyExampleClass.PrintId();///result：1
    cout<<endl;
    delete copyExampleClass4;

    
}

#endif