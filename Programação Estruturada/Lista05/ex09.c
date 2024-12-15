#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Implemente as funções:
// • matriz_le(n, m) essa função lê e retorna uma matriz M de números inteiros fornecida pelo
// usuário. Ela recebe dois parâmetros n e m, onde n é o número de linhas e m, o de colunas. Essa
// função é responsável por alocar a memória para M e por ler todo o conteúdo de M, que deverá ser
// fornecido pelo usuário.
// • matriz_print(M, n, m) essa função imprime na tela do usuário a matriz M que tem n linhas
// e m colunas.

int** le_matriz(int n, int m) {
  int** matriz = calloc(n, sizeof(int));

  printf("Digite a matriz %dx%d: ", n, m);
  for (int i = 0; i < n; i++) {
    matriz[i] = calloc(m, sizeof(int));
    for (int j = 0; j < m; j++) {
      scanf("%d", &matriz[i][j]);
    }
  }
  return matriz;
}

void imprime_matriz(int** M, int n, int m) {
  printf("\nMatriz: \n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%d ", M[i][j]);
    }
    printf("\n");
  }
}

int main() {
  int nlines, ncols;
  printf("Digite o numero de linhas e colunas: ");
  scanf("%d %d", &nlines, &ncols);

  int** matriz = le_matriz(nlines, ncols);

  imprime_matriz(matriz, nlines, ncols);

  for (int i = 0; i < nlines; i++) {
    free(matriz[i]);
  }
  free(matriz);

  return 0;
}