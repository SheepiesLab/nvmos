#include <kernel/userland/elf.h>

bool elf_check_file(Elf32_Ehdr *hdr)
{
	if (!hdr)
		return false;
	if (hdr->e_ident[EI_MAG0] != ELFMAG0)
	{
		ERROR("ELF Header EI_MAG0 incorrect.\n");
		return false;
	}
	if (hdr->e_ident[EI_MAG1] != ELFMAG1)
	{
		ERROR("ELF Header EI_MAG1 incorrect.\n");
		return false;
	}
	if (hdr->e_ident[EI_MAG2] != ELFMAG2)
	{
		ERROR("ELF Header EI_MAG2 incorrect.\n");
		return false;
	}
	if (hdr->e_ident[EI_MAG3] != ELFMAG3)
	{
		ERROR("ELF Header EI_MAG3 incorrect.\n");
		return false;
	}
	return true;
}

bool elf_check_supported(Elf32_Ehdr *hdr)
{
	if (!elf_check_file(hdr))
	{
		ERROR("Invalid ELF File.\n");
		return false;
	}
	if (hdr->e_ident[EI_CLASS] != ELFCLASS32)
	{
		ERROR("Unsupported ELF File Class.\n");
		return false;
	}
	if (hdr->e_ident[EI_DATA] != ELFDATA2LSB)
	{
		ERROR("Unsupported ELF File byte order.\n");
		return false;
	}
	if (hdr->e_machine != EM_386)
	{
		ERROR("Unsupported ELF File target.\n");
		return false;
	}
	if (hdr->e_ident[EI_VERSION] != EV_CURRENT)
	{
		ERROR("Unsupported ELF File version.\n");
		return false;
	}
	if (hdr->e_type != ET_REL && hdr->e_type != ET_EXEC)
	{
		ERROR("Unsupported ELF File type.\n");
		return false;
	}
	return true;
}

// static inline void *elf_load_rel(Elf32_Ehdr *hdr)
// {
// 	int result;
// 	result = elf_load_stage1(hdr);
// 	if (result == ELF_RELOC_ERR)
// 	{
// 		ERROR("Unable to load ELF file.\n");
// 		return NULL;
// 	}
// 	result = elf_load_stage2(hdr);
// 	if (result == ELF_RELOC_ERR)
// 	{
// 		ERROR("Unable to load ELF file.\n");
// 		return NULL;
// 	}
// 	// TODO : Parse the program header (if present)
// 	return (void *)hdr->e_entry;
// }

// void *elf_load_file(void *file)
// {
// 	Elf32_Ehdr *hdr = (Elf32_Ehdr *)file;
// 	if (!elf_check_supported(hdr))
// 	{
// 		ERROR("ELF File cannot be loaded.\n");
// 		return;
// 	}
// 	switch (hdr->e_type)
// 	{
// 	case ET_EXEC:
// 		// TODO : Implement
// 		return NULL;
// 	case ET_REL:
// 		return elf_load_rel(hdr);
// 	}
// 	return NULL;
// }

// static inline Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr)
// {
// 	return (Elf32_Shdr *)((int)hdr + hdr->e_shoff);
// }

// static inline Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx)
// {
// 	return &elf_sheader(hdr)[idx];
// }

// static inline char *elf_str_table(Elf32_Ehdr *hdr)
// {
// 	if (hdr->e_shstrndx == SHN_UNDEF)
// 		return NULL;
// 	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
// }

// static inline char *elf_lookup_string(Elf32_Ehdr *hdr, int offset)
// {
// 	char *strtab = elf_str_table(hdr);
// 	if (strtab == NULL)
// 		return NULL;
// 	return strtab + offset;
// }

// static int elf_get_symval(Elf32_Ehdr *hdr, int table, uint idx)
// {
// 	if (table == SHN_UNDEF || idx == SHN_UNDEF)
// 		return 0;
// 	Elf32_Shdr *symtab = elf_section(hdr, table);

// 	uint32_t symtab_entries = symtab->sh_size / symtab->sh_entsize;
// 	if (idx >= symtab_entries)
// 	{
// 		ERROR("Symbol Index out of Range (%d:%u).\n", table, idx);
// 		return ELF_RELOC_ERR;
// 	}

// 	int symaddr = (int)hdr + symtab->sh_offset;
// 	Elf32_Sym *symbol = &((Elf32_Sym *)symaddr)[idx];
// 	if (symbol->st_shndx == SHN_UNDEF)
// 	{
// 		// External symbol, lookup value
// 		Elf32_Shdr *strtab = elf_section(hdr, symtab->sh_link);
// 		const char *name = (const char *)hdr + strtab->sh_offset + symbol->st_name;

// 		extern void *elf_lookup_symbol(const char *name);
// 		void *target = elf_lookup_symbol(name);

// 		if (target == NULL)
// 		{
// 			// Extern symbol not found
// 			if (ELF32_ST_BIND(symbol->st_info) & STB_WEAK)
// 			{
// 				// Weak symbol initialized as 0
// 				return 0;
// 			}
// 			else
// 			{
// 				ERROR("Undefined External Symbol : %s.\n", name);
// 				return ELF_RELOC_ERR;
// 			}
// 		}
// 		else
// 		{
// 			return (int)target;
// 		}
// 	}
// 	else if (symbol->st_shndx == SHN_ABS)
// 	{
// 		// Absolute symbol
// 		return symbol->st_value;
// 	}
// 	else
// 	{
// 		// Internally defined symbol
// 		Elf32_Shdr *target = elf_section(hdr, symbol->st_shndx);
// 		return (int)hdr + symbol->st_value + target->sh_offset;
// 	}
// }