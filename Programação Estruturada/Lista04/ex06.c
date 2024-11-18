#include <stdio.h>
// Questão 6. Crie o tipo de dados Conjunto e implemente as seguintes funções:
// • Conjunto conjunto(int vet[]) cria um registro conjunto (Conjunto) no qual os elementos são iguais ao do vetor vet.
// • Conjunto conjunto_uniao(Conjunto A, Conjunto B) retorna o conjunto obtido pela união do conjuntos A e B.
// • char conjunto_pertence(int a, Conjunto A) retorna 1 se a pertence ao conjunto A e 0, caso contrário

#define MAX 10

typedef struct {
  int elementos[MAX];
  int tamanho;
} Conjunto;
  
Conjunto conjunto(int vet[], int n) {
  Conjunto C;
  C.tamanho = n;

  for (int i = 0; i < n; i++) {
    C.elementos[i] = vet[i];
  }
  return C;
} 

char conjunto_pertence(int num, Conjunto C) {
  for (int i = 0; i < C.tamanho; i++) {
    if (num == C.elementos[i]) return 1;
  }
  return 0;
}

Conjunto conjunto_uniao(Conjunto A, Conjunto B) {
  Conjunto C;
  C.tamanho = 0;

  for (int i = 0; i < A.tamanho; i++) {
    C.elementos[C.tamanho++] = A.elementos[i];  
  }

  for (int i = 0; i < B.tamanho; i++) {
    if (!conjunto_pertence(B.elementos[i], A)) C.elementos[C.tamanho++] = B.elementos[i];
  }

  return C;
}

void imprimir_conjunto(Conjunto C) {
  printf("{ ");
  for (int i = 0; i < C.tamanho; i++) {
    printf("%d ", C.elementos[i]);
  }
  printf("}\n");
}

int main() {
  int vetA[] = {1, 2, 3, 4, 5, 6};
  int vetB[] = {2, 4, 8, 6, 2};

  Conjunto A = conjunto(vetA, 6);
  Conjunto B = conjunto(vetB, 5);
  Conjunto Uniao = conjunto_uniao(A, B);

  printf("Conjunto A: ");
  imprimir_conjunto(A);

  printf("Conjunto B: ");
  imprimir_conjunto(B);

  printf("Conjunto Uniao: ");
  imprimir_conjunto(Uniao);
  
  return 0;
}