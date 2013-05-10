CC = clang
CFLAGS = -c -Os -m32 -Wall -ffreestanding -nostdlib -fno-stack-protector -s 
LDFLAGS = -Os -melf_i386 -e arch_start -Ttext=0x100000 -s --gc-sections
QEMU_CMD = qemu-system-i386 -kernel
KERNEL = kernel.elf

all : kernel qemu

qemu : ${KERNEL}
	${QEMU_CMD} ${KERNEL}

kernel : startup kernel_c
	ld ${LDFLAGS} startup.o kernel.o -o ${KERNEL}

startup : startup.S
	${CC} ${CFLAGS} startup.S

kernel_c : kernel.c
	${CC} ${CFLAGS} kernel.c



