#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

// declare private functions
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

// --------------------------------------
// public kernel API
// --------------------------------------

// prints a message on the screen at the specified location. If col or row are
// negative, then we just use the current offset.
void k_print_at(char* message, int col, int row) {
  int offset;

  if(col >= 0 && row >= 0) {
    offset = get_offset(col, row);
  } else {
    // get the current cursor offset and set col & row to that val
    offset = get_cursor_offset();

    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }

  // loop through the message, char by char, and print it to the screen
  int i = 0;
  while (message[i] != 0) {
    offset = print_char(message[i++], col, row, WHITE_ON_BLACK);

    // given the new offset returned from print_char, compute the new position
    // for the next iterations
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
}

// prints a message at the current cursor position.
void k_print(char *message) {
  k_print_at(message, -1, -1);
}

void clear_screen() {
  int screen_size = MAX_COLS * MAX_ROWS;
  int i;

  char *v_mem = VIDEO_ADDRESS;

  // for every space on screen, set to black (i.e. white space character w/
  // black background)
  for(i = 0; i < screen_size; i++) {
    v_mem[i * 2] = ' ';
    v_mem[i * 2 + 1] = WHITE_ON_BLACK;
  }

  // set cursor offset to (0, 0)
  set_cursor_offset(get_offset(0, 0));
}

// --------------------------------------
// private kernel functions
// --------------------------------------

// the lowest-level print function for our kernel. this directly accesses the
// video memeory.
// if col and row and negative, we print at the current cursor location.
// if attr is zero, we default to "white on black" coloring by default
// returns the offset of the next character, and sets the video cursor to the
// returned offset.
int print_char(char c, int col, int row, char attr) {
  unsigned char* v_mem = (unsigned char*) VIDEO_ADDRESS;

  // preset attr if not set to anything
  if (!attr) attr = WHITE_ON_BLACK;

  // if coords are messed up, print error E
  if(col >= MAX_COLS || row >= MAX_ROWS) {
    v_mem[2*(MAX_COLS)*(MAX_ROWS)-2] = "E";
    v_mem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_BLACK;

    return get_offset(col, row);
  }

  // set offset based on params passed in
  int offset;
  if (col >= 0 && row >= 0) offset = get_offset(col, row);
  else offset = get_cursor_offset();

  if(c == '\n') {
    // handle new line, move to next row and reset col position
    row = get_offset_row(offset);
    offset = get_offset(0, row+1);
  } else {
    // once everything is validated and handled, set video memory at offset to
    // character and attribute data
    v_mem[offset] = c;
    v_mem[offset+1] = attr;
    // move forward by 2 spaces in offset
    offset += 2;
  }

  if (offset >= MAX_ROWS * MAX_COLS * 2) {
    int i;
    for (i = 1; i < MAX_ROWS; i++)
      k_memcpy(get_offset(0, i) + VIDEO_ADDRESS,
        get_offset(0, i - 1) + VIDEO_ADDRESS,
        MAX_COLS * 2);

    // add a blank last line to remove previous last line
    char *ll = get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
    // clear each column from last row
    for (i = 0; i < MAX_COLS * 2; i++) ll[i] = 0;

    offset -= 2 * MAX_COLS;
  }

  set_cursor_offset(offset);
  return offset;
}

// --------------------------------------
// private kernel getters & setters
// --------------------------------------

// uses the VGA ports to get the current cursor position through two steps:
// 1. ask for high byte of cursor offset  (data 14)
// 2. ask for low byte of cursor offset   (data 15)
int get_cursor_offset() {
  port_byte_out(REG_SCREEN_CTRL, 14);
  // retrieve high byte and shift 8 bytes so there's room for low byte.
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;

  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);

  // position * size of character cell
  return offset * 2;
}

// uses the VGA ports to set the cursor position. instead of reading from the
// VGA registers, we write to them.
void set_cursor_offset(int offset) {
  // deconstruct offset to just position
  offset /= 2;

  port_byte_out(REG_SCREEN_CTRL, 14);
  // set the screen data to the offset bitshifted forward by 8 (the high byte)
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));

  port_byte_out(REG_SCREEN_CTRL, 15);
  // set the screen data to the remainder data, unary'd with 0xff so we only
  // keep low byte
  port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_offset(int col, int row) {
  return 2 * (row * MAX_COLS + col);
}

int get_offset_col(int offset) {
  return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

int get_offset_row(int offset) {
  return offset / (2 * MAX_COLS);
}
