#include <stdio.h>
#include <stdlib.h>
// Questão 1. Escreva um programa que, dado um inteiro n fornecido pelo usuário, leia n números
// inteiros fornecidos pelo usuário e compute a média desses números.

void le_vetor(int* v, int n) {
  printf("Digite %d numeros: ", n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &v[i]);
  }
}

int soma_vetor(int *v, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += v[i];
  }
  return sum;
}

int main() {
  int n;
  printf("Digite o tamanho do vetor: ");
  scanf("%d", &n);

  int* vetor;
  vetor = calloc(n, sizeof(int));
  le_vetor(vetor, n);
  
  int result = soma_vetor(vetor, n);
  printf("Resultado da soma: %d", result);
  
  free(vetor);

  return 0;
}