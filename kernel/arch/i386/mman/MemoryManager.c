//
// Created by benng on 7/17/17.
//

#include <kernel/mman/MemoryManager.h>

int mman_construct(MemoryManager *mman, multiboot_info_t *mbt) {
    mman->mbt = mbt;
}

size_t mman_getMemoryMapLength(MemoryManager *mman) {
    if ((mman->mbt->flags & MULTIBOOT_INFO_MEM_MAP) != MULTIBOOT_INFO_MEM_MAP)
        return -1;
    return mman->mbt->mmap_length / sizeof(MemoryMap);
}

int mman_getMemoryMap(MemoryManager *mman, MemoryMap *_mmap, size_t max) {
    if ((mman->mbt->flags & MULTIBOOT_INFO_MEM_MAP) != MULTIBOOT_INFO_MEM_MAP)
        return -1;

    multiboot_memory_map_t *mmap = mman->mbt->mmap_addr;

    size_t i = 0;
    while (mmap < mman->mbt->mmap_addr + mman->mbt->mmap_length && i < max) {
        _mmap[i].len = mmap->len;
        _mmap[i].addr = mmap->addr;
        _mmap[i].type = mmap->type;
        ++i;
        mmap = (multiboot_memory_map_t *)((unsigned int) mmap + mmap->size + sizeof(mmap->size));
    }

    return i;
}