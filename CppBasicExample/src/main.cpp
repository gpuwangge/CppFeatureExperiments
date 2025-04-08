#include "../include/common.h"
#include "../include/structTest.h"
#include "../include/functionTest.h"
#include "../include/classTest.h"
#include "../include/templateTest.h"

int main(){
	cout<<"Practise 1: 4 methods to create struct objects"<< endl;
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

	
	cout<<"Practise 2: Use pointer to visit struct object"<< endl;
	Student *p = &s1;
	cout<<"p Name:"<<p->name<<", Age="<<p->age<<", Score="<<p->score<<endl<<endl;


	
	cout<<"Practise 3: A struct(teacher) contains another struct(student)"<< endl;
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


	
	cout<<"Practise 4: Pass struct object as parameter"<< endl;
	//pass struct by value
	PrintStudent_Value(s1);
	cout<<"s1(in main) Name:"<<s1.name<<", Age="<<s1.age<<", Score="<<s1.score<<endl;

	//pass struct by address
	PrintStudent_Address(&s2);
	cout<<"s2(in main) Name:"<<s2.name<<", Age="<<s2.age<<", Score="<<s2.score<<endl;
	
	//pass struct by reference
	PrintStudent_Reference(s3);
	cout<<"s3(in main) Name:"<<s3.name<<", Age="<<s3.age<<", Score="<<s3.score<<endl<<endl;


	
	cout<<"Practise 5: Use const in struct"<< endl;
	PrintStudent_Address_Const(&s1);
	cout<<"s1(in main) Name:"<<s1.name<<", Age="<<s1.age<<", Score="<<s1.score<<endl<<endl;

	
	cout<<"Practise 6: Function tests"<< endl;
	cout<<"func1_default(10) = "<<func1_default(10)<<endl;
	cout<<"func2_overload() = "<<func2_overload()<<endl;
	cout<<"func2_overload(10) = "<<func2_overload(10)<<endl;
	
	return 0;
}
