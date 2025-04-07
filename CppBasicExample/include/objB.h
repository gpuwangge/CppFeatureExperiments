#ifndef __OBJB_H__
#define __OBJB_H__

#include <iostream>

class CObjB
{
public:
	void print();
public:
	CObjB(){
		std::cout << "ObjB Constructor" << std::endl;
	}
	~CObjB(){
		std::cout << "ObjB Destructor" << std::endl;
	}
};

#endif