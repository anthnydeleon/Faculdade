#include <stdio.h>

void read_matrix(int n, int matrix[n][n]) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      scanf("%d", &matrix[i][j]);
    }
  }
}

double sum_below_diagonal(int n, int A[n][n]) {
  double sum = 0.0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
        sum += (double) A[i][j];
    }
  }
  return sum;
}

double avg_below_diagonal(int n, int A[n][n]) {
  double avg = 0.0;
  int count = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      avg += (double) A[i][j];
      count++;
    }
  }
  return avg / (double) count;
}

int main() {
  char operation;
  int n;
  scanf("%c", &operation);
  scanf("%d", &n);
  
  int A[n][n];
  double result;
  read_matrix(n, A);
  
  if (operation == 'S') {
    result = sum_below_diagonal(n, A);
  } else {
    result = avg_below_diagonal(n, A);
  }

  printf("%.1f", result);
  return 0;
}