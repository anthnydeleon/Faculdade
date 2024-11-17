#include <stdio.h>
#include <string.h>
// Questão 4. Escreva um programa que receba uma palavra e uma frase como entrada. O programa
// deve escrever “sim” se a palavra estiver presente na frase e “não” caso contrário

#define MAX 1000

void palavra_na_frase(char palavra[], char frase[]) {
  strstr(frase, palavra) != NULL ? printf("Sim") : printf("Nao");
  
  // for (int i = 0; i < MAX; i++) {
  //   if (frase[i] >= 'A' && frase[i] <= 'Z' || frase[i] >= 'a' && frase[i] <= 'z') {
  //   }
  // }
}

int main() {
  char frase[MAX], palavra[MAX];
  printf("Digite a palavra: ");
  scanf("%[^\n]", palavra);
  getchar();

  printf("Digite a frase: ");
  scanf("%[^\n]", frase);
  getchar();

  palavra_na_frase(palavra, frase);

  return 0;
}