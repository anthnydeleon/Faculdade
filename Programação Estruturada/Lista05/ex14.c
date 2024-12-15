#include <stdio.h>
#include <stdlib.h>

void le_vetor(int* v, int n) {
  for (int i = 0; i < n; i++) {
    scanf("%d", &v[i]);
  }
}

void imprime_vetor(int* v, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", v[i]);
  }
}

int verifica_numero_primo(int n) {
  if (n <= 1) return 0;
  
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) return 0; 
  }
  
  return 1;
}

void map(int* v, int n, int func(int)){
  for (int i = 0; i < n; i++) {
    v[i] = func(v[i]);
  }
}

int main() {
  int n = 5;
  
  int* vetor = calloc(n, sizeof(int));
  le_vetor(vetor, n);

  map(vetor, n, verifica_numero_primo);
  imprime_vetor(vetor, n);

  return 0;
}