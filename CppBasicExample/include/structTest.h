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

void StructTest(){
	cout<<"=====Struct Test====="<<endl;

	cout<<"<Struct Test 1: 4 methods to create struct objects>"<< endl;
	//Method #1 to create struct
	Student s1;
	s1.name = "Alex";
	s1.age = 15;
	s1.score = 99;
	
	//Method #2 to create struct
	Student s2 = {"Bob", 18, 100};

	//Method #3 to create struct when defining Student
	s3.name = "Catherine";
	s3.age = 30;
	s3.score = 60;

	//Method #4 to create struct (array of struct)
	Student studentArray[3] {
		{"Daniel", 35, 99},
		{"Eddie", 22, 66},
		{"Fiona", 32, 85}
	};

	cout<<"Print student result on the screen!"<< endl;
	cout<<"s1 Name:"<<s1.name<<", Age="<<s1.age<<", Score="<<s1.score<<endl;
	cout<<"s2 Name:"<<s2.name<<", Age="<<s2.age<<", Score="<<s2.score<<endl;
	cout<<"s3 Name:"<<s3.name<<", Age="<<s3.age<<", Score="<<s3.score<<endl;
	for(int i = 0; i < 3; i++) cout<<"studentArray["<<i<<"] Name:"<<studentArray[i].name<<", Age="<<studentArray[i].age<<", Score="<<studentArray[i].score<<endl;
	cout<<endl;

	
	cout<<"<Struct Test 2: Use pointer to visit struct object>"<< endl;
	Student *p = &s1;
	cout<<"p Name:"<<p->name<<", Age="<<p->age<<", Score="<<p->score<<endl<<endl;

	
	cout<<"<Struct Test 3: A struct(teacher) contains another struct(student)>"<< endl;
	Teacher t;
	t.id = 10000;
	t.name = "Garrett";
	t.age = 50;
	t.stu.name = "Hannah";
	t.stu.age = 20;
	t.stu.score = 60;

	cout<<"Print teacher result on the screen!"<< endl;
	cout<<"t Name:"<<t.name<<", Age="<<t.age<<", Id="<<t.id<<endl;
	cout<<"t.stu Name:"<<t.stu.name<<", Age="<<t.stu.age<<", Score="<<t.stu.score<<endl<<endl;

	
	cout<<"<Struct Test 4: Pass struct object as parameter>"<< endl;
	//pass struct by value
	PrintStudent_Value(s1);
	cout<<"s1(in main) Name:"<<s1.name<<", Age="<<s1.age<<", Score="<<s1.score<<endl;

	//pass struct by address
	PrintStudent_Address(&s2);
	cout<<"s2(in main) Name:"<<s2.name<<", Age="<<s2.age<<", Score="<<s2.score<<endl;
	
	//pass struct by reference
	PrintStudent_Reference(s3);
	cout<<"s3(in main) Name:"<<s3.name<<", Age="<<s3.age<<", Score="<<s3.score<<endl<<endl;
	
	
	cout<<"<Struct Test 5: Use const in struct>"<< endl;
	PrintStudent_Address_Const(&s1);
	cout<<"s1(in main) Name:"<<s1.name<<", Age="<<s1.age<<", Score="<<s1.score<<endl;

	cout<<endl;
}

#endif
