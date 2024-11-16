#include <stdio.h>
#define MAX 20

void le_matriz(int M[][MAX], int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      scanf("%d", &M[i][j]);
    }
  }
}

void findLightsaber(int M[][MAX], int n, int m) {
  int status = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (M[i][j] == 42) {
        int isLightsaber = 1;

        for (int k = 0; k < 3; k++) {
          for (int l = 0; l < 3; l++) {
            if (k == 1 && l == 1) continue;
            if (M[k + i - 1][l + j -1] != 7) {
              isLightsaber = 0; 
            }      
          }
        }

        if (isLightsaber == 1) printf("\nSabre encontrado nas coordenadas em i: %d e j: %d", i, j);
        status = isLightsaber;
      }
    }
  }
  if(status == 0) printf("\nNenhum sabre emitiu luz.");
}

int main() {
  int n, m;
  printf("Digite o tamanho da matriz: ");
  scanf("%d %d", &n, &m);

  int M[MAX][MAX];
  le_matriz(M, n, m);
  findLightsaber(M, n, m);

  return 0;
}