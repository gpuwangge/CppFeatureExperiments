#ifndef __MEMORYTEST_H__
#define __MEMORYTEST_H__

#include "common.h"

class CTest {
public:
	int a;
	char b;
	char c;// although char is 1 byte, because of memory alignment, b and c use 4 bytes together
	double d;
	double e;
	CTest() {

	}
	void print() {//member function will not change class size (member function and member variables are stored separately)
		std::cout << "CTest";
	}
	virtual void vprint() {
		std::cout << "CTest: virtual print";
	}
};

class CDerivedClass : CTest {
public:
	int f;
	CDerivedClass() {

	}
	void print() {
		std::cout << "CDerivedClass";
	}
	virtual void vprint() {
		std::cout << "CDerivedClass: virtual print";
	}
};

void CompareMemory() {
	int *data0 = new int[10];
	int *data1 = new int[10];
	for (int i = 0; i < 10; i++) {
		data0[i] = i;
		data1[i] = i;
	}
	if (0 == memcmp((void *)data0, (void *)data1, 10 * sizeof(int))) 
		std::cout << "Memory same!" << std::endl;
	else 
		std::cout << "Memory different!" << std::endl;
	
	for (int i = 0; i < 10; i++) 
		data0[i] = 10-i;
	if (0 == memcmp((void *)data0, (void *)data1, 10 * sizeof(int)))
		std::cout << "Memory same!" << std::endl;
	else
		std::cout << "Memory different!" << std::endl;

	delete data0;
	delete data1;
}

void CopyMemory() {
	//difference of memcpy and memmove
	//if src and dst have no overlap, they are the same
	//no overlap condition: dst <= src || (char *)dst >= ((char *)src + count
	//if overlap, memcpy can not function normally. however, memcpy runs faster than memmove(becaue memcpy dont need to check overlap)

	const char *s = "source string";
	char d1[20], d2[20];
	memcpy(d1, s, strlen(s));
	memmove(d2, s, strlen(s));
	d1[strlen(s)] = '\0';
	d2[strlen(s)] = '\0';
	std::cout << "d1(memcpy): " << d1 << std::endl;
	std::cout << "d2(memmove): " << d2 << std::endl;


	int x[10], y[10];
	for (int i = 0; i < 10; i++) x[i] = i;
	memcpy(y, x, 10 * sizeof(int));
	for (int i = 0; i < 10; i++) std::cout << y[i] << ", ";
	std::cout << std::endl;
}


void TestFunc() { //how to change a value outside of this function?
	int z = 30;
	int w = 40;
	std::cout << "addr of z(in TestFunc): " << (int*)&z << ", value of z: " << z << std::endl;
	std::cout << "addr of w(in TestFunc): " << (int*)&w << ", value of w: " << w << std::endl;

	int *p = &z;
	std::cout << "addr of p(in TestFunc): " << (int*)p << std::endl;
	p = p + 0x39;//p increase 0xe4, because p is 4 byte, actually move e4/4=0x39???????need debug
	*p = 50;//change p's value to 50
}

void MemoryTest() {
	std::cout << "=====Memory Test=====" << std::endl;
	std::cout << "Print object of class size:"<<std::endl;
	CTest baseObj;
	std::cout << "addr of baseObj:"<< (int*)&baseObj << std::endl;
	std::cout << "addr of baseObj.a:" << (int*)&baseObj.a << std::endl;
	std::cout << "addr of baseObj.b:" << (int*)&baseObj.b << std::endl; //int* is a 4-bytes pointer, because addr is 4 bytes in Win32
	std::cout << "addr of baseObj.c:" << (int*)&baseObj.c << std::endl;
	std::cout << "addr of baseObj.d:" << (int*)&baseObj.d << std::endl;
	std::cout << "addr of baseObj.e:" << (int*)&baseObj.e << std::endl;
	std::cout << "size of baseObj: " << sizeof(baseObj) << std::endl;//32: 4+4+8+8+8(vtable), non-virtual member variable is in static area, will not take up object size

	CDerivedClass derivedObj;
	std::cout << "size of derivedObj: " << sizeof(derivedObj) << std::endl;//derived object size = base object size + derived data size: 40: 32(base)+8
	std::cout<<std::endl;

	std::cout << "Memory address test" << std::endl;
	int x = 10;
	int y = 20;
	std::cout << "addr of x: " << (int*)&x << ", value of x: " << x << std::endl;
	std::cout << "addr of y: " << (int*)&y << ", value of y: " << y << std::endl;
	std::cout << "addr of TestFunc: " << (int*)&TestFunc << std::endl;
	TestFunc();
	std::cout << "addr of y: " << (int*)&y << ", value of y: " << y << std::endl;
	std::cout << std::endl;

	std::cout << "Memory Compare Test: " << std::endl;
	CompareMemory();
	std::cout << std::endl;

	std::cout << "Memory Copy Test: " << std::endl;
	CopyMemory();

	std::cout<<std::endl;
}

#endif

