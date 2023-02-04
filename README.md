# os

an operating system.

## get it

```
git clone https://github.com/maxmmyron/os.git
cd os
```

## running
i run this through qemu in wsl. Because wsl has no graphical capabilities I use Xming as a display server for running the graphical side.

### on windows
1. install wsl and xming
2. install qemu in wsl (i used the qemu-kvm package)
3. in wsl: `export DISPLAY=$(route.exe print | grep 0.0.0.0 | head -1 | awk '{print $4}'):0.0`
   - if you have wsl1 (like i do), instead use `export DISPLAY=:0`, since wsl1 runs on the same IP addresses as windows
4. `qemu-system-x86_64 -drive format=raw,file=boot.bin`


### on linux
i assume its a lot easier given the patchwork necessary on windows thru wsl.
1. install qemu
2. `qemu-system-x86_64 -drive format=raw,file=boot.bin`

## adknowledgements
primary kernel code is my own implementation of primary operating system processes as learned.
bootloader is an adaptation of several sources:
- [Writing a Simple Operating System — from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)  by Nick Blundell
- [os-tutorial](https://github.com/cfenollosa/os-tutorial) by Carlos Fenollosa
- [OSDev Wiki](https://wiki.osdev.org/Main_Page)
- [The little book about OS development](https://littleosbook.github.io/) by Erik Helin and Adam Renberg
