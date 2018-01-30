#include <test/test.h>
#include <test/directory0.h>

#include <kernel/datalayer/allocator/allocator.h>
#include <kernel/datalayer/directory.h>
#include <kernel/datalayer/meta.h>

#include <stdlib.h>
#include <string.h>

size_t nvmos_test_getTestCases(directory0, test0)(
    nvmos_test_testcase_t(ptrdirectory0Blks0, test0) * **res)
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
        /   ->  aaa     ->  dd      ->  ffff
                        ->  eeeee
            ->  bbbb    ->  gggg
            ->  ccccc   
    */

    meta_meta_t root, aaa, bbbb, ccccc, dd, eeeee, ffff, gggg;

    strcpy(root.fileName, "root");
    root.metaPtr = (uint32_t)&root;

    strcpy(aaa.fileName, "aaa");
    aaa.metaPtr = (uint32_t)&aaa;

    strcpy(bbbb.fileName, "bbbb");
    bbbb.metaPtr = (uint32_t)&bbbb;

    strcpy(ccccc.fileName, "ccccc");
    ccccc.metaPtr = (uint32_t)&ccccc;

    strcpy(dd.fileName, "dd");
    dd.metaPtr = (uint32_t)&dd;

    strcpy(eeeee.fileName, "eeeee");
    eeeee.metaPtr = (uint32_t)&eeeee;

    strcpy(ffff.fileName, "ffff");
    ffff.metaPtr = (uint32_t)&ffff;

    strcpy(gggg.fileName, "gggg");
    gggg.metaPtr = (uint32_t)&gggg;

    meta_setDir(&root);
    meta_setDir(&aaa);
    meta_setDir(&bbbb);
    meta_setDir(&ccccc);
    meta_setDir(&dd);
    meta_setDir(&eeeee);
    meta_setDir(&ffff);
    meta_setDir(&gggg);

    dir_addFileRef(&(root.metaContent.fileMeta), "aaa", &aaa, &allocator);
    dir_addFileRef(&(root.metaContent.fileMeta), "bbbb", &bbbb, &allocator);
    dir_addFileRef(&(root.metaContent.fileMeta), "ccccc", &ccccc, &allocator);

    dir_addFileRef(&(aaa.metaContent.fileMeta), "dd", &dd, &allocator);
    dir_addFileRef(&(aaa.metaContent.fileMeta), "eeeee", &eeeee, &allocator);
    
    dir_addFileRef(&(dd.metaContent.fileMeta), "ffff", &ffff, &allocator);

    dir_addFileRef(&(bbbb.metaContent.fileMeta), "gggg", &gggg, &allocator);
}

nvmos_test_defTestRunner(directory0, test0)