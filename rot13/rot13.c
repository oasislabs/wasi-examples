#include <stdio.h>

int main(int argc, char ** argv) {
  int c;
  while (1) {
    c = getchar();
    if (c == EOF) {
      return 0;
    }
    if (c >= 'a' && c <= 'z') {
      c += 13;
      if (c > 'z') {
        c -= 26;
      }
    }
    putchar(c);
  }
  return 0;
}
