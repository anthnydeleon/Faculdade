#include <stdio.h>
// Questão 1. Neste exercício, você vai implementar o algoritmo de busca binária em um vetor ordenado. 
// Mais precisamente, você deve implementar a função abaixo que implementa esse algoritmo.
// @valores é um vetor contendo n números ordenados de forma não decrescente,
// ou seja, valores[0] <= valores[1] <= valores[2] <= ...
// @n é a variável que diz quantos valores estão no vetor
// @chave é um número inteiro
// Retorno: Retorna o menor valor de i tal que valores[i] == chave.
// Caso o valor de @chave não esteja contido em @valores, retorna -1

void le_vetor(int vetor[], int n) {
  printf("Digite os valores: ");
  for(int i = 0; i < n; i++) {

    scanf("%d", &vetor[i]);
  }
}

int busca_binaria(int valores[], int n, int chave) {
  int inicio = 0, fim = n-1;
  for (int i = inicio; i < n; i++) {
    int meio = (inicio+fim)/2;
    (chave <= valores[meio]) ? (fim = meio) : (inicio = meio+1);
    if (valores[i] == chave) return i;
  }

  return -1;
}

int main() {
  int n, chave, resultado;
  printf("Digite a quantidade de elementos e a chave: ");
  scanf("%d %d", &n, &chave);

  int vetor[n];
  le_vetor(vetor, n);

  resultado = busca_binaria(vetor, n, chave);
  printf("%d", resultado);

  return 0;
}