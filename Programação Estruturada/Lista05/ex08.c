#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 8. Implemente a função char* revert(char* s). Essa função recebe uma “string de C”
// s e retornar uma nova “string de C” com os caracteres de s revertidos, i.e., listados da direita para a
// esquerda.

char* revert(char* string) {
  int len = strlen(string);
  char* string_revertida = calloc(len, sizeof(char));

  for (int i = 0; i < len; i++) {
    string_revertida[len-i-1] = string[i];
  }
  
  return string_revertida;
}


#define MAX 100
int main() {
  char string[MAX];
  printf("Digite a string: ");
  scanf("%[^\n]", string);
  getchar();

  char* string_invertida = revert(string);

  printf("String invertida: %s", string_invertida);

  return 0;
}