#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void count_down(int n) {
  if (n == 1) {
    printf("%d", n);
    printf("ACABOU");
  } else {
    printf("%d\n", n);
    count_down(n-1);
  }
}

int main() {
  int n = 5;
  count_down(n);

  return 0;
}