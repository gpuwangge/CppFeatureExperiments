#include "main.h"
#include "tests.hpp"

// ============================================================
// PartIII: Main
// ============================================================
int main(){
	std::cout<<"===Peek Queue Test begin==="<<std::endl;

	testEmptyQueue();
    testCapacityAndFIFO();
    testPeekDoesNotModifyQueue();
    testInvalidCapacity();
    testMoveOnlyType();

    // 重复运行，增加覆盖不同线程交错的机会。
    for (int i = 0; i < 10; ++i) {
        testMPMC();
    }
	
	std::cout<<"===Peek Queue Test end==="<<std::endl;
	return 0;
}

/* 运行结果

*/

/* 结果说明

*/

