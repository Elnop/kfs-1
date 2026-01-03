/* Linker script for KFS-1 kernel */

ENTRY(start)

SECTIONS
{
	/* Kernel starts at 1MB physical address */
	. = 0x00100000;

	/* Multiboot header must be in first 8KB */
	.text ALIGN(4K) : {
		*(.multiboot)
		*(.text)
	}

	/* Read-only data */
	.rodata ALIGN(4K) : {
		*(.rodata)
	}

	/* Initialized data */
	.data ALIGN(4K) : {
		*(.data)
	}

	/* Uninitialized data (BSS) */
	.bss ALIGN(4K) : {
		*(COMMON)
		*(.bss)
	}
}
