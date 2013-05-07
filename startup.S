// multiboot constants (basic)
#define MULTIBOOT_HEADER_MAGIC	0x1BADB002
#define MULTIBOOT_HEADER_FLAGS	0

// stack, main
.extern	stack, kmain

.section .text
.global arch_start

// 32 bit align
.align  4

// multiboot header
multiboot_header:
	// magic?
	.long	MULTIBOOT_HEADER_MAGIC
	// flags
	.long	MULTIBOOT_HEADER_FLAGS
	// checksum
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

arch_start:
	// stack pointer init
	mov	$stack, %esp

	// starting status register - EFLAGS register
	pushl	$0
	popf

	call	kmain

	// disable interrupts and suspend processor
	cli
loop:   hlt
	jmp	loop


.section .bss
.align  4

	.lcomm _stack_, 4096
stack:
