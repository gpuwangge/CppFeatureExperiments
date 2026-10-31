#include "main.h"
#include "tests.hpp"

// ============================================================
// PartIII: Main
// ============================================================
int main(){
	std::cout<<"===SPSC Ring Buffer Test begin==="<<std::endl;

    test_empty_full_fifo();
    test_capacity_one();
    test_slot_wraparound();
    test_counter_overflow();
    test_random_against_deque();

    std::cout << "Stage 1: all single-thread tests passed\n";

    test_spsc_threads();

    std::cout << "Stage 2: SPSC stress test passed\n";
    
	std::cout<<"===SPSC Ring Buffer Test end==="<<std::endl;
	return 0;
}

/* 运行结果

*/

/* 结果说明

*/

