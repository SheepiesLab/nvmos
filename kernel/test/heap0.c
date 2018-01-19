#include <test/heap0.h>
#include <test/test.h>

size_t nvmos_test_getTestCases(heap0, test0)(nvmos_test_testcase_t(heap0, test0) * *res)
{
    static testSeqUnit_t case0_testSeq[] =
        {{MALLOC, 1, 0x8},
         {MALLOC, 2, 0x18},
         {MALLOC, 3, 0x30},
         {MALLOC, 4, 0x50},
         {FREE, 0x18, 0},
         {FREE, 0x30, 0},
         {MALLOC, 3, 0x18},
         {MALLOC, 2, 0x38},
         {FREE, 0x8, 0},
         {MALLOC, 1, 0x8}};

    static nvmos_test_testcase_t(heap0, test0) nvmos_test_testcase(heap0, test0, case0) =
        {
            10,
            case0_testSeq};

    static nvmos_test_testcase_t(heap0, test0) *
        nvmos_test_cases(heap0, test0)
            [nvmos_test_testcaseCount(heap0, test0)] =
        {&nvmos_test_testcase(heap0, test0, case0)};

    *res = nvmos_test_cases(heap0, test0);
    return 1;
}

nvmos_test_testDriver(heap0, test0)
{
#define CLEANUP_RETURN(x)             \
    memset(availMem, 0, availMemLen); \
    return (x);

#define testName "heap0_test0"
#define testCaseNum 0

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