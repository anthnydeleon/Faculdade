#include <stdio.h>

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      scanf("%d", &matrix[i][j]);
    }
  }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols], int transposed[cols][rows]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      transposed[j][i] = matrix[i][j];
    }
  }
}

int main() {
  int n, m;
  scanf("%d %d", &n, &m);

  int A[n][m], transposed[m][n];
  read_matrix(n, m, A);
  transpose_matrix(n, m, A, transposed);
  print_matrix(m, n, transposed);

  return 0;
}