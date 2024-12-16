#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Essa função imprime todas as sequências com n bits (sem repetição). A ordem em que as cadeias de
// bits são impressas não importa, apenas tome cuidado para não imprimir a mesma cadeia mais de uma
// vez. O exemplo abaixo mostra uma das saídas possíveis para quando n =3.

#include <stdio.h>

void list_all_bit_strings(int n, char* bit_string, int index) {
  if (index == n) {
    bit_string[index] = '\0';
    printf("%s\n", bit_string);
    return;
  }

  bit_string[index] = '0';
  list_all_bit_strings(n, bit_string, index + 1);

  bit_string[index] = '1';
  list_all_bit_strings(n, bit_string, index + 1);
}

int main() {
  int n = 3;
  char* bit_string = calloc(n+1, sizeof(char));
  list_all_bit_strings(n, bit_string, 0);
  free(bit_string);

  return 0;
}