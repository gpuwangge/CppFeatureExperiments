#include<iostream>
#include<vector> 
#include<string.h>
using namespace std;

class CTest {
public:
	int a;
	char b;
	char c;//虽然char是1 byte，但因为内存对齐,b,c合用4 bytes内存
	double d;
	double e;
	CTest() {

	}
	void print() {//一般成员函数不会对类的大小造成影响。
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
	//有两个函数memcpy和memmove
	//当src和dst区域没有重叠时，两个函数是完全一样的。
	//没有重叠的条件是：dst <= src || (char *)dst >= ((char *)src + count
	//重叠的时候，memcpy是不能正常工作的。因为memcpy不需要判断重叠，所以它运行速度比memmove快

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


void TestFunc() {
	int z = 30;
	int w = 40;
	cout << "addr of z(in TestFunc): " << (int*)&z << ", value of z: " << z << endl;//y和z之间地址差了0xe4(228)
	cout << "addr of w(in TestFunc): " << (int*)&w << ", value of w: " << w << endl;

	int *p = &z;
	cout << "addr of p(in TestFunc): " << (int*)p << endl;
	p = p + 0x39;//p需要增加0xe4,因为p是4字节，所以实际移动e4/4=0x39
	cout << "addr of p(in TestFunc): " << (int*)p << endl;
	*p = 50;//成功把y的值变成了50
}

int main() {
	cout << "类的大小测试：" << endl;
	CTest baseObj;
	cout << "addr of baseObj:"<< (int*)&baseObj << endl;
	cout << "addr of baseObj.a:" << (int*)&baseObj.a << endl;
	cout << "addr of baseObj.b:" << (int*)&baseObj.b << endl; //int* is a 4-bytes pointer, because addr is 4 bytes in Win32
	cout << "addr of baseObj.c:" << (int*)&baseObj.c << endl;
	cout << "addr of baseObj.d:" << (int*)&baseObj.d << endl;
	cout << "addr of baseObj.e:" << (int*)&baseObj.e << endl;
	cout << "size of baseObj: " << sizeof(baseObj) << endl;//32: 4+4+8+8+8(vtable),非virtual成员函数放在静态区，不占用类的大小

	CDerivedClass derivedObj;
	cout << "size of derivedObj: " << sizeof(derivedObj) << endl;//类对象大小=基类对象大小+自身成员大小: 40: 32(base)+8
	
	cout << "变量的内存地址测试：" << endl;
	int x = 10;
	int y = 20;
	cout << "addr of x: " << (int*)&x << ", value of x: " << x << endl;
	cout << "addr of y: " << (int*)&y << ", value of y: " << y << endl;
	cout << "addr of TestFunc: " << (int*)&TestFunc << endl;
	TestFunc();
	cout << "addr of y: " << (int*)&y << ", value of y: " << y << endl;
	cout << endl;

	cout << "比较大量变量测试: " << endl;
	CompareMemory();
	cout << endl;

	cout << "拷贝变量测试: " << endl;
	CopyMemory();

	getchar();
}