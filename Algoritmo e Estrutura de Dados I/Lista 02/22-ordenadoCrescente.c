#include <stdio.h>
#include <stdlib.h>

int is_sorted(int *v, int n) {
  int num = v[0];
  
  for (int i = 1; i < n; i++) {
    if (num >= v[i]) {
      return 0;
    }
    num = v[i];
  }
  return 1;
}

void read_vector(int *vec, int size) {
  for (int i = 0; i < size; i++) {
    scanf("%d", &vec[i]);
  }
}

int main() {
  int n;
  while (scanf("%d", &n) != EOF) {
    int *v = malloc(n * sizeof(int)); 
    read_vector(v, n); 
    
    if (is_sorted(v, n)) {
      printf("S\n");
    } else {
      printf("N\n");
    }

    free(v);
  }  
  return 0;
}