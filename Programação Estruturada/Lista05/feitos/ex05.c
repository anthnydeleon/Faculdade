#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 5. Escreva a função swap(a, b). Essa função recebe como parâmetro dois arrays de in-
// teiros a e b e não tem nenhum retorno. O comportamento dessa função deve ser o seguinte: após a
// execução da mesma, os valores de a e b devem estar trocados. Você deve realizar essa tarefa apenas 
// usando manipulação de ponteiro, não copie os elementos de um vetor no outro!

void le_vetor(int* v, int n) {
  printf("Digite %d numeros: ", n);
  for(int i = 0; i < n; i++)
    scanf("%d", &v[i]);
}

void imprime_vetor(int *v, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", v[i]);
  }
}

void swap(int** a, int** b) {
  int* temp = *a;
  *a = *b;
  *b = temp;
}

int main() {
  int n;
  printf("Digite o tamanho do vetor: ");
  scanf("%d", &n);

  int *a, *b;
  a = calloc(n, sizeof(int));
  b = calloc(n, sizeof(int));

  le_vetor(a, n);
  le_vetor(b, n);

  swap(&a, &b);
  
  printf("Vetor a: ");
  imprime_vetor(a, n);

  printf("\nVetor b: ");
  imprime_vetor(b, n);

  free(a);
  free(b);

  return 0;
}