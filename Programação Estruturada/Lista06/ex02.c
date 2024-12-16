#include <stdio.h>

double power(double a, int b) {
  if (b == 0) return 1;
  return a * power(a, b-1);
}

int main() {
  int a = 5, b = 2;
  int result = power(a, b);

  printf("%d", result);

  return 0;
}