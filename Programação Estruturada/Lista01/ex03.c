#include <stdio.h>
// Questão 3. Escreva uma função em C que compute o valor de n!, onde n ∈ N

int fatorial(int n) {
  int result = n;
  for (int i = n-1; i > 1; i--) result *= i;
  return result;
}

int main() {
  int n, result;
  printf("Digite um numero: ");
  scanf("%d", &n);

  result = fatorial(n);
  printf("%d", result);
  
  return 0;
}