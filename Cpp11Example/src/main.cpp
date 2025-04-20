#include "../include/common.h"
#include <vector>

void PrintVector(vector<int> &v, string s){
	cout<<"&"<<s<<" = "<<&v<<": ";
	for(int i = 0; i < v.size(); i++) {
		cout<<v[i];
		if(i != v.size()-1) cout<<", ";
	}
	cout<<endl;
}

void RvalueReference_Test(){
	cout<<"=====Rvalue Reference Test (int)====="<<endl;
	int a = 42;
	int b = a;
	int &c = a;
	int &&d = std::move(a);
	int &&e = (int &&)a;
	cout<<"&a = "<<&a<<", a = "<<a<<endl;
	cout<<"&b = "<<&b<<", b = "<<b<<endl;
	cout<<"&c = "<<&c<<", c = "<<c<<", Lvalue Reference"<<endl;
	cout<<"&d = "<<&d<<", d = "<<d<<", Rvalue Reference(Move)"<<endl;
	cout<<"&e = "<<&e<<", e = "<<e<<", Rvalue Reference(Cast)"<<endl;
	cout<<endl;

	cout<<"=====Rvalue Reference Test (vector)====="<<endl;
	vector<int> vec_a = {11,22,33,44,55};
	vector<int> vec_b = vec_a;
	vector<int> &vec_c = vec_a;
	vector<int> &&vec_d = std::move(vec_a);
	vector<int> &&vec_e = (vector<int> &&)vec_a;
	PrintVector(vec_a, "vec_a");
	PrintVector(vec_b, "vec_b");
	PrintVector(vec_c, "vec_c");
	PrintVector(vec_d, "vec_d");
	PrintVector(vec_e, "vec_e");
	cout<<endl;
}


class MyString {
private:
	char* data;

public:
	MyString(const char* str = "") {
		std::cout << "Constructor\n";
		data = new char[strlen(str) + 1];
		strcpy(data, str);
	}

	MyString(const MyString& other) {
		std::cout << "Copy Constructor(Deep Copy)\n";
		data = new char[strlen(other.data) + 1];
		strcpy(data, other.data);
	}

	MyString(MyString&& other) noexcept { //noexcept tells compiler that this code will be throw exception, so it will run faster
		std::cout << "Move Constructor(Rvalue Reference)\n";
		data = other.data;
		other.data = nullptr; 
	}

	MyString& operator=(const MyString& other) {
		std::cout << "Copy Assignment Operator\n";
		if (this != &other) {
			delete[] data;
			data = new char[strlen(other.data) + 1];
			strcpy(data, other.data);
		}
		return *this;
	}

	MyString& operator=(MyString&& other) noexcept {
		std::cout << "Move Assignment Operator(Rvalue Reference)\n";
		if (this != &other) {
			delete[] data;
			data = other.data;
			other.data = nullptr;
		}
		return *this;
	}

	~MyString() {
		std::cout << "Destructor\n";
		delete[] data;
	}

	void print() const {
		std::cout << "Data: " << (data ? data : "null") << "\n";
	}
};
	
MyString ReturnTemporaryObject(){
	MyString myString("Temporary");
	//return myString; //will call constructor
	return std::move(myString); //will call constructor and move constructor, force return rvalue, berak NRVO
}

void RvalueReference_Class_Test(){
	cout<<"=====Rvalue Reference Test (Class): Move Constructor and Move Assignment Operator====="<<endl;

	MyString a("42");
	cout<<"a.print(): ";
	a.print(); // Result: 42

	MyString b = std::move(a); // Call Move Constructor
	//MyString b = a; //This will call Copy Constructor

	cout<<"a.print(): "; a.print(); // Result: null
	cout<<"b.print(): "; b.print(); // Result: 42

	MyString c;
	c = std::move(b); //Call Move Assighnment Operator
	//c = b; //This will call Copy Assignment Operator
	cout<<"a.print(): "; a.print(); // Result: null
	cout<<"b.print(): "; b.print(); // Result: null
	cout<<"c.print(): "; c.print(); // Result: 42
}

void RvalueReference_Return_Test(){
	cout<<"=====Rvalue Reference Test (Class)====="<<endl;
	MyString d = ReturnTemporaryObject();
	cout<<"d.print(): "; d.print(); // Result: Temporary
}

int main(){
	RvalueReference_Test();
	RvalueReference_Class_Test(); cout<<endl;
	RvalueReference_Return_Test();
	
	return 0;
}
