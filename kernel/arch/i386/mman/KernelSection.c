//
// Created by benng on 7/17/17.
//

#include <kernel/mman/KernelSection.h>

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

KernelSection *ksection_getKsections() {
    static KernelSection ksects[KSECTION_COUNT + 1];

    ksects[0].section = KSECTION_SECTION_TEXT;
    ksects[0].addr = (uint64_t) &__start_text;
    ksects[0].len = (uint64_t) (&__end_text - &__start_text);

    ksects[1].section = KSECTION_SECTION_RODATA;
    ksects[1].addr = (uint64_t) &__start_rodata;
    ksects[1].len = (uint64_t) (&__end_rodata - &__start_rodata);

    ksects[2].section = KSECTION_SECTION_DATA;
    ksects[2].addr = (uint64_t) &__start_data;
    ksects[2].len = (uint64_t) (&__end_data - &__start_data);

    ksects[3].section = KSECTION_SECTION_TSS;
    ksects[3].addr = (uint64_t) &__start_tss;
    ksects[3].len = (uint64_t) (&__end_tss - &__start_tss);

    ksects[4].section = KSECTION_SECTION_BSS;
    ksects[4].addr = (uint64_t) &__start_bss;
    ksects[4].len = (uint64_t) (&__end_bss - &__start_bss);

    ksects[5].section = KSECTION_SECTION_HEAP;
    ksects[5].addr = (uint64_t) &__start_heap;
    ksects[5].len = (uint64_t) (&__end_heap - &__start_heap);

    return ksects;
}