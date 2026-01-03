.set MULTIBOOT_MAGIC,     0x1BADB002
.set MULTIBOOT_FLAGS,     0x00000000
.set MULTIBOOT_CHECKSUM,  -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot
.align 4
	.long MULTIBOOT_MAGIC
	.long MULTIBOOT_FLAGS
	.long MULTIBOOT_CHECKSUM

.section .bss
.align 16
stack_bottom:
	.skip 16384		/* 16KB stack */
stack_top:

.section .text
.global start
.extern kernel_main

start:
	movl $stack_top, %esp

	pushl $0
	popf

	pushl %ebx
	pushl %eax

	call kernel_main

	cli
hang:
	hlt
	jmp hang

.section .note.GNU-stack,"",@progbits