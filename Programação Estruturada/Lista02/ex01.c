#include <stdio.h>
// Questão 1. Implemente a seguinte função
// @valores é um vetor contendo os n números
// @n é a variável que diz quantos valores estão no vetor
// @chave é um número inteiro
// Retorno: Sua função deve retornar i se valores[i] == chave.
// Se o valor 'chave'não está no vetor, então sua função deve retornar -1

void le_vetor(int vetor[], int n) {
  printf("Digite o vetor: ");
  for (int i = 0; i < n; i++) {
    scanf("%d", &vetor[i]);
  }
}

int busca(int vetor[], int n, int chave) {
  for (int i = 0; i < n; i++) {
    if (vetor[i] == chave) return i;
  }
  return -1;    
}

int main() {
  int n, chave, result;

  printf("Digite o tamanho do vetor e o valor da chave: ");
  scanf("%d %d", &n, &chave);

  int vetor[n];
  le_vetor(vetor, n);
  
  result = busca(vetor, n, chave);
  printf("Indice da chave: %d", result);
  
  return 0;
}