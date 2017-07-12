#include <stdint.h>
#include <stddef.h>

extern uint8_t __start_text;
extern uint8_t __end_text;
extern uint8_t __start_rodata;
extern uint8_t __end_rodata;
extern uint8_t __start_data;
extern uint8_t __end_data;
extern uint8_t __start_tss;
extern uint8_t __end_tss;
extern uint8_t __start_bss;
extern uint8_t __end_bss;
extern uint8_t __start_heap;
extern uint8_t __end_heap;

#define SECTION_TEXT    1
#define SECTION_RODATA  2
#define SECTION_DATA    3
#define SECTION_TSS     4
#define SECTION_BSS     5
#define SECTION_HEAP    6

#define SECTION_START   1
#define SECTION_END     0

uint64_t getSectionAddress(int section, int pos);
