#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Questão 6. Adapte o programa que resolve o problema da Torre de Hanoi visto em sala de aula. Ao
// invés de exibir os movimentos, imprima o número de movimentos necessários para mover os n discos.

void hanoi(int n, char orig, char dest, char aux, int* soma) {
  if (n > 0) {
    hanoi(n - 1, orig, aux, dest, soma);
    (*soma)++;
    hanoi(n - 1, aux, dest, orig, soma);
  }
}

int main() {
  int n = 3;
  int soma = 0;
  hanoi(n, 'A', 'C', 'B', &soma);
  printf("Numero total de movimentos: %d\n", soma);

  return 0;
}