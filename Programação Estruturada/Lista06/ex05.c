#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gcd(int a, int b) {
  if (b == 0) return a;
  return gcd(b, a%b);
}

int main() {
  int a = 48, b = 18;
  int result = gcd(a, b);
  printf("O MDC de %d e %d eh: %d\n", a, b, result);
  
  return 0;
}