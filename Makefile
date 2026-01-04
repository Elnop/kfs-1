AS = as
CC = gcc
LD = ld
GRUB_MKRESCUE = grub-mkrescue
BUILD_DIR = .build

ASFLAGS = --32
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-exceptions -fno-stack-protector \
         -nostdlib -nodefaultlibs -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T linker.ld

ASM_SRC = boot/boot.S
C_SRC = kernel/kernel.c \
        kernel/vga.c \
        kernel/keyboard.c \
        kernel/screen.c \
        lib/string.c \
        lib/printk.c

ASM_OBJ = $(patsubst %.S,$(BUILD_DIR)/%.o,$(ASM_SRC))
C_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRC))
OBJ = $(ASM_OBJ) $(C_OBJ)

KERNEL = kernel.bin
ISO = kfs.iso
ISO_DIR = iso

all: $(ISO)

$(ISO): $(KERNEL) grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub/
	./make_iso.sh $(ISO_DIR) $(ISO)

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(KERNEL) $(ISO)
	rm -rf $(ISO_DIR)

re: fclean all

run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

run-iso: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

run-kvm: $(KERNEL)
	qemu-system-i386 -enable-kvm -kernel $(KERNEL)

debug: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL) -s -S

.PHONY: all clean fclean re run run-kvm debug
