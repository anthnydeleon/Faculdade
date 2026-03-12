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

void selectionSort(int *v, int n) {
  int count = 0;
  for (int i = 0; i < n-1; i++) {
    int min = i;
    for (int j = i+1; j < n; j++) {
      count++;
      if (v[j] < v[min]) {
        min = j;
      }
    }

      int temp = v[i];
      v[i] = v[min];
      v[min] = temp;
      print_vector(v, n);
  }

  printf("Total de IFs executados: %d", count);
}

int main() {
    int n;
    scanf("%d", &n);
    int *v = malloc(n * sizeof(int));
    read_vector(v, n);
    selectionSort(v, n);

    free(v);
  return 0;
}