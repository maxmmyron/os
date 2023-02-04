#include "../drivers/ports.h"

void main() {
    // ask VGA control register (0x3d4) for bytes regarding the cursor position
    // request byte 14: high byte of cursor position
    port_byte_out(0x3d4, 14);

    // data is returned in the VGA data register (0x3d5)
    int position = port_byte_in(0x3d5);
    // we got the high byte, so lets bit shift it back 8 bits.
    position = position << 8;

    // request the low byte using the same control register
    port_byte_out(0x3d4, 15);
    // we can just append the incoming byte to the position since the high byte
    // was shifted back
    position += port_byte_in(0x3d5);

    // VGA cells consists of the character and its control data
    int vga_offset = position * 2;

    // write to screen at current cursor position
    char* vga = 0xb8000;
    vga[vga_offset] = "X";
    // white text on black background
    vga[vga_offset + 1] = 0x0f;
}
