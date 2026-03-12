#include <stdio.h>
#include <stdlib.h>

void read_vector(int *v, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
}

void print_vector(int *v, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void bubbleSort(int *v, int n) {
  int i, j, temp;
  int swapped = 0, count = 0;
  for (i = 0; i < n-1; i++) {
      for (j = 0; j < n-i-1; j++) {
          count++;
          if (v[j] > v[j+1]) {
              temp = v[j];
              v[j] = v[j+1];
              v[j+1] = temp;
              swapped++;
              print_vector(v, n);
          }
      }
  }
  
  printf("Total de IFs executados: %d\n", count);
  printf("Total de IFs satisfeitos: %d\n", swapped);
}

int main() {
    int n;
    scanf("%d", &n);
    int *v = malloc(n * sizeof(int));
    read_vector(v, n);
    bubbleSort(v, n);

    free(v);
  return 0;
}