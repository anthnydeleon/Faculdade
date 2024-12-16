#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 4. Implemente a função recursiva chamada inverte. Um dos parâmetros que essa função
// irá receber é char word[]. Você pode passar outros parâmetros se sentir necessidade. Como resul-
// tado, essa função deve inverter a palavra recebida, i.e., se a word armazenar a palavra “abobora”, após
// a execução da função, ela armazenará a palavra “aroboba”.

void inverte_palavra(char word[], int inicio, int fim) {
  if (inicio >= fim) return;

  char temp = word[inicio];
  word[inicio] = word[fim];
  word[fim] = temp;

  inverte_palavra(word, inicio + 1, fim - 1);
}

void inverte(char word[]) {
  int len = strlen(word);
  inverte_palavra(word, 0, len - 1);
}

int main() {
  char word[100];
  scanf("%[^\n]", word);

  inverte(word);
  printf("%s", word);

  return 0;
}