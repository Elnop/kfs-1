# KFS-1 - Essential Knowledge

## Target Architecture
- **i386 (x86)** mandatory
- [Intel x86 Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [x86 Assembly Guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)

## Bootloader
- **GRUB** (GRand Unified Bootloader)
- [GRUB Manual](https://www.gnu.org/software/grub/manual/grub/grub.html)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [OSDev GRUB](https://wiki.osdev.org/GRUB)

## Assembly
- Boot code in **ASM** with multiboot header
- [AT&T](https://fr.wikipedia.org/wiki/AT%26T_(assembleur))
- [NASM Documentation](https://www.nasm.us/xdoc/2.16.01/html/nasmdoc0.html)
- [NASM WIKI](https://fr.wikipedia.org/wiki/Netwide_Assembler)
- [OSDev Bare Bones](https://wiki.osdev.org/Bare_Bones)
- [x86 Instruction Reference](https://www.felixcloutier.com/x86/)

## Compilation
### Required Flags (for C++)
```
-fno-builtin
-fno-exception
-fno-stack-protector
-fno-rtti (-ffreestanding for C)
-nostdlib
-nodefaultlibs
```
- [GCC Options](https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html)
- [Freestanding Environment](https://wiki.osdev.org/C_Library)

## Linking
- Create a **custom linker script** (.ld)
- Use `ld` binary but NOT host's .ld file
- [UTAH](https://www.math.utah.edu/docs/info/ld_3.html#SEC4)
- [GNU LD Manual](https://sourceware.org/binutils/docs/ld/)
- [Linker Scripts](https://sourceware.org/binutils/docs/ld/Scripts.html)
- [OSDev Linker Scripts](https://wiki.osdev.org/Linker_Scripts)

## VGA Text Mode
- Screen output via VGA buffer
- Memory address: **0xB8000**
- Format: character + color attribute
- [OSDev Text Mode](https://wiki.osdev.org/Text_mode)
- [VGA Text Buffer](https://wiki.osdev.org/Printing_To_Screen)

## Emulation
- Recommended: **KVM** (optional)
- Alternative: QEMU
- [QEMU Documentation](https://www.qemu.org/docs/master/)
- [OSDev Testing](https://wiki.osdev.org/Testing)

## Virtual Image Creation
- Max size: **10 MB**
- GRUB installation on image
- [Creating Disk Images](https://wiki.osdev.org/Loopback_Device)

## Makefile
- Compile sources (ASM + chosen language)
- Link all objects
- [Make Manual](https://www.gnu.org/software/make/manual/make.html)

## Supported Languages
- **C** (recommended), C++, Rust, Go, pure ASM
- Limitations: no dynamic allocations initially
- [OSDev Languages](https://wiki.osdev.org/Languages)
- [Rust OS Development](https://os.phil-opp.com/)

## Basic Kernel Library
- Basic types (`uint8_t`, `uint32_t`, etc.)
- String functions (`strlen`, `strcmp`, etc.)
- No stdlib available
- [OSDev Required Knowledge](https://wiki.osdev.org/Required_Knowledge)

## Main Resources
- [OSDev Wiki](https://wiki.osdev.org/) - **Resource #1**
- [OSDev Bare Bones Tutorial](https://wiki.osdev.org/Bare_Bones)
- [OSDev Meaty Skeleton](https://wiki.osdev.org/Meaty_Skeleton)
- [Intel Architecture Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

## Minimum Requirements
1. Kernel bootable via GRUB
2. ASM boot code with multiboot header
3. Basic kernel library
4. Screen interface (VGA text mode)
5. Display "42" on screen

## Bonus Features
- Scroll and cursor support
- VGA color support
- printf/printk implementation
- Keyboard handling
- Multiple screens with keyboard shortcuts

## Useful Commands
```bash
# Compile ASM
nasm -f elf32 boot.s -o boot.o

# Compile C freestanding
gcc -m32 -c kernel.c -o kernel.o -ffreestanding -nostdlib

# Link
ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o

# Install GRUB
grub-install --boot-directory=mnt/boot /dev/loop0

# Test with QEMU
qemu-system-i386 -kernel kernel.bin
```

## Critical Points
- No dependencies on system libraries
- Custom linker script mandatory
- Final size ≤ 10 MB
- i386 architecture only
