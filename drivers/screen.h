#ifndef _K_SCREEN_H
#define _K_SCREEN_H

#include "../cpu/type.h"

// define video address so we don't need to explicitly define it
// this video address is for text mode
#define VIDEO_ADDRESS 0xb8000

// screen size defs
#define MAX_ROWS 25
#define MAX_COLS 80

// color defs
#define WHITE_ON_BLACK 0x0f
#define GREEN_ON_BLACK 0x02
#define BLACK_ON_RED 0x40

// screen I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

char attr;

// public kernel API
void clear_screen();
void print_at(char* message, int col, int row);
void print(char* message);
void print_backspace();
void set_screen_attr(char screen_attr);

#endif
