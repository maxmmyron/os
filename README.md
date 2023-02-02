# os

an operating system demo.

## get it

```
git clone https://github.com/maxmmyron/os.git
cd os
```

## running
i run this through qemu in wsl, with Xming for a graphical interface on windows

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
