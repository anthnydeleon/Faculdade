#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 4. Escreva a função inc(x). Essa função recebe apenas um parâmetro do tipo inteiro e
// não tem retorno. Seu comportamento é o seguinte: após a execução de inc(x), o valor de x está
// incrementado em uma unidade.

void incrementa_um(int* n) {
  *n += 1;
}

int main() {
  int n;
  printf("Digite o valor: ");
  scanf("%d", &n);

  incrementa_um(&n);
  printf("Valor incrementado: %d", n);

  return 0;
}