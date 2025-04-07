#ifndef __OBJA_H__
#define __OBJA_H__

#include <iostream>

class CObjA
{
public:
	void print();
public:
	CObjA(){
		std::cout << "ObjA Constructor" << std::endl;
	}
	~CObjA(){
		std::cout << "ObjA Destructor" << std::endl;
	}
};

#endif