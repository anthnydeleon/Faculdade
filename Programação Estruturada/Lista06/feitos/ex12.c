#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 12. O Merge Sort é um algoritmo de ordenação que segue o paradigma dividir para conquistar. O algoritmo funciona da seguinte forma:
// 1. Dividir: O vetor original com n elementos é divido em dois subvetores, um de tamanho [n/2] e outro de tamanho ⌊n/2⌋.
// 2. Conquistar: Os subvetores são ordenados recursivamente.
// 3. Combinar: Os subvetores ordenados são combinados (intercalados) em um único vetor ordenado.

void merge_sort(int* v, int n) {
  if (n < 2) return;

  int mid = n / 2;
  int* a = malloc(mid * sizeof(int));
  int* b = malloc((n - mid) * sizeof(int));
  
  for (int i = 0; i < mid; i++) a[i] = v[i];
  for (int i = mid; i < n; i++) b[i - mid] = v[i];

  merge_sort(a, mid);
  merge_sort(b, n - mid);

  int i = 0, j = 0, k = 0;
  while (i < mid && j < n - mid) v[k++] = (a[i] < b[j]) ? a[i++] : b[j++];
  while (i < mid) v[k++] = a[i++];
  while (j < n - mid) v[k++] = b[j++];

  free(a);
  free(b);
}

int main() {
  int vetor[7] = {38, 27, 43, 3, 9, 82, 10};
  int n = 7;
  
  merge_sort(vetor, n);
  printf("Vetor ordenado: ");
  for (int i = 0; i < n; i++) {
    printf("%d ", vetor[i]);
  }

  return 0;
}