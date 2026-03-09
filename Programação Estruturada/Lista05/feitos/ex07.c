#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 7. Escreva a função cartesiano(v1, n1, v2, n2). Essa função recebe como parâmetro
// dois vetores de inteiro, v1 e v2, e os seus respectivos tamanhos, n1 e n2. O retorno dessa função deve
// ser um array com os elementos do conjunto obtido pelo produto cartesiano de v1 por v2.

int* cartesiano(int* v1, int n1, int* v2, int n2) {
  int tamanho = n1 * n2 * 2;
  int* result = calloc(tamanho, sizeof(int));

  int k = 0;
  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < n2; j++) {
      result[k++] = v1[i];
      result[k++] = v2[j];
    }
  }
  return result;
}

void imprime_vetor(int* vetor, int n) {
  printf("Produto Cartesiano:\n");
  for (int i = 0; i < n; i += 2) {
    printf("(%d, %d)\n", vetor[i], vetor[i+1]);
  }
}

void le_vetor(int* vetor, int n) {
  printf("Digite o vetor: ");
  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
  }
}

int main() {
  int n1, n2;
  printf("Digite o tamanho do v1 e v2: ");
  scanf("%d %d", &n1, &n2);

  int* v1 = calloc(n1, sizeof(int));
  int* v2 = calloc(n2, sizeof(int));
  le_vetor(v1, n1);
  le_vetor(v2, n2);

  int* result = cartesiano(v1, n1, v2, n2);
  imprime_vetor(result, n1*n2*2);

  return 0;
}