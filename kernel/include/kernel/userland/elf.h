#ifndef _NVMOS_ELF_H_
#define _NVMOS_ELF_H_
#include <stdint.h>
#include <stdbool.h>

#include <kernel/kdef.h>
#include <stdio.h>

// Reference: https://wiki.osdev.org/ELF_Tutorial
// ELF Specs: https://docs.oracle.com/cd/E23824_01/html/819-0690/glcfv.html#scrolltoc

typedef uint16_t Elf32_Half; // Unsigned half int
typedef uint32_t Elf32_Off;  // Unsigned offset
typedef uint32_t Elf32_Addr; // Unsigned address
typedef uint32_t Elf32_Word; // Unsigned int
typedef int32_t Elf32_Sword; // Signed int

#define ERROR(...) printf(__VA_ARGS__);

// ELF Header

#define ELF_NIDENT 16
#define ELF_RELOC_ERROR -1

typedef struct
{
	uint8_t e_ident[ELF_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr;

enum Elf_Ident
{
	EI_MAG0 = 0,	   // 0x7F
	EI_MAG1 = 1,	   // 'E'
	EI_MAG2 = 2,	   // 'L'
	EI_MAG3 = 3,	   // 'F'
	EI_CLASS = 4,	  // Architecture (32/64)
	EI_DATA = 5,	   // Byte Order
	EI_VERSION = 6,	// ELF Version
	EI_OSABI = 7,	  // OS Specific
	EI_ABIVERSION = 8, // OS Specific
	EI_PAD = 9		   // Padding
};

#define ELFMAG0 0x7F // e_ident[EI_MAG0]
#define ELFMAG1 'E'  // e_ident[EI_MAG1]
#define ELFMAG2 'L'  // e_ident[EI_MAG2]
#define ELFMAG3 'F'  // e_ident[EI_MAG3]

#define ELFDATA2LSB (1) // Little Endian
#define ELFCLASS32 (1)  // 32-bit Architecture

enum Elf_Type
{
	ET_NONE = 0, // Unkown Type
	ET_REL = 1,  // Relocatable File
	ET_EXEC = 2  // Executable File
};

#define EM_386 (3)	 // x86 Machine Type
#define EV_CURRENT (1) // ELF Current Version

bool elf_check_file(Elf32_Ehdr *hdr);

bool elf_check_supported(Elf32_Ehdr *hdr);

// static inline void *elf_load_rel(Elf32_Ehdr *hdr);

// void *elf_load_file(void *file);

// ELF Section Header

typedef struct
{
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf32_Shdr;

#define SHN_UNDEF (0x00) // Undefined/Not present

enum ShT_Types
{
	SHT_NULL = 0,	 // Null section
	SHT_PROGBITS = 1, // Program information
	SHT_SYMTAB = 2,   // Symbol table
	SHT_STRTAB = 3,   // String table
	SHT_RELA = 4,	 // Relocation (w/ addend)
	SHT_NOBITS = 8,   // Not present in file
	SHT_REL = 9,	  // Relocation (no addend)
};

enum ShT_Attributes
{
	SHF_WRITE = 0x01, // Writable section
	SHF_ALLOC = 0x02  // Exists in memory
};

// static inline Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr);

// static inline Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx);

// static inline char *elf_str_table(Elf32_Ehdr *hdr);

// static inline char *elf_lookup_string(Elf32_Ehdr *hdr, int offset);

// ELF Symbol Table

typedef struct
{
	Elf32_Word st_name;
	Elf32_Addr st_value;
	Elf32_Word st_size;
	uint8_t st_info;
	uint8_t st_other;
	Elf32_Half st_shndx;
} Elf32_Sym;

#define ELF32_ST_BIND(INFO) ((INFO) >> 4)
#define ELF32_ST_TYPE(INFO) ((INFO)&0x0F)

enum StT_Bindings
{
	STB_LOCAL = 0,  // Local scope
	STB_GLOBAL = 1, // Global scope
	STB_WEAK = 2	// Weak, (ie. __attribute__((weak)))
};

enum StT_Types
{
	STT_NOTYPE = 0, // No type
	STT_OBJECT = 1, // Variables, arrays, etc.
	STT_FUNC = 2	// Methods or functions
};

// static int elf_get_symval(Elf32_Ehdr *hdr, int table, uint idx);

// ELF Program Header

typedef struct
{
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

enum PT_Types
{
	PT_NULL = 0,
	PT_LOAD = 1,
	PT_DYNAMIC = 2,
	PT_INTERP = 3,
	PT_NOTE = 4,
	PT_SHLIB = 5,
	PT_PHDR = 6,
	PT_TLS = 7,
	PT_LOOS = 0x60000000,
	PT_SUNW_UNWIND = 0x6464e550,
	PT_SUNW_EH_FRAME = 0x6474e550,
	PT_LOSUNW = 0x6ffffffa,
	PT_SUNWBSS = 0x6ffffffa,
	PT_SUNWSTACK = 0x6ffffffb,
	PT_SUNWDTRACE = 0x6ffffffc,
	PT_SUNWCAP = 0x6ffffffd,
	PT_HISUNW = 0x6fffffff,
	PT_HIOS = 0x6fffffff,
	PT_LOPROC = 0x70000000,
	PT_HIPROC = 0x7fffffff
};

enum PT_Flags
{
	PF_X = 0x1,				 //Execute
	PF_W = 0x2,				 //Write
	PF_R = 0x4,				 //Read
	PF_MASKPROC = 0xf0000000 //Unspecified
};

#endif