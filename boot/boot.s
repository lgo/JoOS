# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header abiding the Multiboot Standard. Special section for forcing header
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Designating specific memory with symbols for the bottom and top of the stack
# Retaining the ESP register within this section prevents danger
.section .bootstrap_stack
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The entry to the kernel as specified in the linker script
.section .text
.global _start
.type _start, @function
_start:
  # Note that the processor is not fully initialized yet and stuff
  # such as floating point instructions are not available yet.

  # Setup of the stack, placing the top of the stack address in the esp register
  movl $stack_top, %esp

  # Disable interrupts for our kernal's purpose
  cli  
  # Call the main kernel, executing C code
  call kernel_main

  # In the event the kernel returns, puts the computer in a dead loop
  cli
  hlt
.Lhang:
  jmp .Lhang

# Set the size of the _start symbol in respect to current location, '.'
.size _start, . - _start
