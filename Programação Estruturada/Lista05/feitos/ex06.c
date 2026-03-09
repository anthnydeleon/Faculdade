#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 6. Implemente a função strcat(a, b). Essa função recebe dois parâmetros a e b do tipo
// “string em C”, i.e., array de caracteres, e retorna uma string que é a concatenação da string a com a
// string b. Além disso, a sua função não deve alterar o conteúdo das variáveis a e b, que ainda podem
// ser úteis para o cliente da sua função.

#define MAX 100

int tamanho_string(char* string) {
  int i = 0;
  while(string[i] != '\0') i++;
  return i;
}

char* concatenar_string(char* a, char* b) {
  int n = tamanho_string(a); 
  int m = tamanho_string(b);
  
  char* result = calloc(n+m+1, sizeof(char));

  strcpy(result, a);
  strcat(result, b);

  return result;
}

int main() {
  char a[MAX];
  char b[MAX];

  printf("Digite a primeira string: ");
  scanf("%[^\n]", a);
  getchar();

  printf("Digite a segunda string: ");
  scanf("%[^\n]", b); 
  getchar();

  char* concat = concatenar_string(a, b);
  printf("%s", concat);
  free(concat);

  return 0;
}