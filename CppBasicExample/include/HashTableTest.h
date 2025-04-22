
#ifndef __HASHTABLETEST_H__
#define __HASHTABLETEST_H__

#include "common.h"


/*Users/MrJoke/Desktop/Monster/Dropbox/JobHunt/JobTester/HashTable/HashTableTest.cpp
/*Hash Talbe: 1.Inorder to save space, the data structure should be dynamic*/
/*To be specific, a dynamic array*/
/*2.And it should still satisfy the O(1) insert, delete and query*/
/*"Therefore, arrays are a better choice for long strings 
with a limited set of possible character values;
hash tables are more efficient for shorter 
strings or when there are many possible character values."*/
/*1 is simple to satisfy, the problem is 2. how can we find a element in dynamic 
data structure using O(1)*/
/*if satisfy 2 first, use consecutive mem space, all operation is o(1) but space is o(n)*/

/*AdVANVED TECH*/
/* Establish a fixed array(size n), each element is a linklist (assume average size m)
So space is O(n), index is O(m)
Its good for a large scope of input (like all unicorn in an article), you dont have to 
set a space for each possible input
*/

struct DynamicHashElem{
	int key;
	int count;
	DynamicHashElem(int _key, int _count){
		key = _key;
		count = _count;
	}
};

class DynamicHashTable{
public:
	list<DynamicHashElem> *elem;
	char *key;
	DynamicHashTable(){
		elem = new list<DynamicHashElem>[13];//ASCII test: n = 13, m = 10
		key = new char[13];
	}

	~DynamicHashTable(){
		delete []elem;
		delete []key;
	}

	int GetIndex(int x){
		return x/10;
	}

	void AddPair(char _key){
		int index = GetIndex(_key);
		for(list<DynamicHashElem>::iterator iter = elem[index].begin(); 
			iter != elem[index].end();
			iter++){
			if((*iter).key == _key){
				(*iter).count++;
				return;
			}
		}
		elem[index].push_back(DynamicHashElem(_key, 1));
	}

	void PrintAll(string s){
		cout<<"Print All: "<<s<<endl;
		for(int i = 0; i < 13; i++){
			for(list<DynamicHashElem>::iterator iter = elem[i].begin(); 
				iter != elem[i].end();
				iter++){
					cout<<"("<<(char)((*iter).key)<<","<<(*iter).count<<") ";
			}
		}
		cout<<endl;
	}
};

class HashTable{
public:
	int *value;
	char *key;
	HashTable(){
		value = new int[52];
		key = new char[52];
		for(int i = 0; i < 52; i++){
			key[i] = ' ';
			value[i] = 0;
		}
	}

	~HashTable(){
		delete []key;
		delete []value;
	}

	int GetIndex(int x){
		return x - 'A';
	}

	void AddPair(char _key){
		int index = GetIndex(_key);
		if(index >=0 &&index <52){
			key[index] = _key;
			value[index]++;
		}
		
	}

	void DeletePair(char _key){
		int index = GetIndex(_key);
		if(index >=0 &&index <52){
			value[index]--;
		}
	}

	void PrintAll(string s){
		cout<<"Print All: "<<s<<endl;
		for(int i = 0; i < 52; i++){
			cout<<"("<<key[i]<<","<<value[i]<<"); ";
		}
		cout<<endl;
	}

};


void HashTableTest(){
	cout<<"=====Hash Table Test====="<<endl;

	HashTable hashTable;
	hashTable.PrintAll("(Empty)");

	const char *text1 = "A good hash function and implementation algorit\
hm are essential for good hash table performance but may be difficult to achieve\
A basic requirement is that the function should provide a \
uniform distribution of hash values A nonuniform distribution incre\
ases the number of collisions and the cost of resolving them \
Uniformity is sometimes difficult to ensure by design but may\
be evaluated empirically using statistical tests eg a Pearsons \
chisquared test for discrete uniform distributions";

	for(int i = 0; i< strlen(text1); i++){
		if(text1[i] != ' ')
			hashTable.AddPair(text1[i]);
	}
	hashTable.PrintAll("(Add text1)");
	
	const char *text2 = "A good hash function and implementation algorit\
hm are essential for good hash table performan";

	for(int i = 0; i< strlen(text2); i++){
		if(text2[i] != ' ')
			hashTable.DeletePair(text2[i]);
	}
	hashTable.PrintAll("(Remove text2)");
	cout<<endl;

	DynamicHashTable dHashTable;
	for(int i = 0; i< strlen(text2); i++){
		if(text2[i] != ' ')
			dHashTable.AddPair(text2[i]);
	}
	dHashTable.PrintAll("(Dynamic Hash Table: Add text2)");
	cout<<endl;
}


#endif