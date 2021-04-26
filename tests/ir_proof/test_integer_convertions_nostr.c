int main () {
  const int over = 65000;
  signed int over1 = over;
  // char overflowed
  signed char int8 =  over1;
  signed int int8int = (signed int)(unsigned char)int8;

  // char with printed binary value
  signed char bin = 0b11101000;
  signed int binInt = (signed int)(unsigned char)bin;

  return 0;
}
