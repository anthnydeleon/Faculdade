#include <stdio.h>

int main() {
  int vetor[1000], n, x, count = 0;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
  }
  scanf("%d", &x);

  for (int i = 0; i < n; i++) {
    if (vetor[i] >= x) {
      count++;
    }
  }
  printf("%d\n", count);

  return 0;
}