#include <test/heap0.h>

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
        if (testCase->actions[i] == MALLOC)
        {
            nvmos_ptr_t result =
                heap_malloc(&heap, testCase->sizeOrLoc[i]);
            if (result == NULL)
                CLEANUP_RETURN(-i);

            if (result != testCase->results[i])
                CLEANUP_RETURN(-i);
        }
        else
        {
            if (heap_free(&heap, testCase->sizeOrLoc[i]))
                CLEANUP_RETURN(-i);
        }
    }
    CLEANUP_RETURN(0);

#undef CLEANUP_RETURN
}