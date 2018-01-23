#include <test/test.h>
#include <test/ptrBlks0.h>

#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/ptrBlks.h>

#include <stdlib.h>

size_t nvmos_test_getTestCases(ptrBlks0, test0)(
    nvmos_test_testcase_t(ptrBlks0, test0) * **res)
{
#define testCaseCount 2

    static int32_t case0_pushSeq[12] =
        {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    static nvmos_test_testcase_t(ptrBlks0, test0) nvmos_test_testcase(ptrBlks0, test0, case0) =
        {
            .pushSeq = case0_pushSeq,
            .pushSeqLen = 12,
            .maxBlkCount = 4096};

    static int32_t case1_pushSeq[12] =
        {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};

    static nvmos_test_testcase_t(ptrBlks0, test0) nvmos_test_testcase(ptrBlks0, test0, case1) =
        {
            .pushSeq = case1_pushSeq,
            .pushSeqLen = 12,
            .maxBlkCount = 4096};

    static nvmos_test_testcase_t(ptrBlks0, test0) *
        nvmos_test_cases(ptrBlks0, test0)
            [testCaseCount] =
        {
            &nvmos_test_testcase(ptrBlks0, test0, case0),
            &nvmos_test_testcase(ptrBlks0, test0, case1)};

    *res = nvmos_test_cases(ptrBlks0, test0);

    return testCaseCount;
#undef testCaseCount
}

nvmos_test_testDriver(ptrBlks0, test0)
{
    nvmos_dl_allocator_t allocator;
    nvmos_dl_alloc_createAllocator(&allocator, availMem, availMemLen, 0x1000);

    ptrBlks_t ptrBlks;
    ptrBlks_construct(&ptrBlks, NULL, NULL, NULL, NULL);

    nvmos_ptr_t blkList[testCase->maxBlkCount];

    size_t pushSeqPos = 0;
    size_t blkCount = 0;

    for (; pushSeqPos < testCase->pushSeqLen; ++pushSeqPos)
    {
        if (blkCount + testCase->pushSeq[pushSeqPos] < 0 ||
            blkCount + testCase->pushSeq[pushSeqPos] > testCase->maxBlkCount)
        {
            nvmos_test_fail(
                "ptrBlks0",
                0,
                "Push block out of bound at push sequence %d",
                pushSeqPos);
            return -1;
        }
        if (testCase->pushSeq[pushSeqPos] > 0)
        {
            blkList[blkCount] =
                nvmos_dl_alloc_allocateBlocks(
                    &allocator,
                    testCase->pushSeq[pushSeqPos]);

            if (ptrBlks_pushBlks(
                    &ptrBlks,
                    blkList[blkCount],
                    testCase->pushSeq[pushSeqPos],
                    &allocator))
            {
                nvmos_test_fail(
                    "ptrBlks0",
                    0,
                    "Push block failed at push sequence %d",
                    pushSeqPos);
                return -1;
            }

            for (int i = 1; i < testCase->pushSeq[pushSeqPos]; ++i)
            {
                blkList[blkCount + i] = blkList[blkCount] + i * 0x1000;
            }
            blkCount += testCase->pushSeq[pushSeqPos];
        }
        else if (testCase->pushSeq[pushSeqPos] < 0)
        {
            if (ptrBlks_popBlks(
                    &ptrBlks,
                    abs(testCase->pushSeq[pushSeqPos]),
                    &allocator))
            {
                nvmos_test_fail(
                    "ptrBlks0",
                    0,
                    "Pop block failed at push sequence %d",
                    pushSeqPos);
                return -1;
            }

            blkCount += testCase->pushSeq[pushSeqPos];
        }
        else
        {
        }
    }
    for (size_t i = 0; i < blkCount; ++i)
    {
        nvmos_ptr_t blkPtr =
            (nvmos_ptr_t)ptrBlks_getDataBlkAt(&ptrBlks, i);
        if (blkPtr == NULL)
        {
            nvmos_test_fail(
                "ptrBlks0",
                0,
                "Data block is NULL at %d",
                i);
            return -1;
        }
        if (blkPtr != blkList[i])
        {
            nvmos_test_fail(
                "ptrBlks0",
                0,
                "Data block is inconsistent at %d",
                i);
            return -1;
        }
    }
    return 0;
}

nvmos_test_defTestRunner(ptrBlks0, test0)

    size_t nvmos_test_getTestCases(ptrBlks0, test1)(
        nvmos_test_testcase_t(ptrBlks0, test1) * **res)
{
#define testCaseCount 2

    static int32_t case0_popSeq[12] =
        {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    static nvmos_test_testcase_t(ptrBlks0, test1) nvmos_test_testcase(ptrBlks0, test1, case0) =
        {
            .popSeq = case0_popSeq,
            .popSeqLen = 12,
            .maxBlkCount = 4096};

    static int32_t case1_popSeq[12] =
        {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};

    static nvmos_test_testcase_t(ptrBlks0, test1) nvmos_test_testcase(ptrBlks0, test1, case1) =
        {
            .popSeq = case1_popSeq,
            .popSeqLen = 12,
            .maxBlkCount = 4096};

    static nvmos_test_testcase_t(ptrBlks0, test1) *
        nvmos_test_cases(ptrBlks0, test1)
            [testCaseCount] =
        {
            &nvmos_test_testcase(ptrBlks0, test1, case0),
            &nvmos_test_testcase(ptrBlks0, test1, case1)};

    *res = nvmos_test_cases(ptrBlks0, test1);

    return testCaseCount;
#undef testCaseCount
}

nvmos_test_testDriver(ptrBlks0, test1)
{
    nvmos_dl_allocator_t alloc;
    nvmos_dl_alloc_createAllocator(&alloc, availMem, availMemLen, 0x1000);

    ptrBlks_t ptrBlks;
    ptrBlks_construct(&ptrblks, NULL, NULL, NULL, NULL);

    nvmos_ptr_t pushBlksPtr =
        nvmos_dl_alloc_allocateBlocks(
            &alloc,
            testCase->maxBlkCount);

    if (pushBlksPtr == NULL)
    {
        nvmos_test_fail("ptrBlks0", 1, "Allocator failed at allocating push blocks...");
        return -1;
    }

    if (ptrBlks_pushBlks(&ptrBlks, pushBlksPtr, testCase->maxBlkCount, &alloc))
    {
        nvmos_test_fail("ptrBlks0", 1, "Failed at pushing blocks...");
        return -1;
    }

    size_t i = 0;
    size_t blkCount = testCase->maxBlkCount;
    for (; i < testCase->popSeqLen; ++i)
    {
        blkCount -= testCase->popSeq[i];
        if (ptrBlks_popBlks(&ptrBlks, testCase->popSeq[i], &alloc))
        {
            nvmos_test_fail(
                "ptrBlks0",
                1,
                "Failed at pushing blocks at %d...",
                i);
            return -1;
        }

        if (blkCount != ptrBlks_getSize(&ptrBlks))
        {
            nvmos_test_fail(
                "ptrBlks0",
                1,
                "Block size after poping not consistent at %d...",
                i);
            return -1;
        }

        for (size_t j = 0; j < blkCount; ++j)
        {
            if (ptrBlks_getDataBlkAt(&ptrBlks, i) !=
                pushBlksPtr + j * 0x1000)
            {
                nvmos_test_fail(
                    "ptrBlks0",
                    1,
                    "Block pointer after poping not consistent at i = %d j = %d...",
                    i,
                    j);
                return -1;
            }
        }
    }
    return 0;
}

nvmos_test_defTestRunner(ptrBlks0, test1)