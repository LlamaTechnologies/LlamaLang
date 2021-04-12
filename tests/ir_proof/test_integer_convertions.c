#include <stdio.h>

void printIntBin(size_t const, void const * const);

int main () {
  const int over = 65000;

  // char overflowed
  signed char int8 =  over;
  signed int int8int = (signed int)(unsigned char)int8;
  printf("int :\t\t%d\n", over);
  printf("int :\t\t");
  printIntBin(sizeof(unsigned int), &over);

  // bin  representation of char
  printf("char:\t\t%d\n", int8int);
  printf("char:\t\t%c\n", int8int);
  printf("char:\t\t");
  printIntBin(sizeof(unsigned int), &int8int);

  // char with printed binary value
  signed char bin = 0b11101000;
  signed int binInt = (signed int)(unsigned char)bin;

  printf("intBin:\t\t%d\n", binInt);
  printf("charBin:\t\t%c\n", binInt);
  printf("charBin:\t");
  printIntBin(sizeof(signed int), &binInt);

  return 0;
}

void printIntBin(size_t const size, void const * const ptr) {
  unsigned char *b = (unsigned char *)ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; --i) {
    for ( j = 7; j >= 0; --j) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  printf("\n");
}
