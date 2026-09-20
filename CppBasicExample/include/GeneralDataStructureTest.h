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
void PrintHashTable(std::unordered_map<std::string, int> &hashMap){
	std::cout<<"Print Hash Map: ";
	for (const auto& pair : hashMap) std::cout<<"("<<pair.first<<":"<<pair.second << ") ";
	std::cout<<std::endl;
	std::cout<<"Size is:"<<hashMap.size()<<std::endl;
	std::cout<<std::endl;
}

void BuildHashMap(){
	std::cout<<"Build Hash Map"<<std::endl;
	std::unordered_map<std::string, int> hashMap;
	std::list<std::string> lst;
	for(int i = 0; i < 20; i++){
		//cout<<i<<" ";
		char* c= new char;
		itoa(i,c,10);
		std::string s = c;
		lst.push_back(s);
		hashMap.insert(std::make_pair(s, i));
	}
	PrintHashTable(hashMap);

	std::cout<<"Add an element: hashMap[\"test\"] = 10000"<<std::endl;
	hashMap["test"] = 10000;
	PrintHashTable(hashMap);

	std::cout<<"Remove the following elements(key): ";
	for(std::list<std::string>::iterator iter = lst.begin(); iter != lst.end(); ++iter){
		std::cout<<hashMap[*iter]<<" ";
		hashMap.erase(*iter);
	}
	std::cout<<std::endl;
	PrintHashTable(hashMap);
}

/*Vector*/
/*Each double allocation need O(n), but happen so few so it is still O(1)*/
/*Compare to list, no 'insert' in vector*/
void BuildVector(){
	std::cout<<"Build Vector"<<std::endl;

	std::vector<int> v;
	for(int i = 0;i < 30; i++){
		std::cout<<i<<" ";
		v.push_back(i);
	}

	std::cout<<std::endl;

	std::cout<<"Size is: "<<v.size()<<std::endl;

	std::cout<<"Pop back 10 elements from the vector"<<std::endl;
	for(int i = 0;i < 10; i++) v.pop_back();
	for(int i = 0;i < v.size(); i++) std::cout<<v[i]<<" ";
	std::cout<<"Size is: "<<v.size()<<std::endl;
	std::cout<<std::endl;

}

void StringBuffer(){
	std::cout<<"StringBuffer: NA"<<std::endl;

	//cout<<endl;
}


void GeneralDataStructureTest(){
	std::cout<<"=====General Data Structure Test====="<<std::endl;

	BuildHashMap();
	BuildVector();
	StringBuffer();

	std::cout<<std::endl;
}

#endif