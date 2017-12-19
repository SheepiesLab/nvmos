#include <test/heap0.h>
#include <test/test.h>

int nvmos_test_run_heap0_test0(
    nvmos_ptr_t availMem,
    size_t availMemLen,
    nvmos_test_case_heap0_test0_t *testCase[HEAP0_TEST0_TESTCASE_COUNT])
{
    char testName[] = "heap0_test0";
    for (size_t i = 0; i < HEAP0_TEST0_TESTCASE_COUNT; ++i)
    {
        int testResult =
            nvmos_test_driver_heap0_test0(
                availMem,
                availMemLen,
                testCase[i]);
        if (testResult)
        {
            nvmos_test_fail(
                testName,
                i,
                "Case failed at %d",
                testResult);
        }
        else
        {
            nvmos_test_success(testName, i);
        }
    }
}

int nvmos_test_driver_heap0_test0(
    nvmos_ptr_t availMem,
    size_t availMemLen,
    nvmos_test_case_heap0_test0_t *testCase)
{
#define CLEANUP_RETURN(x)             \
    memset(availMem, 0, availMemLen); \
    return (x);

    Heap heap;
    heap_construct(&heap, availMem, availMem + availMemLen);

    for (size_t i = 0; i < testCase->actionLen; ++i)
    {
        if (testCase->testSeq[i].action == MALLOC)
        {
            nvmos_ptr_t result =
                heap_malloc(
                    &heap,
                    testCase->testSeq[i].sizeOrLoc);
            if (result == NULL)
                CLEANUP_RETURN(-i);

            if (result !=
                testCase->testSeq[i].result + availMem)
                CLEANUP_RETURN(-i);
        }
        else
        {
            if (heap_free(
                    &heap,
                    testCase->testSeq[i].sizeOrLoc + availMem))
                CLEANUP_RETURN(-i);
        }
    }
    CLEANUP_RETURN(0);

#undef CLEANUP_RETURN
}