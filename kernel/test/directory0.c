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

    meta_setDir(&(dirs[0]));
    meta_setDir(&(dirs[1]));
    meta_setDir(&(dirs[2]));
    meta_setDir(&(dirs[3]));
    meta_setDir(&(dirs[4]));
    meta_setDir(&(dirs[5]));
    meta_setDir(&(dirs[6]));
    meta_setDir(&(dirs[7]));

    dir_addFileRef(&(dirs[0].metaContent.fileMeta), "1", &(dirs[1])), &allocator);
    dir_addFileRef(&(dirs[0].metaContent.fileMeta), "2", &(dirs[2])), &allocator);
    dir_addFileRef(&(dirs[0].metaContent.fileMeta), "3", &(dirs[3])), &allocator);
    
    dir_addFileRef(&(dirs[1].metaContent.fileMeta), "4", &(dirs[4])), &allocator);
    dir_addFileRef(&(dirs[1].metaContent.fileMeta), "5", &(dirs[5])), &allocator);
    
    dir_addFileRef(&(dirs[2].metaContent.fileMeta), "6", &(dirs[6])), &allocator);
    
    dir_addFileRef(&(dirs[4].metaContent.fileMeta), "7", &(dirs[7])), &allocator);

    return 0;
}

nvmos_test_defTestRunner(directory0, test0)