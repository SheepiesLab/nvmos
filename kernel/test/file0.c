#include <test/test.h>
#include <test/file0.h>

#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/file.h>

#include <stdio.h>

size_t nvmos_test_getTestCases(file0, test0)(
    nvmos_test_testcase_t(file0, test0) * **res)
{
    return 1;
}

nvmos_test_testDriver(file0, test0)
{
    nvmos_dl_allocator_t alloc;
    nvmos_dl_alloc_createAllocator(&alloc, availMem, availMemLen, 0x1000);

    file_meta_t fileMeta;
    fileMeta.refCount = 0;
    fileMeta.fileSize = 0;
    fileMeta.blkSize = 0;
    fileMeta._1stBlk = NULL;
    fileMeta._1stPtrBlk = NULL;
    fileMeta._2ndPtrBlk = NULL;
    fileMeta._3rdPtrBlk = NULL;

    for (size_t i = 0; i < 2500; ++i)
    {
        if (file_append(&fileMeta, "abcdefgh", 8, &alloc) != 8)
        {
            nvmos_test_fail("file0", 0, "file_append not satisfied at %d", i);
            return -1;
        }
    }
    printf("Write finished\n");

    for (size_t i = 0; i < 2500; ++i)
    {
        char buf[8];
        size_t ret = file_read(&fileMeta, buf, i * 8, 8);
        if (ret != 8)
        {
            printf("Extra Info: file_read return %d\n", ret);
            nvmos_test_fail("file0", 0, "file_read not satisfied at %d", i);
            return -1;
        }
        char res[] = "abcdefgh";
        if (memcmp(buf, res, 8) != 0)
        {
            nvmos_test_fail("file0", 0, "file_read corrupted data at %d", i);
            return -1;
        }
    }
    return 0;
}

nvmos_test_defTestRunner(file0, test0)