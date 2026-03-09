#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 3. Escreva a função swap(a, b). Essa função recebe como parâmetro dois inteiros a e b
// e não tem nenhum retorno. O comportamento dessa função deve ser o seguinte: após a execução da
// mesma, os valores de a e b devem estar trocados.
void swap(int* a, int* b) {
  int aux = *a;
  *a = *b;
  *b = aux;
}

int main() {
  int a, b;
  scanf("%d %d", &a, &b);

  swap(&a, &b);
  printf("Valor de a: %d \nValor de b: %d\n", a, b);
  
  return 0;
}