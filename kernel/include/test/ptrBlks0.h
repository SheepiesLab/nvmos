#ifndef TEST_PTRBLKS0_H_
#define TEST_PTRBLKS0_H_

typedef struct
{
    int32_t *pushSeq;
    size_t pushSeqLen;
    size_t maxBlkCount;
} nvmos_test_testcase_t(ptrBlks0, test0);

nvmos_test_defTestcaseCount(ptrBlks0, test0, 1)

static int32_t case0_pushSeq[12] =
    {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

static nvmos_test_testcase_t(ptrBlks0, test0) case0 =
    {
        .pushSeq = case0_pushSeq,
        .pushSeqLen = 12,
        .maxBlkCount = 4096};

static nvmos_test_testcase_t(ptrBlks0, test0) *
    nvmos_test_cases(ptrBlks0, test0)
        [nvmos_test_testcaseCount(ptrBlks0, test0)] =
    {&case0};

nvmos_test_testDriver(ptrBlks0, test0);

nvmos_test_testRunner(ptrBlks0, test0);

#endif