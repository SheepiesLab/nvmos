#include <kernel/sectionAddresses.h>

uint64_t getSectionAddress(int section, int pos){
  if (pos == SECTION_START){
    switch(section){
      case SECTION_TEXT:
        return (uint64_t)&__start_text;
      case SECTION_RODATA:
        return (uint64_t)&__start_rodata;
      case SECTION_DATA:
        return (uint64_t)&__start_data;
      case SECTION_BSS:
        return (uint64_t)&__start_bss;
      default:
        return NULL;
    }
  }
  else if (pos == SECTION_END){
    switch(section){
      case SECTION_TEXT:
        return (uint64_t)&__end_text;
      case SECTION_RODATA:
        return (uint64_t)&__end_rodata;
      case SECTION_DATA:
        return (uint64_t)&__end_data;
      case SECTION_BSS:
        return (uint64_t)&__end_bss;
      default:
        return NULL;
    }
  }
  else return NULL;
}
