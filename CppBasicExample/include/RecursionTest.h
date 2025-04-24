
#ifndef __RECURSIONTEST_H__
#define __RECURSIONTEST_H__

#include "common.h"

void DoPermute(string in, string out, bool* used, int length, int level){
	if(level == length){
		for(int i = 0; i< out.length(); i++){
			cout<<out[i];
		}
		cout<<"; ";
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

void PermutationOfString(string s){
	int length = s.length();
	bool *used = new bool[length];
	for(int i = 0; i < length; i++){
		used[i] = false;
	}
	string out;
	DoPermute(s, out, used, length, 0);
}


void DoCombine(string in, string out, int length, int start){
	for(int i = start; i < length; i++){
		out.push_back(in[i]);
		for(int j = 0; j < out.length(); j++)
			cout<<out[j];
		cout<<"; ";
		if(i < length - 1){
			DoCombine(in, out, length, i+1);

			out.resize(out.length() - 1);
		}
	}
}

void CombineOfString(string s){
	int length = s.length();
	string out;
	DoCombine(s, out, length, 0);
}


void RecursionTest(){
	cout << "=====Recursion Test=====" << endl;

	cout<<"Recursion Test1 (PermutationOfString)"<<endl;
	string s = "abcd";
	PermutationOfString(s);
	cout<<endl;

	cout<<"Recursion Test2: (CombineOfString)"<<endl;
	CombineOfString(s);
	cout<<endl;

	cout<<endl;
}

#endif

