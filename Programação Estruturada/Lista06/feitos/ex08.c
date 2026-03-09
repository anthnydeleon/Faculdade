#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 8. Implemente uma função recursiva para gerar a sequência de Collatz para um dado número
// inteiro positivo. A sequência de Collatz é gerada a partir das seguintes regras:
// • Se o número atual for par, divida-o por 2.
// • Se o número atual for ímpar, multiplique-o por 3 e some 1.

void sequencia_collatz(int n) {
  printf("%d ", n);
  if (n == 1) {
    return;
  } else if (n % 2 == 0) {
    sequencia_collatz(n/2);
  } else {
    sequencia_collatz((n*3)+1);
  }
}

int main() {
  int n;
  scanf("%d", &n);
  sequencia_collatz(n);

  return 0;
}