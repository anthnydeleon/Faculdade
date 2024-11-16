#include <stdio.h>
#define MAX 10
// Questão 2. Leia um número n, onde 1 ≤n ≤10, e então leia uma matriz quadrada M[n][n]. Na
// sequência, leia um caractere c. Caso c =“S′′, calcule a soma dos elementos da diagonal principal da
// matriz M. Caso c = “M′′, então calcule a média dos elementos que estão acima da diagonal superior.

void le_matriz(double matriz[][MAX], int n) {
  printf("Digite os valores da matriz: ");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%lf", &matriz[i][j]);      
    }
  }
}

double soma_diagonal_superior(double matriz [][MAX], int n) {
  double value = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (j >= i) value += matriz[i][j];
    }
  }
  return value;
} 

double media_diagonal_superior(double matriz [][MAX], int n) {
  double value = 0, count = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (j > i) {
        value += matriz[i][j];
        count++;
      }
    }
  }
  return value/count;
} 

int main() {
  int n;
  double result, M[MAX][MAX];
  char operacao;

  printf("Digite o tamanho da matriz: ");
  scanf("%d", &n);

  le_matriz(M, n);

  printf("Digite a operacao desejada (s - soma, m - media): ");
  scanf(" %c", &operacao);

  printf("Operacao escolhida: %c\n", operacao);

  result = operacao == 's' ? soma_diagonal_superior(M, n) : media_diagonal_superior(M, n);
  printf("%.2lf", result);

  return 0;
}