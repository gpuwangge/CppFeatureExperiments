#ifndef __FUNCTIONTEST_H__
#define __FUNCTIONTEST_H__

#include "common.h"

int func(int a, int b = 10, int c = 10){ //in C++, function can have default input vlaues
  return a + b + c;
}

#endif
