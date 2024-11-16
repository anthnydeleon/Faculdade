#include <stdio.h>
// Questão 2. Escreva um programa em C que leia um número n do usuário e imprima todos os números primos no intervalo [1,n].

void printPrimeNumbers(int n) {
  for (int i = 1; i <= n; i++) {
    int isPrime = 1;
    for (int j = 2; j < i; j++) {
      if (i != j && i % j == 0) {
      isPrime = 0;
      break;
      }
    }
    if (isPrime == 1) printf("%d ", i);
  }
}

int main() {
  int n;
  printf("Digite um numero: ");
  scanf("%d", &n);

    printPrimeNumbers(n);
  return 0;
}
