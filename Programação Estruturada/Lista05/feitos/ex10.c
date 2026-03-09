#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int* data;
  int capacity;
  int nelements;
}* VectorInt;

VectorInt vectorint(void) {
  VectorInt vec = malloc(sizeof(*vec));
  vec->data = malloc(sizeof(int));
  vec->capacity = 1;
  vec->nelements = 0;
  return vec;
}

void vectorint_insert(VectorInt v, int a) {
  if (v->nelements == v->capacity) {
    int new_capacity = v->capacity * 2;
    int* new_data = realloc(v->data, new_capacity * sizeof(int));
    v->data = new_data;
    v->capacity = new_capacity;
  }
  
  v->data[v->nelements] = a;
  v->nelements++;
}

void vectorint_remove(VectorInt v, int a) {
  for (int i = 0; i < v->nelements; i++) {
    if (v->data[i] == a) {
      for (int j = i; j < v->nelements - 1; j++) {
        v->data[j] = v->data[j + 1];
      }
      v->nelements--;
      i--;
    }
  }

  if (v->nelements < v->capacity / 2 && v->capacity > 1) {
    int new_capacity = v->capacity / 2;
    int* new_data = realloc(v->data, new_capacity * sizeof(int));
    v->data = new_data;
    v->capacity = new_capacity;
  }
}

int vectorint_at(VectorInt v, int i) {
  return v->data[i];
}

int quantidade_de_elementos(VectorInt v) {
  return v->nelements;
}

void verify_if_contains(VectorInt v, int a) {
  int verify = 0;
  for (size_t i = 0; i < v->nelements; i++) {
    if (v->data[i] == a) {
      verify = 1;
      break;
    }
  }
  (verify) ? printf("O numero %d esta no vetor!\n", a) : printf("O numero %d nao esta no vetor!\n", a);
}

void free_vector_memory(VectorInt v) {
  free(v->data);
  free(v);
}

int main() {
  VectorInt vec = vectorint();

  vectorint_insert(vec, 10);
  vectorint_insert(vec, 20);
  vectorint_insert(vec, 30);
  vectorint_insert(vec, 40);
  printf("Vetor apos insercoes: ");
  for (int i = 0; i < quantidade_de_elementos(vec); i++) {
      printf("%d ", vectorint_at(vec, i));
  }
  printf("\n");

  vectorint_remove(vec, 20);
  printf("Vetor apos remocao: ");
  for (int i = 0; i < quantidade_de_elementos(vec); i++) {
      printf("%d ", vectorint_at(vec, i));
  }
  printf("\n");

  verify_if_contains(vec, 65);
  printf("Quantidade de elementos: %d", quantidade_de_elementos(vec));

  free_vector_memory(vec);

  return 0;
}