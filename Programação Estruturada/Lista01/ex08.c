#include <stdio.h>
// Questão 8. Escreva um programa em C que leia um número inteiro n fornecido pelo usuário e imprima Fn, 
// onde Fn é o n-ésimo número de Fibonacci que é definido como:
// Fn    1,se n =1 ou n =2,
//      Fn−1 + Fn−2,caso contrário


void fibonacciSequence(int n) {
  int fn = 0, fn_1 = 0, fn_2 = 0, count = 0;

  for (int i = 0; i <= count; i++) {
    if (count == n) break;

    if (i == 1 || i == 2) {
      fn = 1;
      fn_1 = 1; 
      fn_2 = 1;
    } else {
      fn = fn_1 + fn_2;
      fn_2 = fn_1;
      fn_1 = fn;
    }
    count++;
  }
  printf("%d", fn);
}

int main() {
  int n;
  printf("Digite o numero que deseja achar na sequencia: ");
  scanf("%d", &n);

  fibonacciSequence(n);

  return 0;
}