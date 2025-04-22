#ifndef __GENERALDATASTRUCTURETEST_H__
#define __GENERALDATASTRUCTURETEST_H__

#include "common.h"
//#include <hash_map>
#include <unordered_map>
#include <list>
#include <string>
#include <vector>

/*
-Since there was no hash table defined in the C++ standard library, 
different implementors of the standard libraries would provide a 
non-standard hash table often named hash_map. Because these 
implementations were not written following a standard they all 
had subtle differences in functionality and performance guarantees.

-Starting with C++11 a hash table implementation has been added 
to the C++ standard library standard. It was decided to use an 
alternate name for the class to prevent collisions with these 
non-standard implementations and to prevent inadvertent use of 
the new class by developers who had hash_table in their code.

-The chosen alternate name is unordered_map which really is 
more descriptive as it hints at the class's map interface and the 
unordered nature of its elements.
*/

/*List and Hash Table*/
void PrintHashTable(unordered_map<string, int> &hashMap){
	cout<<"Print Hash Map: ";
	for (const auto& pair : hashMap) cout<<"("<<pair.first<<":"<<pair.second << ") ";
	cout<<endl;
	cout<<"Size is:"<<hashMap.size()<<endl;
	cout<<endl;
}

void BuildHashMap(){
	cout<<"Build Hash Map"<<endl;
	unordered_map<string, int> hashMap;
	list<string> lst;
	for(int i = 0; i < 20; i++){
		//cout<<i<<" ";
		char* c= new char;
		itoa(i,c,10);
		string s = c;
		lst.push_back(s);
		hashMap.insert(make_pair(s, i));
	}
	PrintHashTable(hashMap);

	cout<<"Add an element: hashMap[\"test\"] = 10000"<<endl;
	hashMap["test"] = 10000;
	PrintHashTable(hashMap);

	cout<<"Remove the following elements(key): ";
	for(list<string>::iterator iter = lst.begin(); iter != lst.end(); ++iter){
		cout<<hashMap[*iter]<<" ";
		hashMap.erase(*iter);
	}
	cout<<endl;
	PrintHashTable(hashMap);
}

/*Vector*/
/*Each double allocation need O(n), but happen so few so it is still O(1)*/
/*Compare to list, no 'insert' in vector*/
void BuildVector(){
	cout<<"Build Vector"<<endl;

	vector<int> v;
	for(int i = 0;i < 30; i++){
		cout<<i<<" ";
		v.push_back(i);
	}

	cout<<endl;

	cout<<"Size is: "<<v.size()<<endl;

	cout<<"Pop back 10 elements from the vector"<<endl;
	for(int i = 0;i < 10; i++) v.pop_back();
	for(int i = 0;i < v.size(); i++) cout<<v[i]<<" ";
	cout<<"Size is: "<<v.size()<<endl;
	cout<<endl;

}

void StringBuffer(){
	cout<<"StringBuffer: NA"<<endl;

	//cout<<endl;
}


void GeneralDataStructureTest(){
	cout<<"=====General Data Structure Test====="<<endl;

	BuildHashMap();
	BuildVector();
	StringBuffer();

	cout<<endl;
}

#endif