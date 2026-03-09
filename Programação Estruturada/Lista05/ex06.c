#include <stdio.h>
#include <stdlib.h>
// Questão 6. Implemente a função strcat(a, b). Essa função recebe dois parâmetros a e b do tipo
// “string em C”, i.e., array de caracteres, e retorna uma string que é a concatenação da string a com a
// string b. Além disso, a sua função não deve alterar o conteúdo das variáveis a e b, que ainda podem
// ser úteis para o cliente da sua função.
#define MAX 100

int strlen(char* a) {
  int i = 0;
  while (a[i] != '\0') i++;
  
  return i;
}

char* strcat(char* a, char* b) {
  int i = 0;
  int len_a = strlen(a);
  int len_b = strlen(b);
  int len = len_a+len_b;

  char* string = calloc(len, sizeof(char));

  for (i; i < len_a; i++) string[i] = a[i];
  for (int j = 0; i < len; i++, j++) string[i] = b[j]; 

  return string;
}


int main() {
  char* a = calloc(MAX, sizeof(char));
  char* b = calloc(MAX, sizeof(char));

  a = "ola";
  b = " teste";

  char* string = strcat(a, b);

  printf("%s", string);

  free(a);
  free(b);
  free(string);
  
  return 0;
}