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

    ksects[KSECTION_SECTION_TEXT].section = KSECTION_SECTION_TEXT;
    ksects[KSECTION_SECTION_TEXT].addr = (uint64_t) &__start_text;
    ksects[KSECTION_SECTION_TEXT].len = (uint64_t) (&__end_text - &__start_text);

    ksects[KSECTION_SECTION_RODATA].section = KSECTION_SECTION_RODATA;
    ksects[KSECTION_SECTION_RODATA].addr = (uint64_t) &__start_rodata;
    ksects[KSECTION_SECTION_RODATA].len = (uint64_t) (&__end_rodata - &__start_rodata);

    ksects[KSECTION_SECTION_DATA].section = KSECTION_SECTION_DATA;
    ksects[KSECTION_SECTION_DATA].addr = (uint64_t) &__start_data;
    ksects[KSECTION_SECTION_DATA].len = (uint64_t) (&__end_data - &__start_data);

    ksects[KSECTION_SECTION_TSS].section = KSECTION_SECTION_TSS;
    ksects[KSECTION_SECTION_TSS].addr = (uint64_t) &__start_tss;
    ksects[KSECTION_SECTION_TSS].len = (uint64_t) (&__end_tss - &__start_tss);

    ksects[KSECTION_SECTION_BSS].section = KSECTION_SECTION_BSS;
    ksects[KSECTION_SECTION_BSS].addr = (uint64_t) &__start_bss;
    ksects[KSECTION_SECTION_BSS].len = (uint64_t) (&__end_bss - &__start_bss);

    ksects[KSECTION_SECTION_HEAP].section = KSECTION_SECTION_HEAP;
    ksects[KSECTION_SECTION_HEAP].addr = (uint64_t) &__start_heap;
    ksects[KSECTION_SECTION_HEAP].len = (uint64_t) (&__end_heap - &__start_heap);

    return ksects;
}