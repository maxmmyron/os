C_SOURCES = $(wildcard kernel/*.c drivers/*.c clib/*.c process/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h clib/*.h process/*.h)

# syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

# use debugging symbols in gcc
CFLAGS = -g

# default rule: concat the bootloader and kernel into a single os image
os-img.bin: boot/boot.bin kernel.bin
	cat $^ > os-img.bin

# build the kernel binary
# --oformat binary removes symbols that we dont need
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# for debugging
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: os-img.bin
	qemu-system-i386 -fda os-img.bin

debug: os-img.bin kernel.elf
	qemu-system-i386 -S -s -fda os-img.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# generic rules for wildcards
%.o: %.c ${C_HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

# removes all build files from directories
clean:
	rm -rf *.bin *.dis *.o os-img.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o process/*.o clib/*.o
