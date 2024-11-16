#include <stdio.h>
// Questão 6. Escreva um programa que leia um inteiro n e imprima os n primeiros números primos.

void printPrimeNumbers(int n) {
  int count = 1, number = 1;

  while (count != n+1) {
    int isPrime = 1;
    for (int i = 2; i < number; i++) {
      if (number % i == 0) {
        isPrime = 0;
        break;
      }
    }
    
    if (isPrime == 1) {
      printf("%d ", number);
      count++;
    }
    number++;
  }
}

int main() {
  int n;
  
  printf("Digite um numero: ");
  scanf("%d", &n);  

  printPrimeNumbers(n);

  return 0;
}