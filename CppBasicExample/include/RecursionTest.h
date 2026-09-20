#ifndef __RECURSIONTEST_H__
#define __RECURSIONTEST_H__

#include "common.h"

void DoPermute(std::string in, std::string out, bool* used, int length, int level){
	if(level == length){
		for(int i = 0; i< out.length(); i++){
			std::cout<<out[i];
		}
		std::cout<<"; ";
		return;
	}

	for(int i = 0; i < length; i++){
		if(used[i]) continue;
		out.push_back(in[i]);
		used[i] = true;
		DoPermute(in, out, used, length, level+1);
		used[i] = false;
		out.resize(out.size() - 1);
	}
}

void PermutationOfString(std::string s){
	int length = s.length();
	bool *used = new bool[length];
	for(int i = 0; i < length; i++){
		used[i] = false;
	}
	std::string out;
	DoPermute(s, out, used, length, 0);
}


void DoCombine(std::string in, std::string out, int length, int start){
	for(int i = start; i < length; i++){
		out.push_back(in[i]);
		for(int j = 0; j < out.length(); j++)
			std::cout<<out[j];
		std::cout<<"; ";
		if(i < length - 1){
			DoCombine(in, out, length, i+1);

			out.resize(out.length() - 1);
		}
	}
}

void CombineOfString(std::string s){
	int length = s.length();
	std::string out;
	DoCombine(s, out, length, 0);
}


void RecursionTest(){
	std::cout << "=====Recursion Test=====" << std::endl;

	std::cout<<"Recursion Test1 (PermutationOfString)"<<std::endl;
	std::string s = "abcd";
	PermutationOfString(s);
	std::cout<<std::endl;

	std::cout<<"Recursion Test2: (CombineOfString)"<<std::endl;
	CombineOfString(s);
	std::cout<<std::endl;

	std::cout<<std::endl;
}

#endif

