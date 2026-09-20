#ifndef __STRINGTEST_H__
#define __STRINGTEST_H__

#include "common.h"

//can use string1 += string2
//string.append(&x)


void CStyleString(){
	std::cout<<"CStyleString"<<std::endl;

	std::cout<<"Length and Index Test:"<<std::endl;
	char str[] = "123";
	std::cout<<"strlen: "<<strlen(str)<<std::endl;;
	for(int i = 0; i< strlen(str);i++){
		std::cout<<i<<":"<<str[i]<<std::endl; 
	}

	std::cout<<"Compare Test"<<std::endl;
	char *str2 = new char[10];
	str2[0] = '3';
	str2[1] = '\0';
	for(int i = 0; i< strlen(str2);i++){
		std::cout<<i<<":"<<str2[i]<<std::endl; 
	}
	std::cout<<"Compare: "<<strcmp(str, str2)<<std::endl;//non zero is not equal


	std::cout<<"Copy Test:"<<std::endl;
	strcpy(str2, str);
	for(int i = 0; i< strlen(str2);i++){
		std::cout<<i<<":"<<str2[i]<<std::endl; 
	}
	std::cout<<"Compare: "<<strcmp(str, str2)<<std::endl;// zero means equal

	
}

void CPlusStyleString(){
	std::cout<<"CPlusStyleString"<<std::endl;

	std::cout<<"Length and Index Test:"<<std::endl;
	std::string str;
	std::cout<<str.empty()<<std::endl;
	str = "sdfawe";
	std::cout<<str.empty()<<std::endl;
	str = "xv33";
	std::cout<<"strlen: "<<str.size()<<std::endl;;
	for(int i = 0; i< str.size();i++){
		std::cout<<i<<":"<<str[i]<<std::endl;
	}


	std::cout<<"Compare Test:"<<std::endl;
	std::string str2;
	str2 = "fawe";
	for(int i = 0; i< str2.size();i++){
		std::cout<<i<<":"<<str2[i]<<std::endl;
	}
	std::cout<<"Compare: "<<str.compare(str2)<<std::endl;


	std::cout<<"Copy Test:"<<std::endl;
	str2 = str;
	for(int i = 0; i< str2.size();i++){
		std::cout<<i<<":"<<str2[i]<<std::endl;
	}
	std::cout<<"Compare: "<<str.compare(str2)<<std::endl;// zero means equal
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


void Reverse(const char *s, char *output){//buggy?
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

void StringTest(){
	std::cout << "=====String Test=====" << std::endl;

	CStyleString();
	std::cout<<std::endl;
	CPlusStyleString();
	std::cout<<std::endl;

	std::cout<<"My strcpy"<<std::endl;
	char source[] = "Input";
	char *destination;
	destination = new char[strlen(source)];
	MyStrcpy(source, destination);
	for(int i = 0; i < strlen(destination); i++){
		std::cout<<destination[i];
	}
	std::cout<<std::endl;

	std::cout<<"My strcmp"<<std::endl;
	std::cout<<MyStrcmp(source,destination)<<std::endl;
	char diff[] = "Another";
	std::cout<<MyStrcmp(source,diff)<<std::endl;
	std::cout<<std::endl;

	std::cout<<"My strlen"<<std::endl;
	for(int j = 0; j < strlen(diff); j++)
			std::cout<<diff[j];
	std::cout<<MyStrLen(diff)<<std::endl;



	const char *text = "A good hash function and implementation algorit\
		hm are essential for good hash table performance but may be difficult to achieve\
		A basic requirement is that the function should provide a \
		uniform distribution of hash values A nonuniform distribution incre\
		ases the number of collisions and the cost of resolving them \
		Uniformity is sometimes difficult to ensure by design but may\
		be evaluated empirically using statistical tests eg a Pearsons \
		chisquared test for discrete uniform distributions";
	std::cout<<"Reverse Test"<<std::endl;
	for(int i =0; i< strlen(text); i++){
		std::cout<<text[i];
	}
	std::cout<<std::endl;
	char* output = new char[strlen(text)];
	Reverse(text, output);
	for(int i =0; i< strlen(output); i++){
		std::cout<<output[i];
	}
	std::cout<<std::endl;

	std::cout<<std::endl;
}

#endif