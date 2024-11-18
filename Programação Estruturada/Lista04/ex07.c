#include <stdio.h>
// Questão 7. Crie o tipo de dados Matriz e implemente as seguintes funções
// • Matriz matriz_le(int n) lê e retorna uma matriz quadrada de dimensões n×n.
// • Matriz matriz_multiplica(Matriz A, Matriz B) retorna a matriz resultante da multiplicação da matriz A por B.

#define MAX 100

typedef struct {
  int elementos[MAX][MAX];
  int tamanho;
} Matriz;

Matriz matriz_le(int n) {
  Matriz M;
  M.tamanho = n;

  printf("Digite a matriz: ");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%d", &M.elementos[i][j]);
    }
  }
  
  return M;
}

Matriz matriz_multiplica(Matriz A, Matriz B) {
  Matriz M;
  M.tamanho = A.tamanho;

  for (int i = 0; i < M.tamanho; i++) {
    for (int j = 0; j < M.tamanho; j++) {
      M.elementos[i][j] = 0;
    }
  }

  for (int row = 0; row < A.tamanho; row++) {
    for (int col = 0; col < A.tamanho; col++) {
      for (int i = 0; i < A.tamanho; i++) {
        M.elementos[row][col] += A.elementos[row][i] * B.elementos[i][col];
      }
    }
  }
  return M;
}

void imprime_matriz(Matriz M) {
  for (int i = 0; i < M.tamanho; i++) {
    for (int j = 0; j < M.tamanho; j++) {
      printf("%d ", M.elementos[i][j]);   
    }
    printf("\n");
  }
}

int main() {
  int n = 2;
  Matriz A, B, C;

  A = matriz_le(n);
  B = matriz_le(n);
  C = matriz_multiplica(A, B);

  printf("\nMatriz A: \n");
  imprime_matriz(A);

  printf("\nMatriz B: \n");
  imprime_matriz(B);

  printf("\nMatriz AxB: \n");
  imprime_matriz(C);

  return 0;
}