# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
#.set VIDINFO,  1 << 2						# OS wants video mode set
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot


# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
#.long 0, 0, 0, 0, 0
#.long 0 # 0 = set graphics mode
#.long 1024, 768, 32 # Width, height, depth

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 65536 # 16 KiB
stack_top:
.align 4
# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp
	movl %esp, %ebp
	push %ebx

	# Initialize the core kernel before running the global constructors.
	call kernel_early

	cli
.Lhang:
	hlt
	jmp .Lhang
.size _start, . - _start
