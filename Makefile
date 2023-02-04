C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h)

# syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

# use debugging symbols in gcc
CFLAGS = -g

# default rule
os-img.bin: boot/boot.bin kernel.bin
	cat $^ > os-img.bin

# --oformat binary removes symbols that we dont need
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# for debugging
kernel.elf: boot/kernely_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: os-img.bin
	qemu-system-i386 -fda os-img.bin

debug: os-img.bin kernel.elf
	qemu-system-i386 -fda os-img.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# generic rules for wildcards
%.o %.e ${HEADERS}
	${CC} ${CFLAGS} -freestanding -c $< -o $@

%.o %.asm
	nasm $< -f elf -0 $@

%.bin %.asm
	nasm $< -f bin -0 $@

clean:
	rm -rf *.bin *.dis *.o os-img.bin *.elf
	rm-rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
