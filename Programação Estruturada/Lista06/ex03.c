#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int soma(int* v, int n) {
  if (n == 0) return v[n];
  return v[n] + soma(v, n-1); 
}

void le_vetor(int* v, int n) {
  for (int i = 0; i < n; i++) {
    scanf("%d", &v[i]);
  }
}

int main() {
  int n = 3;
  int* vetor = calloc(n, sizeof(int));
  le_vetor(vetor, n);

  int result = soma(vetor, n-1);
  printf("%d", result);

  free(vetor);

  return 0;
}