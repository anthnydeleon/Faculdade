#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int funcao_recursiva(int n, int k) {
  if (k == 0 || k == n) return 1;
  return funcao_recursiva(n - 1, k - 1) + funcao_recursiva(n - 1, k);
}

int main() {
  int n = 10, k = 2;
  int result = funcao_recursiva(n, k);
  printf("%d", result);

  return 0;
}