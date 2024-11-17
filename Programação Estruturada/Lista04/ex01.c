#include <stdio.h>
// Questão 1. Escreva um programa que recebe um caractere e uma frase e imprima a frase fornecida
// até a primeira ocorrência do caractere fornecido.

#define MAX 1000

void cortarString(char string[], char c, char resultado[]) {
  for (int i = 0; i < MAX; i++) {
    resultado[i] = string[i];
    if (string[i] == c) break;
  }
}

int main() {
  char c;
  char string[MAX], resultado[MAX];
  
  printf("Digite o caractere que deseja parar: ");
  scanf("%c", &c);
  getchar();

  printf("Digite a string: ");
  scanf("%[^\n]", string);
  getchar();

  cortarString(string, c, resultado);

  printf("%s", resultado);
}