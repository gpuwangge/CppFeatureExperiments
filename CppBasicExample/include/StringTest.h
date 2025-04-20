#include <iostream>
using namespace std;

//can use string1 += string2
//string.append(&x)


void CStyleString(){
	cout<<"CStyleString"<<endl;

	cout<<"Length and Index Test:"<<endl;
	char* str = "123";
	cout<<"strlen: "<<strlen(str)<<endl;;
	for(int i = 0; i< strlen(str);i++){
		cout<<i<<":"<<str[i]<<endl; 
	}

	cout<<"Compare Test"<<endl;
	char *str2 = new char[10];
	str2[0] = '3';
	str2[1] = '\0';
	for(int i = 0; i< strlen(str2);i++){
		cout<<i<<":"<<str2[i]<<endl; 
	}
	cout<<"Compare: "<<strcmp(str, str2)<<endl;//non zero is not equal


	cout<<"Copy Test:"<<endl;
	strcpy(str2, str);
	for(int i = 0; i< strlen(str2);i++){
		cout<<i<<":"<<str2[i]<<endl; 
	}
	cout<<"Compare: "<<strcmp(str, str2)<<endl;// zero means equal

	
}

void CPlusStyleString(){
	cout<<"CPlusStyleString"<<endl;

	cout<<"Length and Index Test:"<<endl;
	string str;
	cout<<str.empty()<<endl;
	str = "sdfawe";
	cout<<str.empty()<<endl;
	str = "xv33";
	cout<<"strlen: "<<str.size()<<endl;;
	for(int i = 0; i< str.size();i++){
		cout<<i<<":"<<str[i]<<endl;
	}


	cout<<"Compare Test:"<<endl;
	string str2;
	str2 = "fawe";
	for(int i = 0; i< str2.size();i++){
		cout<<i<<":"<<str2[i]<<endl;
	}
	cout<<"Compare: "<<str.compare(str2)<<endl;


	cout<<"Copy Test:"<<endl;
	str2 = str;
	for(int i = 0; i< str2.size();i++){
		cout<<i<<":"<<str2[i]<<endl;
	}
	cout<<"Compare: "<<str.compare(str2)<<endl;// zero means equal
}


//====================
void MyStrcpy(char* s, char* d){
	int len = strlen(s);
	while(*d++ = *s++);
}

bool MyStrcmp(char* x, char *y){
	while(*x){
		if(*x != *y) return false;
		*x++;
		*y++;
	}
	return true;
}

int MyStrLen(char *x){
	int count = 0;
	while(*x++){
		count++;
	}
	return count;
}
//===================


void Reverse(char *s, char *output){//buggy?
	int len = strlen(s);
	int bufferIndex = 0;
	int outputIndex = 0;
	char *buffer = new char[len];
	for(int i = len-1; i >=0; i--){
		buffer[bufferIndex++]=s[i];
		if(i == 0){
			for(int j = bufferIndex-1; j >=0; j--){
				output[outputIndex++] = buffer[j];
			}
		}
		if(s[i] == ' '){
			for(int j = bufferIndex-2; j >=0; j--){
				output[outputIndex++] = buffer[j];
			}
			bufferIndex = 0;
			output[outputIndex++] = ' ';
		}
		
	}
}

void main(){
	CStyleString();
	cout<<endl;
	CPlusStyleString();
	cout<<endl;

	cout<<"My strcpy"<<endl;
	char *source = "Input";
	char *destination;
	destination = new char[strlen(source)];
	MyStrcpy(source, destination);
	for(int i = 0; i < strlen(destination); i++){
		cout<<destination[i];
	}
	cout<<endl;

	cout<<"My strcmp"<<endl;
	cout<<MyStrcmp(source,destination)<<endl;
	char *diff = "Another";
	cout<<MyStrcmp(source,diff)<<endl;
	cout<<endl;

	cout<<"My strlen"<<endl;
	for(int j = 0; j < strlen(diff); j++)
			cout<<diff[j];
	cout<<MyStrLen(diff)<<endl;



	char *text = "A good hash function and implementation algorit\
		hm are essential for good hash table performance but may be difficult to achieve\
		A basic requirement is that the function should provide a \
		uniform distribution of hash values A nonuniform distribution incre\
		ases the number of collisions and the cost of resolving them \
		Uniformity is sometimes difficult to ensure by design but may\
		be evaluated empirically using statistical tests eg a Pearsons \
		chisquared test for discrete uniform distributions";
	cout<<"Reverse Test"<<endl;
	for(int i =0; i< strlen(text); i++){
		cout<<text[i];
	}
	cout<<endl;
	char* output = new char[strlen(text)];
	Reverse(text, output);
	for(int i =0; i< strlen(output); i++){
		cout<<output[i];
	}
	cout<<endl;

	getchar();
	return;

}