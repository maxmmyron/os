// a simpler implementation of memcpy, renamed such that we don't encounter
// possible namespace collisions
void mcpy(char* source, char* dest, int nbytes) {
  int i;
  for (i = 0; i < nbytes; i++)
    *(dest + i) = *(source + i);
}
