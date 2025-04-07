#include "../include/main.h"

struct Student{
	string name;
	int age;
	int score;
};


int main(){
	//Method #1 to create struct
	struct Student s1;
	s1.name = "Robin";
	s1.age = 15;
	s1.score = 99;
	
	//Method #2 to create struct
	struct Student s2 = {"Connor", 18, 100};

	
	//cout<<"hello world!"<< endl;
	cout<<s1.name<<", "<<s1.age<<", "<<s1.score<<endl;
	cout<<s2.name<<", "<<s2.age<<", "<<s2.score<<endl;
	
	cout<<endl;
	return 0;
}
