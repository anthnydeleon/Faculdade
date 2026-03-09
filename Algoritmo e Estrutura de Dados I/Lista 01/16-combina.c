#include <stdio.h>
#include <stdlib.h>

int * combina (int *v, int *w, int n, int m) {
  int *z = malloc((n + m) * sizeof(int));
  for (int i = 0; i < n; i++) {
    z[i] = v[i];
  }
  for (int i = 0; i < m; i++) {
    z[n + i] = w[m - 1 - i];
  }
  return z;
}

void read_vector(int *vec, int size) {
  for (int i = 0; i < size; i++) {
    scanf("%d", &vec[i]);
  }
}

void print_vector(int *vec, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", vec[i]);
  }
  printf("\n");
}

int main() {
  int n, m;
  scanf("%d %d", &n, &m);
  int *v = malloc(n * sizeof(int));
  int *w = malloc(m * sizeof(int));
  
  read_vector(v, n);
  read_vector(w, m);
  int *z = combina(v, w, n, m);
  print_vector(z, n + m);

  free(v);
  free(w);
  free(z);
  
  return 0;
}