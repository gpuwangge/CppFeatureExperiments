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




#endif
