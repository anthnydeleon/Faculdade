#include <stdio.h>

void read_matrix(int n, int matrix[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%d", &matrix[i][j]);
    }
  }
}

void print_matrix(int n, int matrix[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

void sum_matrix(int n, int A[n][n], int B[n][n], int result[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      result[i][j] = A[i][j] + B[i][j];
    }
  }
}

int main() {
  int n;
  scanf("%d", &n);

  int A[n][n], B[n][n], result[n][n];
  read_matrix(n, A);
  read_matrix(n, B);
  sum_matrix(n, A, B, result);
  print_matrix(n, result);
    
  return 0;
}