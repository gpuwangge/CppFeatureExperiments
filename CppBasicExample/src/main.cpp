// #include "../include/infra.h"
#include "../include/objA.h"
#include "../include/objB.h"
// #include "../include/axi.h"
// #include "../include/port.h"

#include <iostream>
//#include <stdio.h>
#include <string.h>
#include <memory>
using namespace std;

int main(){
	/*
	CInfra infra;
	infra.print();
	infra.funcA();
	infra.funcB();
	infra.funcC();

	axi::CAXI axi;
	axi.print();

	CPort port;
	port.print();
	*/

	int a = 10;
	int *p_int = &a;
	int **p_p_int = &p_int;

	cout<<"Variable"<< endl;
	printf("a = %i\n", a);
	//cout<<"&a = " << &a << endl;
	printf("&a = %p\n", &a);
	cout<<"sizeof(a) = " << sizeof(a) << endl;
	cout<<"typeid(a).name() = " << typeid(a).name() << endl;
	
	cout<< endl;

	cout<<"Pointer"<< endl;
	cout<<"*p_int = " << *p_int <<endl;
	//cout<<"p_int = " << p_int <<endl;
	printf("p_int = %p\n", p_int);
	cout<<"sizeof(p_int) = " << sizeof(p_int) << endl;
	cout<<"typeid(p_int).name() = " << typeid(p_int).name() << endl;

	cout<< endl;

	cout<<"Pointer points to pointer"<< endl;
	cout<<"**p_p_int = " << **p_p_int <<endl;
	//cout<<"*p_p_int = " << *p_p_int <<endl;
	printf("*p_p_int = %p\n", *p_p_int);
	printf("p_p_int = %p\n", p_p_int);
	cout<<"sizeof(p_p_int) = " << sizeof(p_p_int) << endl;
	cout<<"typeid(p_p_int).name() = " << typeid(p_p_int).name() << endl;

	cout<<endl;

	cout<<"Array"<< endl;
	int array[5] = {1,2,3,4,5};
	cout<<"array[i] = ";
	for(int i = 0; i < 5; i++) cout<<array[i]<<" ";
	cout<<endl;
	cout<<"&array[i] = ";
	for(int i = 0; i < 5; i++) printf("%p ", &array[i]);
	cout<<endl;
	cout<<"sizeof(array) = " << sizeof(array) << endl;
	cout<<"typeid(array).name() = " << typeid(array).name() << endl;

	cout<<endl;

	cout<<"Buffer"<< endl;
	char *buffer = new char[8];
	memset(buffer, 127, 8);
	cout<<"buffer[i] = ";
	for(int i = 0; i < 8; i++) cout<<(int)buffer[i]<<" ";
	cout<<endl;
	cout<<"&buffer[i] = ";
	for(int i = 0; i < 8; i++) printf("%p ", &buffer[i]);
	cout<<endl;
	cout<<"sizeof(buffer) = " << sizeof(buffer) << endl;
	cout<<"typeid(buffer).name() = " << typeid(buffer).name() << endl;
	delete[] buffer;

	cout<<endl;

	//cout<<"Difference of variable and pointer"<< endl;
	//int address_a = &a;//not allawed


	cout<<"Smart pointer(shared_ptr)"<< endl;
	shared_ptr<CObjA> p_objA = make_shared<CObjA>();
	p_objA->print();

	cout<<endl;

	cout<<"Smart pointer(unique_ptr)"<< endl;
	auto ptrB1 = make_unique<CObjB>();
	ptrB1->print();
	cout<<"unique_ptr can not duplicate, but can move"<< endl;
	auto ptrB2 = move(ptrB1);
	ptrB2->print();
	ptrB1->print(); //?ptrB1 still works, suggest even after move, ptrB1 still points to objectB
	
	cout<<"Try to reset ptrB2..."<<endl;
	ptrB2.reset(); //can not reset ptrB1, take no effect
	cout<<"reset ptrB2 completed"<<endl;
	ptrB2->print(); //?after reset, still can print

	cout<<endl;

    return 0;
}