#include <stdio.h>
#define MAX 100

void imprime_matriz(int M[][MAX], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) { 
      printf("%d", M[i][j]);
    }
    printf("\n");
  }
}

void build_matriz(int M[][MAX], int n) {
  for (int i = 0; i < n; i++){
    for (int j = 0; j < n; j++){
      M[i][j] = 0;
    }
  }

  int count = 1;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (M[i][j] == 0) M[i][j] = count;
      if (M[n-1-i][j] == 0) M[n-1-i][j] = count;
      if (M[j][i] == 0) M[j][i] = count;
      if (M[j][n-1-i] == 0) M[j][n-1-i] = count;
    }
    count++;
  }
}

int main() {
  int n;
  printf("Digite a quantidade de elementos: ");
  scanf("%d", &n);

  int matriz[MAX][MAX];
  build_matriz(matriz, n);
  imprime_matriz(matriz, n);

  return 0;
}