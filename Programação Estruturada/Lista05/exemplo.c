#include <stdio.h>

int square(int a) {
return a * a;
}

int cube(int a) {
return a * a * a;
}

void map(int array[], int n, int func(int)) {
  for (int i = 0; i < n; i++)
  array[i] = func(array[i]);
}

int main(int argc, char *argv[]) {
  int vet[5] = {1, 2, 3, 4, 5};
  int vet2[5] = {1, 2, 3, 4, 5};

  map(vet, 5, square);
  map(vet2, 5, cube);
  
  for (int i = 0; i < 5; i++) {
    printf("%d\n", vet[i]);
  }

  for (int i = 0; i < 5; i++) {
    printf("%d\n", vet2[i]);
  }
    
  return 0;
}