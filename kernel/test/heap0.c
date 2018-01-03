#include <test/heap0.h>
#include <test/test.h>

nvmos_test_testDriver(heap0, test0)
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

nvmos_test_defTestRunner(heap0, test0)