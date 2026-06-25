#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

typedef struct tCliente {
    char nome[50];
    int cpf;
    int ano, mes, dia;
} Cliente;

int compara_cliente(Cliente *a, Cliente *b, int tipo) {  
  if (tipo == 1) return strcmp(a->nome, b->nome);
  if (tipo == 2) return b->cpf - a->cpf;
  if (a->ano != b->ano) return a->ano - b->ano;
  if (a->mes != b->mes) return a->mes - b->mes;
  return a->dia - b->dia;
}

void imprime_clientes(Cliente *arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%s", arr[i].nome);
    if (i < n - 1) {
      printf(" ");
    }
  }
  printf("\n");
}

void bubble_sort(Cliente *array, int n, int tipo) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++) {
      if (compara_cliente(&array[j], &array[j + 1], tipo) > 0) {
        Cliente tmp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = tmp;
      }
    }
  }
}

int main() {
  int n, tipo;
  scanf("%d %d", &n, &tipo);

  Cliente clientes[n];
  
  for (int i = 0; i < n; i++) {
    scanf("%s %d %d %d %d", clientes[i].nome, &clientes[i].cpf, &clientes[i].dia, &clientes[i].mes, &clientes[i].ano);
  }

  bubble_sort(clientes, n, tipo);
  imprime_clientes(clientes, n);

  return 0;
}