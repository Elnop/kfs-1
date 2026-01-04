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

DOCKER_IMAGE = kfs-builder

.PHONY: all clean fclean re run run-iso run-kvm debug manualy-iso manualy-bin

all: docker-iso

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(KERNEL) $(ISO)
	rm -rf $(ISO_DIR)

re: fclean all

run: all
	qemu-system-i386 -kernel $(KERNEL)

run-iso: all
	qemu-system-i386 -cdrom $(ISO)

run-kvm: all
	qemu-system-i386 -enable-kvm -kernel $(KERNEL)

debug: all
	qemu-system-i386 -kernel $(KERNEL) -s -S

docker-iso: build-docker
	docker run --rm -v $(PWD):/kfs -w /kfs -u $(shell id -u):$(shell id -g) $(DOCKER_IMAGE) make manualy-iso

docker-iso: build-docker
	docker run --rm -v $(PWD):/kfs -w /kfs -u $(shell id -u):$(shell id -g) $(DOCKER_IMAGE) make manualy-bin

docker-rebuild:
	docker build -t $(DOCKER_IMAGE) .

manualy-iso: $(ISO)

manualy-bin: $(ISO)

$(ISO): $(KERNEL) grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub/
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR)

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build-docker:
	if ! docker image inspect $(DOCKER_IMAGE) >/dev/null 2>&1; then \
		docker build -t $(DOCKER_IMAGE) . || exit 1; \
	fi

rtfm-qemu:
	man qemu-system-i386