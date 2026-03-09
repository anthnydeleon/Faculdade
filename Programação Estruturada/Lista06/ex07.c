#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void collatz(int n) {
  if (n == 1) {
    printf("%d ", n);
  } else if (n % 2 == 0) {
    printf("%d ", n);
    collatz(n/2);
  } else {
    printf("%d ", n);
    collatz(n*3+1);
  }
}

int main() {
  int n = 7;

  collatz(n);
  return 0;
}