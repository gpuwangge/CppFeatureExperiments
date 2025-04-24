#include "../include/common.h"
#include "../include/BitTest.h"
#include "../include/GeneralDataStructureTest.h"
#include "../include/HashTableTest.h"
#include "../include/LinkedListTest.h"
#include "../include/MemoryTest.h"
#include "../include/MultiThreadsTest.h"
#include "../include/RecursionTest.h"
#include "../include/SortingTest.h"
#include "../include/StackTest.h"
#include "../include/StringTest.h"
#include "../include/TreeTest.h"

int main(){
	BitOperationTest();
	GeneralDataStructureTest();
	HashTableTest();
	LinkedListTest();
	MemoryTest();
	//MultiThreadsTest();
	RecursionTest();
	SortingTest();
	StackTest();
	StringTest();
	TreeTest();

	return 0;
}
