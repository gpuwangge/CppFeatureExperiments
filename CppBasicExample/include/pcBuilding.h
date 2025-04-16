#ifndef __PCBUILDING_H__
#define __PCBUILDING_H__

#include "common.h"

//To assemble a personal computer(PC), we need CPU, GPU and Memory
//reference: https://www.bilibili.com/video/BV1et411b73Z?spm_id_from=333.788.videopod.episodes&vd_source=e9d9bc8892014008f20c4e4027b98036&p=142


//Abstract Classes
class CPU{
public:
    virtual void calculate() = 0;
};

class GPU{
public:
    virtual void display() = 0;
};
  
class Memory{
public:
    virtual void storage() = 0;
};


class Computer{
    CPU *m_cpu;
    GPU *m_gpu;
    Memory *m_mem;

public:
    Computer(CPU *cpu, GPU *gpu, Memory *mem){
        m_cpu = cpu;
        m_gpu = gpu;
        m_mem = mem;
    }

    ~Computer(){
        if(m_cpu != NULL) {delete m_cpu; m_cpu = NULL;}
        if(m_gpu != NULL) {delete m_gpu; m_gpu = NULL;}
        if(m_mem != NULL) {delete m_mem; m_mem = NULL;}
    }

    void work(){
        m_cpu->calculate();
        m_gpu->display();
        m_mem->storage();
    }
};


class IntelCPU: public CPU{
public:
    void calculate(){
        cout << "Intel CPU starts to calculate!" << endl;
    }
};

class IntelGPU: public GPU{
public:
    void display(){
        cout << "Intel GPU starts to display!" << endl;
    }
};

class IntelMemory: public Memory{
public:
    void storage(){
        cout << "Intel Memory starts to storage!" << endl;
    }
};
        
class LenovoCPU: public CPU{
public:
    void calculate(){
        cout << "Lenovo CPU starts to calculate!" << endl;
    }
};

class LenovoGPU: public GPU{
public:
    void display(){
        cout << "Lenovo GPU starts to display!" << endl;
    }
};

class LenovoMemory: public Memory{
public:
    void storage(){
        cout << "Lenovo Memory starts to storage!" << endl;
    }
};
      

void PCBuildingTest(){
    cout<<"=====PC Building Test====="<< endl;

    CPU *intelCPU = new IntelCPU;
    GPU *intelGPU = new IntelGPU;
    IntelMemory *intelMemory = new IntelMemory;

    Computer *computer1 = new Computer(intelCPU, intelGPU, intelMemory);
    computer1->work();
    cout<<endl;
    delete computer1;

    Computer *computer2 = new Computer(new LenovoCPU, new LenovoGPU, new LenovoMemory);
    computer2->work();
    cout<<endl;
    delete computer2;

    Computer *computer3 = new Computer(new IntelCPU, new LenovoGPU, new IntelMemory);
    computer3->work();
    cout<<endl;
    delete computer3;
}



#endif