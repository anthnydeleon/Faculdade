#include <stdio.h>

int main() {
  int vetor[1000], n, soma = 0;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
  }

  for (int i = 0; i < n; i++) {
    soma += vetor[i];
  }

  printf("%d", soma);

  return 0;
}