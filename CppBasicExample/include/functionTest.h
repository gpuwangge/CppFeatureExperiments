#ifndef __FUNCTIONTEST_H__
#define __FUNCTIONTEST_H__

#include "common.h"

int func1_default(int a, int b = 10, int c = 10){ //in C++, function can have default input vlaues
  return a + b + c;
}

int func2_overload(){ //in C++, function can overload
  cout<<"Call func2_overload()"<<endl;
}

int func2_overload(int a){
  cout<<"Call func2_overload(int a)"<<endl;
}

#endif
