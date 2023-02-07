#include "function.h"
#include "../drivers/screen.h"

void panic(char* exception_message) {
  set_screen_attr(BLACK_ON_RED);
  clear_screen();
  print("panic!\n");
  print("exception message: ");
  print(exception_message);
  asm volatile("hlt");
}
