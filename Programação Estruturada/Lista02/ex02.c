#include <stdio.h>
// Questão 2. Escreva um programa que leia uma sequência de n números inteiros, 1 ≤n ≤100, e
// imprima esses números em ordem não crescente.

void le_vetor(int vetor[], int n) {
  printf("Digite os valores: ");
  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
  }
}

void imprime_vetor(int vetor[], int n) {
  printf("\nOrdem descrecente: ");
  for (int i = 0; i < n; i++) {
    printf("%d ", vetor[i]);
  }
}

void descrecente(int vetor[], int n) {
  for (int i = 1; i < n; i++) {
    int aux;
    for (int j = 0; j < n; j++) {
      if (vetor[i] > vetor[j]) {
        aux = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = aux;
      }
    }
  }
}

int main() {
  int n;
  
  printf("Digite o tamanho da sequencia: ");
  scanf("%d", &n);

  int vetor[n];
  le_vetor(vetor, n);

  descrecente(vetor, n);
  imprime_vetor(vetor, n);

  return 0;
}