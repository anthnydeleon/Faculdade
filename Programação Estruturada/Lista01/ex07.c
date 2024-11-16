#include <stdio.h>
// Questão 7. Escreva um programa que leia dois inteiros i e j, onde i ≤j, e que imprima todos os números primos no intervalo [i,j].

void printPrimeNumbers(int start, int end) {
  for (int i = start; i <= end; i++) {
    int isPrime = 1;
    for (int j = 2; j < i; j++) {
      if (i != 1 && i % j == 0) {
        isPrime = 0;
        break;
      }
    }
    if (i != 0 && isPrime == 1) printf("%d ", i);
  }
}

int main() {
  int start, end;
  
  printf("Digite os valores de i e j: ");
  scanf("%d %d", &start, &end);

  printPrimeNumbers(start, end);

  return 0;
}