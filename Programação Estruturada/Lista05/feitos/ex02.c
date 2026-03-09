#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 2. Escreva um programa que, dado um inteiro n fornecido pelo usuário, leia n caracteres
// fornecidos pelo usuário e as armazene em um vetor. Na sequência, para cada uma das n entradas
// armazenadas no vetor, o seu programa deve imprimir a seguinte linha: “entrada xxx, conteudo
// yyy, endereco: zzz”, onde xxx deve ser substituído pelo índice da entrada, yyy pelo conteúdo
// dessa entrada no array e zzz pelo endereço de memória dessa entrada em decimal.

void le_vetor(int* v, int n) {
  printf("Digite %d numeros: ", n);
  for(int i = 0; i < n; i++)
    scanf("%d", &v[i]);
}

void imprime_conteudo(int *v, int n) {
  for (int i = 0; i < n; i++) {
    printf("Entrada: %d, conteudo: %d, endereco: %p\n", i, v[i], &v[i]);
  }
}

int main() {
  int n;
  printf("Digite o tamanho do vetor: ");
  scanf("%d", &n);

  int* vetor;
  vetor = calloc(n, sizeof(int));
  le_vetor(vetor, n);
  imprime_conteudo(vetor, n);

  free(vetor);

  return 0;
}