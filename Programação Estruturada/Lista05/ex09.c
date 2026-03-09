#include <stdio.h>
#include <stdlib.h>

// Questão 9. Implemente as funções:
// • matriz_le(n, m) essa função lê e retorna uma matriz M de números inteiros fornecida pelo
// usuário. Ela recebe dois parâmetros n e m, onde n é o número de linhas e m, o de colunas. Essa
// função é responsável por alocar a memória para M e por ler todo o conteúdo de M, que deverá ser
// fornecido pelo usuário.
// • matriz_print(M, n, m) essa função imprime na tela do usuário a matriz M que tem n linhas
// e m colunas.

int** matriz_le(int n, int m) {
  int** M = calloc(n, sizeof(int*)); 
  for (int i = 0; i < n; i++) M[i] = calloc(m, sizeof(int));
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      scanf("%d", &M[i][j]); 
    }
  }

  return M;
}

void imprime_matriz(int** M, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      printf("%d ", M[i][j]);
    }
    printf("\n");
  }
}

int main() {
  int n = 3, m = 2;
  int** matriz = matriz_le(n, m);
  imprime_matriz(matriz, n, m);

  for (int i = 0; i < n; i++) {
    free(matriz[i]);
  }
  free(matriz);

  return 0;
}