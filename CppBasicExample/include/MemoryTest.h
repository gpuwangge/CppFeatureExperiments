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
		cout << "CTest";
	}
	virtual void vprint() {
		cout << "CTest: virtual print";
	}
};

class CDerivedClass : CTest {
public:
	int f;
	CDerivedClass() {

	}
	void print() {
		cout << "CDerivedClass";
	}
	virtual void vprint() {
		cout << "CDerivedClass: virtual print";
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
		cout << "Memory same!" << endl;
	else 
		cout << "Memory different!" << endl;
	
	for (int i = 0; i < 10; i++) 
		data0[i] = 10-i;
	if (0 == memcmp((void *)data0, (void *)data1, 10 * sizeof(int)))
		cout << "Memory same!" << endl;
	else
		cout << "Memory different!" << endl;

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
	cout << "d1(memcpy): " << d1 << endl;
	cout << "d2(memmove): " << d2 << endl;


	int x[10], y[10];
	for (int i = 0; i < 10; i++) x[i] = i;
	memcpy(y, x, 10 * sizeof(int));
	for (int i = 0; i < 10; i++) cout << y[i] << ", ";
	cout << endl;
}


void TestFunc() { //how to change a value outside of this function?
	int z = 30;
	int w = 40;
	cout << "addr of z(in TestFunc): " << (int*)&z << ", value of z: " << z << endl;
	cout << "addr of w(in TestFunc): " << (int*)&w << ", value of w: " << w << endl;

	int *p = &z;
	cout << "addr of p(in TestFunc): " << (int*)p << endl;
	p = p + 0x39;//p increase 0xe4, because p is 4 byte, actually move e4/4=0x39???????need debug
	*p = 50;//change p's value to 50
}

void MemoryTest() {
	cout << "=====Memory Test=====" << endl;
	cout << "Print object of class size:"<<endl;
	CTest baseObj;
	cout << "addr of baseObj:"<< (int*)&baseObj << endl;
	cout << "addr of baseObj.a:" << (int*)&baseObj.a << endl;
	cout << "addr of baseObj.b:" << (int*)&baseObj.b << endl; //int* is a 4-bytes pointer, because addr is 4 bytes in Win32
	cout << "addr of baseObj.c:" << (int*)&baseObj.c << endl;
	cout << "addr of baseObj.d:" << (int*)&baseObj.d << endl;
	cout << "addr of baseObj.e:" << (int*)&baseObj.e << endl;
	cout << "size of baseObj: " << sizeof(baseObj) << endl;//32: 4+4+8+8+8(vtable), non-virtual member variable is in static area, will not take up object size

	CDerivedClass derivedObj;
	cout << "size of derivedObj: " << sizeof(derivedObj) << endl;//derived object size = base object size + derived data size: 40: 32(base)+8
	cout<<endl;

	cout << "Memory address test" << endl;
	int x = 10;
	int y = 20;
	cout << "addr of x: " << (int*)&x << ", value of x: " << x << endl;
	cout << "addr of y: " << (int*)&y << ", value of y: " << y << endl;
	cout << "addr of TestFunc: " << (int*)&TestFunc << endl;
	TestFunc();
	cout << "addr of y: " << (int*)&y << ", value of y: " << y << endl;
	cout << endl;

	cout << "Memory Compare Test: " << endl;
	CompareMemory();
	cout << endl;

	cout << "Memory Copy Test: " << endl;
	CopyMemory();

	cout<<endl;
}

#endif

