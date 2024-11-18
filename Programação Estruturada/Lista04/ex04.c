#include <stdio.h>
// Questão 4. Escreva um programa que receba uma palavra e uma frase como entrada. O programa
// deve escrever “sim” se a palavra estiver presente na frase e “não” caso contrário

#define MAX 1000

int lengthWord(char palavra[]) {
  int count = 0;
  while (palavra[count] != '\0') {
    count++;
  }
  return count;
}

int palavra_na_frase(char palavra[], char frase[]) {
  int i = 0, j = 0;
  int len = lengthWord(palavra);

  while (frase[i] != '\0') {
    if (frase[i] == palavra[j]) {
    j++;
    if (j == len) {
      if (frase[i+1] == ' ' || frase[i+1] == '\0') {
        if (i - j < 0 || frase[i - j] == ' ') {
          return 1;
        }
      } 
    } 
    } else {
        j = 0;
    }
    i++;
  }
  return 0;
}

int main() {
  char frase[MAX], palavra[MAX];
  printf("Digite a palavra: ");
  scanf("%[^\n]", palavra);
  getchar();

  printf("Digite a frase: ");
  scanf("%[^\n]", frase);
  getchar();

  palavra_na_frase(palavra, frase) ? printf("Sim") : printf("Nao");
  
  return 0;
}