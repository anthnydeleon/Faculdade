#include <stdio.h>

int mdc(int a, int b) {
  while (b != 0) {
    int temp = a % b;
    a = b;
    b = temp;
  }
  return a;
}


int main() {
  int a, b;
  scanf("%d %d", &a, &b);
  
  int mmc = (a * b) / mdc(a, b);
  printf("%d", mmc);
  
  return 0;
}