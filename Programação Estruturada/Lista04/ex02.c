#include <stdio.h>
// Questão 2. Escreva um programa que devolva o tamanho da maior cadeia de 0’s de uma string

#define MAX 1000

void contarZeros(char string[]) {
  int auxiliar = 0, count = 0;
  for (int i = 0; i < MAX; i++) {
    count = (string[i] == '0') ? count + 1 : 0;
    if (count > auxiliar) auxiliar = count;
  }
  printf("%d", auxiliar);
}

int main() {
  char string[MAX];
  printf("Digite a cadeia de numeros: ");
  scanf("%[^\n]", string);
  getchar();

  contarZeros(string);
  
  return 0;
}