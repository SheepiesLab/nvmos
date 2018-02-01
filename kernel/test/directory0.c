#include <test/test.h>
#include <test/directory0.h>

#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/directory.h>
#include <kernel/datalayer/meta.h>

#include <stdlib.h>
#include <string.h>

size_t nvmos_test_getTestCases(directory0, test0)(
    nvmos_test_testcase_t(directory0, test0) * **res)
{
#define testCaseCount 1

    return 1;
#undef testCaseCount
}

nvmos_test_testDriver(directory0, test0)
{
    nvmos_dl_allocator_t allocator;
    nvmos_dl_alloc_createAllocator(&allocator, availMem, availMemLen, 0x1000);

    /*
        0   ->  1   ->  4   ->  7
                    ->  5
            ->  2   ->  6
            ->  3   
    */

    meta_meta_t dirs[8];

    for (int i = 0; i < 8; ++i)
    {
        meta_setDir(&(dirs[i]));
        dirs[i].metaContent.fileMeta.refCount = 0;
        dirs[i].metaContent.fileMeta.fileSize = 0;
        dirs[i].metaContent.fileMeta.blkSize = 0;
        dirs[i].metaContent.fileMeta._1stBlk = NULL;
        dirs[i].metaContent.fileMeta._1stPtrBlk = NULL;
        dirs[i].metaContent.fileMeta._2ndPtrBlk = NULL;
        dirs[i].metaContent.fileMeta._3rdPtrBlk = NULL;
    }

    dir_addFileRef(&(dirs[0].metaContent.fileMeta), "1", &(dirs[1]), &allocator);
    dir_addFileRef(&(dirs[0].metaContent.fileMeta), "2", &(dirs[2]), &allocator);
    dir_addFileRef(&(dirs[0].metaContent.fileMeta), "3", &(dirs[3]), &allocator);

    dir_addFileRef(&(dirs[1].metaContent.fileMeta), "4", &(dirs[4]), &allocator);
    dir_addFileRef(&(dirs[1].metaContent.fileMeta), "5", &(dirs[5]), &allocator);

    dir_addFileRef(&(dirs[2].metaContent.fileMeta), "6", &(dirs[6]), &allocator);

    dir_addFileRef(&(dirs[4].metaContent.fileMeta), "7", &(dirs[7]), &allocator);

    if (!dir_fileNameUsed(&(dirs[0].metaContent.fileMeta), "1"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 1 not detected", 0);
    }
    if (!dir_fileNameUsed(&(dirs[0].metaContent.fileMeta), "2"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 2 not detected", 0);
    }
    if (!dir_fileNameUsed(&(dirs[0].metaContent.fileMeta), "3"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 3 not detected", 0);
    }
    if (dir_fileNameUsed(&(dirs[0].metaContent.fileMeta), "4"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 4 detected", 0);
    }

    if (!dir_fileNameUsed(&(dirs[1].metaContent.fileMeta), "4"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 4 not detected", 0);
    }
    if (!dir_fileNameUsed(&(dirs[1].metaContent.fileMeta), "5"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 5 not detected", 0);
    }
    if (dir_fileNameUsed(&(dirs[1].metaContent.fileMeta), "6"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 6 detected", 0);
    }

    if (!dir_fileNameUsed(&(dirs[2].metaContent.fileMeta), "6"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 6 not detected", 0);
    }
    if (dir_fileNameUsed(&(dirs[2].metaContent.fileMeta), "7"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 7 detected", 0);
    }

    if (!dir_fileNameUsed(&(dirs[4].metaContent.fileMeta), "7"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 7 not detected", 0);
    }
    if (dir_fileNameUsed(&(dirs[4].metaContent.fileMeta), "8"))
    {
        nvmos_test_fail("directory0", 0, "Directory Name 8 detected", 0);
    }

    dir_fileRef_t *fref01 = dir_getFileRefById(
        &(dirs[0].metaContent.fileMeta),
        dir_searchFileRef(&(dirs[0].metaContent.fileMeta), "1"));
    if (fref01 == NULL)
    {
        nvmos_test_fail("directory0", 0, "Directory Search 0->1 failed", 0);
    }
    else
    {
        meta_meta_t *meta01 = (meta_meta_t *)(fref01->metaPtr);
        if (meta01 != &(dirs[1]))
        {
            nvmos_test_fail("directory0", 0, "Directory Search 0->1 is not consistent", 0);
        }
    }

    dir_fileRef_t *fref02 = dir_getFileRefById(
        &(dirs[0].metaContent.fileMeta),
        dir_searchFileRef(&(dirs[0].metaContent.fileMeta), "2"));
    if (fref02 == NULL)
    {
        nvmos_test_fail("directory0", 0, "Directory Search 0->2 failed", 0);
    }
    else
    {
        meta_meta_t *meta02 = (meta_meta_t *)(fref02->metaPtr);
        if (meta02 != &(dirs[2]))
        {
            nvmos_test_fail("directory0", 0, "Directory Search 0->2 is not consistent", 0);
        }
    }

    dir_fileRef_t *fref03 = dir_getFileRefById(
        &(dirs[0].metaContent.fileMeta),
        dir_searchFileRef(&(dirs[0].metaContent.fileMeta), "3"));
    if (fref03 == NULL)
    {
        nvmos_test_fail("directory0", 0, "Directory Search 0->3 failed", 0);
    }
    else
    {
        meta_meta_t *meta03 = (meta_meta_t *)(fref03->metaPtr);
        if (meta03 != &(dirs[3]))
        {
            nvmos_test_fail("directory0", 0, "Directory Search 0->3 is not consistent", 0);
        }
    }

    return 0;
}

nvmos_test_defTestRunner(directory0, test0)