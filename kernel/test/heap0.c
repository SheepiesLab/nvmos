/*

seq explained:
	Parameter  "seq"  is  the  sequence  of  heap memory
	allocation  or  deallocation.  "seq"  is an array of
	integer.  An allocation is represented by a positive
	integer   in   "seq".   Value   indicates   size  of
	allocation.  An  deallocation  is  represented  by a
	negtive integer. Value indicates index of allocation
	to be freed.

seq example:
8,8,8,8,-2,16

res explained:
	Parameter  "res"  is the desired resulting allocated
	location  of  the corresponding allocation in "seq".

n explained:
	Parameter  "n"  is the number of member in "seq" and 
	"res".

*/

#include <kernel/mman/heap/Heap.h>

int testcases_heap0(nvmos_pointer_t heapStart,nvmos_pointer_t heapEnd){
	const int TESTCASE_COUNT = 1;
	
	int *testCases_seq[TESTCASE_COUNT];
	int *testCases_res[TESTCASE_COUNT];
	int testCases_n[TESTCASE_COUNT];

	// Test Case 0
	{
		const int TESTCASE_ID = 0;
		testCases_n[TESTCASE_ID] = 2;
		int seq[testCases_n[TESTCASE_ID]] = {8, 8};
		int res[testCases_n[TESTCASE_ID]] = {0x8, 0x18};
		testCases_seq[TESTCASE_ID] = seq;
		testCases_res[TESTCASE_ID] = res;
	}

	// Run all test cases
	for(int i = 0;i < TESTCASE_COUNT; ++i){
		if (testDriver_heap0(
						testCases_seq[i],
						testCases_res[i],
						testCases_n[i],
						heapStart,
						heapEnd)){
			printf("Failed: Test Case %d\n", i);				
		}
		else{
			printf("Passed: Test Case %d\n", i);
		}
	}

	return 0;
}

int testDriver_heap0(
		int *seq, 
		nvmos_pointer_t *res, 
		int n, 
		nvmos_pointer_t heapStart,
		nvmos_pointer_t heapEnd){
	Heap heap;
	heap_construct(&heap, heapStart, heapEnd);
	
	nvmos_pointer_t allocs[n];

	for (int i = 0; i < n; ++i){
		if (seq[i]>0){
			allocs[i] = heap_malloc(&heap, seq[i]);
			if (allocs[i] != res[i]){
				return -1;
			}
			else{
				continue;
			}
		}
		else{
			if (allocs[-seq[i]] != NULL){
				heap_free(&heap, allocs[-seq[i]]);
				allocs[-seq[i]] = NULL;
				continue;
			}
			else{
				continue;
			}
		}
	}

	return 0;
}








