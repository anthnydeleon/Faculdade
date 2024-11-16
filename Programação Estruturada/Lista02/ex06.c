#include <stdio.h>
#define MAX 100
// Questão 6. Escreva um programa que, dados dois inteiros n e m, onde 1 ≤n,m ≤1000, leia uma
// matriz A de ordem n ×m e verifique se a matriz A é simétrica. Lembre-se que uma matriz é simétrica
// se e somente se Ai,j =A j,i

void le_matriz(int M[][MAX], int n) {
  for(int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
     scanf("%d", &M[i][j]); 
    }
  }
}

void verifySimetry(int M[][MAX], int n) {
  int isSimetric = 1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (M[i][j] != M[j][i]) {
        isSimetric = 0;
      }
    }
    break;
  }
  printf(isSimetric == 1 ? "Simetrica" : "Nao simetrica");
}

int main() {
  int n, m;
  printf("Digite a quantidade de linhas e colunas da matriz: ");
  scanf("%d %d", &n, &m);
  
  if (n != m) {
    printf("Nao simetrica");
    return 0;
  }
  
  int A[MAX][MAX];
  le_matriz(A, n);
  verifySimetry(A, n);

  return 0;
}