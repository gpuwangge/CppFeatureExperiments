#ifndef __STRUCTTEST_H__
#define __STRUCTTEST_H__

#include "common.h"

struct Student{
	string name;
	int age;
	int score;
}s3;

struct Teacher{ //a struct contains another struct
	int id;
	string name;
	int age;
	Student stu;

};

void PrintStudent_Value(Student s){ //pass struct by value
	s.name = "Hacked";
	cout<<"s(in function) Name:"<<s.name<<", Age="<<s.age<<", Score="<<s.score<<endl;
}

void PrintStudent_Address(Student *s){ //pass struct by address
	s->name = "Hacked";
	cout<<"s(in function) Name:"<<s->name<<", Age="<<s->age<<", Score="<<s->score<<endl;
}

void PrintStudent_Reference(Student &s){ //pass struct by reference
	s.name = "Hacked";
	cout<<"s(in function) Name:"<<s.name<<", Age="<<s.age<<", Score="<<s.score<<endl;
}

void PrintStudent_Address_Const(const Student *s){ //pass struct by address
	//s->name = "Hacked"; //will trigger error
	cout<<"s(in function) Name:"<<s->name<<", Age="<<s->age<<", Score="<<s->score<<endl;
}


#endif
