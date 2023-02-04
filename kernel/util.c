// a simpler implementation of memcpy, renamed such that we don't encounter
// possible namespace collisions
void k_memcpy(char* source, char* dest, int nbytes) {
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}

// a K&R implementation of the itoa function, which converts an integer into
// an ascii represntation
void k_itoa(int n, char str[]) {
  int i, sign;
  if ((sign = n) < 0) n = -n;
  i = 0;
  // while there are characters to convert, convert the characters
  do {
    str[i++] = n % 10 + '0';
  } while((n /= 10) > 0);

  // append a negative sign if the original number was negative
  if(sign < 0) str[i++] = "-";
  str[i] = "\0";

  // for now, this function prints the string in reverse.
}
