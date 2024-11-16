#include <stdio.h>
// Questão 10. Escreva um programa em C que leia uma sequência de n números inteiros e imprima os
// seguintes valores: o número mínimo da sequência, o máximo e a média. Na sua implementação, você
// deve dividir o seu programa nas seguintes funções


void readVector(int vector[], int length) {
  for (int i = 0; i < length; i++) {
    scanf("%d", &vector[i]);
  }
}

int minimum(int vector[], int length) {
  int aux = vector[0];
  for (int i = 1; i < length; i++) {
    if (vector[i] < aux) aux = vector[i];
  }

  return aux;
}

int maximum(int vector[], int length) {
  int aux = vector[0];
  for (int i = 1; i < length; i++) {
    if (vector[i] > aux) aux = vector[i];
  }

  return aux;
}

double average(int vector[], int length) {
  double aux = 0;
  for (int i = 0; i < length; i++) {
    aux += (double) vector[i]/length;
  }

  return aux;
}

int main() {
  int length, min, max;
  double avg;
  printf("Digite o tamanho do vetor: ");
  scanf("%d", &length);

  int vector[length];
  readVector(vector, length);

  min = minimum(vector, length);
  max = maximum(vector, length);
  avg = average(vector, length);

  printf("Minimo: %d, maximo: %d, media: %.2lf", min, max, avg);
  return 0;
}