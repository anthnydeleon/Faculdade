#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Essa função recebe um número inteiro n como entrada e converte-o em uma representação binária na
// forma de string. O resultado da conversão é armazenado na variável output.

void convert_to_binary(int n, char output[], int* index) {
  if (n > 0) {
    convert_to_binary(n / 2, output, index);
    output[(*index)++] = (n % 2) + '0';
  }
  output[(*index)] = '\0';
}

int main() {
  int n = 3;
  char output[32];

  int index = 0;
  convert_to_binary(n, output, &index);
  printf("\nBinario de %d: %s\n", n, output);

  return 0;
}