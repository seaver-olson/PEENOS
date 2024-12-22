CC := aarch64-linux-gnu-gcc
LD := aarch64-linux-gnu-ld
OBJDUMP := aarch64-linux-gnu-objdump
OBJCOPY := aarch64-linux-gnu-objcopy
CONFIGS := -DCONFIG_HEAP_SIZE=4096

CFLAGS := -Iinclude -O0 -ffreestanding -fno-pie -fno-stack-protector -g3 -mcpu=cortex-a53+nofp -Wall $(CONFIGS)

ODIR = obj
SDIR = src

# Define subdirectories
ARCH_DIR = $(SDIR)/arch/arm
COMMANDS_DIR = $(SDIR)/commands
DRIVERS_DIR = $(SDIR)/drivers
KERNEL_DIR = $(SDIR)/kernel
LIBC_DIR = $(SDIR)/libc
EXCEPTIONS_DIR = $(KERNEL_DIR)/exceptions

# Object files by directory
ARCH_OBJS = $(ODIR)/arch/arm/boot.o $(ODIR)/arch/arm/vector_table.o
COMMANDS_OBJS = $(ODIR)/commands/cmds.o
DRIVERS_OBJS = $(ODIR)/drivers/fat.o $(ODIR)/drivers/sd.o $(ODIR)/drivers/systimer.o
KERNEL_OBJS = $(ODIR)/kernel/kernel_main.o $(ODIR)/kernel/mmu.o $(ODIR)/kernel/page.o
EXCEPTIONS_OBJS = $(ODIR)/kernel/exceptions/exc.o $(ODIR)/kernel/exceptions/interrupt.o
LIBC_OBJS = $(ODIR)/libc/delays.o $(ODIR)/libc/pLib.o $(ODIR)/libc/rprintf.o

# Combine all objects
OBJS = $(ARCH_OBJS) $(COMMANDS_OBJS) $(DRIVERS_OBJS) $(KERNEL_OBJS) $(EXCEPTIONS_OBJS) $(LIBC_OBJS)

# Pattern rules for .c and .s files
$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -g -o $@ $^

$(ODIR)/%.o: $(SDIR)/%.s
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -g -o $@ $^

# Targets
all: bin rootfs.img

bin: $(OBJS)
	$(LD) $(OBJS) -Tkernel.ld -o kernel8.img
	cp kernel8.img kernel8.elf
	$(OBJCOPY) -O binary kernel8.img
	aarch64-linux-gnu-size kernel8.elf

clean:
	rm -rf obj/*
	rm -f rootfs.img
	rm -f kernel8.img
	rm -f kernel8.elf

debug:
	screen -S qemu -d -m qemu-system-aarch64 -machine raspi3b -kernel kernel8.img -hda rootfs.img -S -s -serial null -serial stdio -monitor none -nographic -k en-us 
	TERM=xterm gdb -x gdb_init_prot_mode.txt && killall qemu-system-aarch64

run:
	clear && qemu-system-aarch64 -machine raspi3b -kernel kernel8.img -drive file=rootfs.img,format=raw,if=sd -serial null -serial stdio -monitor none --nographic
	
disassemble:
	$(OBJDUMP) -D kernel8.elf

rootfs.img:
	dd if=/dev/zero of=rootfs.img bs=1M count=16
	mkfs.fat -F12 rootfs.img
	sudo mkdir -p /mnt/disk
	sudo mount rootfs.img /mnt/disk
	sudo mkdir -p /mnt/disk/boot/firmware
	sudo mkdir /mnt/disk/bin
	sudo mkdir /mnt/disk/etc
	sudo umount /mnt/disk
