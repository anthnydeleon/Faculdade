#include <stdio.h>

int main() {
  int vetor[1000], n, x;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
  }

  while (scanf("%d", &x) != EOF) {
    int found = 0;
    for (int i = 0; i < n; i++) {
      if (vetor[i] == x) {
        found = 1;
        break;
      }
    }
    if (found) {
      printf("SIM\n");
    } else {
      printf("CHAVE NAO ENCONTRADA\n");
    }
  }
  
  return 0;
}