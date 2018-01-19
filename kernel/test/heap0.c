#include <test/heap0.h>
#include <test/test.h>

nvmos_test_testDriver(heap0, test0)
{
#define CLEANUP_RETURN(x)             \
    memset(availMem, 0, availMemLen); \
    return (x);

#define testName "heap0_test0"
#define testCaseNum 0

    Heap heap;
    heap_bt
    construct(&heap, availMem, availMem + availMemLen);

    for (size_t i = 0; i < testCase->actionLen; ++i)
    {
        if (testCase->testSeq[i].action == MALLOC)
        {
            nvmos_ptr_t result =
                heap_malloc(
                    &heap,
                    testCase->testSeq[i].sizeOrLoc);
            if (result == NULL)
            {
                nvmos_test_fail(
                    testName,
                    testCaseNum,
                    "Case failed at %d",
                    i);
                CLEANUP_RETURN(-i);
            }

            if (result !=
                testCase->testSeq[i].result + availMem)
            {
                nvmos_test_fail(
                    testName,
                    testCaseNum,
                    "Case failed at %d",
                    i);
                CLEANUP_RETURN(-i);
            }
        }
        else
        {
            if (heap_free(
                    &heap,
                    testCase->testSeq[i].sizeOrLoc + availMem))
            {
                nvmos_test_fail(
                    testName,
                    testCaseNum,
                    "Case failed at %d",
                    i);
                CLEANUP_RETURN(-i);
            }
        }
    }
    CLEANUP_RETURN(0);

#undef CLEANUP_RETURN
#undef testName
#undef testCaseNum
}

nvmos_test_defTestRunner(heap0, test0)