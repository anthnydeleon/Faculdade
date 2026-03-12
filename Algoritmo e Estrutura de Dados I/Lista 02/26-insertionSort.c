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

void insertionSort(int *v, int n) {
  int displacement = 0;
  for (int i = 1; i < n; i++) {
    int key = v[i];
    int j = i - 1;
    while (j >= 0 && v[j] > key) {
      v[j + 1] = v[j];
      print_vector(v, n);
      displacement++;
      j--;
    }
    v[j + 1] = key;
    print_vector(v, n);
  }

  printf("Deslocamentos: %d", displacement);
}

int main() {
    int n;
    scanf("%d", &n);
    int *v = malloc(n * sizeof(int));
    read_vector(v, n);
    insertionSort(v, n);

    free(v);
  return 0;
}