#include <stdio.h>
#define MAX 100
// Questão 5. Escreva um programa que, dados dois inteiros n e m, onde 1 ≤n,m ≤1000, leia uma
// matriz A de ordem n ×m e calcule a sua transposta.

void le_matriz(int M[][MAX], int n, int m) {
  printf("Digite os elementos da matriz de tamanho %dx%d: ", n, m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      scanf("%d", &M[i][j]);
    }
  }
}

void imprime_matriz(int M[][MAX], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
}

void transpor_matriz(int M[][MAX], int T[][MAX], int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      T[j][i] = M[i][j];
    }
  }
}

int main() {
  int n, m;
  printf("Digite a quantidade de linhas e colunas da matriz: ");
  scanf("%d %d", &n, &m);

  int A[MAX][MAX], T[MAX][MAX];

  le_matriz(A, n, m);
  imprime_matriz(A, n, m);
  transpor_matriz(A, T, n, m);
  imprime_matriz(T, m, n);

  return 0;
}