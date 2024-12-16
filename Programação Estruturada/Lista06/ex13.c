#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 13. Considere uma sequência formada pelos caracteres ‘(‘ e ‘)‘. Dizemos que uma sequên-
// cia de parênteses é válida se:
// 1. Cada parêntese de abertura ‘(‘ possui um parêntese de fechamento ‘)‘ correspondente.
// 2. Em qualquer prefixo da sequência, o número de parênteses de fechamento ‘)‘ nunca excede o
// número de parênteses de abertura ‘(‘.

void verifica_parenteses(char string[], int posicao, int fim, int fechar) {
  if (fechar < 0) {
    printf("Sequencia invalida!\n");
    return;
  }

  if (posicao > fim) {
    if (fechar == 0) printf("Sequencia valida!\n");
    else printf("Sequencia invalida!\n");
    return;
  }

  if (string[posicao] == '(') fechar++;
  if (string[posicao] == ')') fechar--;

  verifica_parenteses(string, posicao + 1, fim, fechar);
}

int main () {
  char string[4] = {'(', ')', '(', ')'};
  int len = strlen(string);
  verifica_parenteses(string, 0, len-1, 0);

  return 0;
}