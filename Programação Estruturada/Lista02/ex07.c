#include <stdio.h>
#define MAX 100

// Questão 7. Escreva um programa que:
// • leia os inteiros n e m, onde 1 ≤n,m ≤1000;
// • leia uma matriz A de ordem n×m;
// • leia dois inteiros i e j, com 1 ≤i,j ≤n;
// • compute uma nova matriz B formada pela troca da i-ésima linha com a j-ésima linha de A.

void le_matriz(int M[][MAX], int n, int m) {
  for(int i = 0; i < n; i++) {
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

void trocar_linha(int M[][MAX], int n, int m, int i, int j) {
  int aux;
  for (int k = 0; k < m; k++) {
    aux = M[i-1][k];
    M[i-1][k] = M[j-1][k];
    M[j-1][k] = aux;
  }
}

int main() {
  int n, m;
  printf("Digite a quantidade de linhas e colunas da matriz: ");
  scanf("%d %d", &n, &m);

  int A[MAX][MAX];
  le_matriz(A, n, m);

  int i, j;
  printf("Digite as linhas que deseja trocar: ");
  scanf("%d %d", &i, &j);
  
  trocar_linha(A, n, m, i, j);
  imprime_matriz(A, n, m);

  return 0;
}