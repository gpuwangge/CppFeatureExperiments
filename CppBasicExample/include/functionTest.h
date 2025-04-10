#ifndef __FUNCTIONTEST_H__
#define __FUNCTIONTEST_H__

#include "common.h"

int func1_default(int a, int b = 20, int c = 30){ //in C++, function can have default input vlaues
  return a + b + c;
}

void func2_overload(){ //in C++, function can overload
  cout<<"Call func2_overload()"<<endl;
}

void func2_overload(int a){
  cout<<"Call func2_overload(int a)"<<endl;
}

void func2_overload(double a){
  cout<<"Call func2_overload(double a)"<<endl;
}

void FunctionTest(){
  cout<<"=====Function Test====="<< endl;
  cout<<"func1_default(10) = "<<func1_default(10)<<endl;
  cout<<"func2_overload(): "<<endl;
  func2_overload();
  cout<<"func2_overload(10): "<<endl;
  func2_overload(10);
  cout<<"func2_overload(0.1): "<<endl;
  func2_overload(0.1);

  cout<<endl;
}


#endif
