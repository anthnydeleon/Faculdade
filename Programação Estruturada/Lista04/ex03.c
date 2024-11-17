#include <stdio.h>
// Questão 3. Escreva um programa que converte os caracteres maiúsculos de uma string em minúsculos 
// e vice-versa. Suponha que só há caracteres latinos sem acentos

#define MAX 100

void inverteCaracter(char string[], char resultado[]) {
  for (int i = 0; i < MAX; i++) {
    if (string[i] >= 'a' && string[i] <= 'z') {
      resultado[i] = string[i]-32;
    } else if (string[i] >= 'A' && string[i] <= 'Z') {
      resultado[i] = string[i]+32;
    } else {
      resultado[i] = string[i];
    } 
  }
}

int main() {
  char string[MAX], resultado[MAX];
  printf("Digite a string: ");
  scanf("%[^\n]", string);
  getchar();

  inverteCaracter(string, resultado);
  printf("%s", resultado);

  return 0;
}