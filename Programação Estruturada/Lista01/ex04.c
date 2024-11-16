#include <stdio.h>
// Questão 4. Escreva uma função em C que compute o valor de (n/k), onde 0 ≤k ≤n e n,k ∈ N

int fatorial(int n) {
  int result = n;
  for (int i = n-1; i > 1; i--) result *= i;
  return result;
}

int combinatoria(int n, int k) {
  int result;
  result = fatorial(n) / (fatorial(k) * fatorial(n-k));
  return result;
}

int main() {
  int n, k, result;
  printf("Digite os valores de n e k: ");
  scanf("%d %d", &n, &k);

  result = combinatoria(n, k);
  printf("Resultado da combinatoria: %d", result);
  
  return 0;
}