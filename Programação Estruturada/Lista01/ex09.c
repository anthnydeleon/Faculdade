#include <stdio.h>
// Questão 9. Escreva um programa em C que faça o produto de Hadamard de dois vetores(u1,u2,...,un)
// e v =(v1,v2,...,vn), isso é, que compute o vetor (u1v1,u2v2,...,unvn). A entrada consiste primeiro
// do valor n, onde 1 ≤ n ≤100, seguida dos n números reais do vetor u, que é seguida pelos n números
// reais do vetor v

void readVector(double vector[], int length) {
  printf("Digite o vetor: ");
  for (int i = 0; i < length; i++) {
    scanf("%lf", &vector[i]);
  }
}

void printVector(double vector[], int length) {
  for (int i = 0; i < length; i++) {
    printf("%.2lf ", vector[i]);
  }
}

double produtoHadamard(double u[], double v[], double result[], int length) {
  for (int i = 0; i < length; i++) {
    result[i] = u[i] * v[i];
  }
}


int main() {
  int length;
  printf("Digite o tamanho do vetor: ");
  scanf("%d", &length);

  double u[length], v[length], result[length];

  readVector(u, length);
  readVector(v, length);
  
  produtoHadamard(u, v, result, length);
  printVector(result, length);

  return 0;
}