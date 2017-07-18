/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;
	PROVIDE(__start_k = .);

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.text BLOCK(4K) : ALIGN(4K)
	{
    PROVIDE(__start_text = .);
		*(.multiboot)
		*(.text)
    PROVIDE(__end_text = .);
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
    PROVIDE(__start_rodata = .);
		*(.rodata)
    PROVIDE(__end_rodata = .);
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
    PROVIDE(__start_data = .);
		*(.data)
    PROVIDE(__end_data = .);

    PROVIDE(__start_tss = .);
		*(.tss)
    PROVIDE(__end_tss = .);
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
    PROVIDE(__start_bss = .);
		*(COMMON)
		*(.bss)
    PROVIDE(__end_bss = .);
	}

	/* Heap */
	.heap BLOCK(4K) : ALIGN(4K)
	{
    PROVIDE(__start_heap = .);
		*(.heap)
    PROVIDE(__end_heap = .);
	}


	PROVIDE(__end_k = .);

	/* The compiler may produce other sections, put them in the proper place in
	   in this file, if you'd like to include them in the final kernel. */
}
