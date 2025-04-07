#include "../include/main.h"

struct Student{
	string name;
	int age;
	int score;
}s3;


int main(){
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
	struct Student studentArray[3] {
		{"Daniel", 35, 99},
		{"Eddie", 22, 66},
		{"Fiona, 32, 85"}
	}

	
	cout<<"Print result on the screen!"<< endl;
	cout<<"Name:"<<s1.name<<", Age="<<s1.age<<", Score="<<s1.score<<endl;
	cout<<"Name:"<<s2.name<<", Age="<<s2.age<<", Score="<<s2.score<<endl;
	cout<<"Name:"<<s3.name<<", Age="<<s3.age<<", Score="<<s3.score<<endl;
	for(int i = 0; i < 3; i++) cout<<"Name:"<<studentArray[i].name<<", Age="<<studentArray[i].age<<", Score="<<studentArray[i].score<<endl;
	
	cout<<endl;
	return 0;
}
