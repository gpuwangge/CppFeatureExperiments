#include "../include/common.h"
#include "../include/BitTest.h"
#include "../include/GeneralDataStructureTest.h"
#include "../include/HashTableTest.h"
#include "../include/LinkedListTest.h"
#include "../include/MemoryTest.h"
#include "../include/MultiThreadsTest.h"

int main(){
	BitOperationTest();
	GeneralDataStructureTest();
	HashTableTest();
	LinkedListTest();
	MemoryTest();
	//MultiThreadsTest();

	return 0;
}
