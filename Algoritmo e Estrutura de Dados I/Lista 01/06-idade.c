#include <stdio.h>

int main() {
  int anoDeNascimento, anoAtual, idade;
  char nome[50];

  scanf("%d %s %d", &anoAtual, &nome, &anoDeNascimento);

  idade = anoAtual-anoDeNascimento;

  printf("%s, voce completa %d anos de idade neste ano.", nome, idade);
  return 0;
}