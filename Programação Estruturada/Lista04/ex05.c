#include <stdio.h>
// Questão 5. Desenvolva um programa que reformate os parágrafos de um texto, garantindo que nenhuma linha exceda 80 caracteres.

#define MAX 1000

void concatenar_texto(char texto[]) {
  for (int i = 0; i < MAX; i++) {
    if (texto[i] == '\n') {
      texto[i] = ' ';
    }
  }
}

void formatar_texto(char texto[]) {
  concatenar_texto(texto);
  int i = 0;
  int contador = 0;

  while (texto[i] != '\0') {
    if (contador == 80) {
      int j = i - 1;
      while (j >= 0 && texto[j] != ' ') {
        j--;
      }
      
      if (j >= 0) {
        texto[j] = '\n';
        contador = 0;
      }
    }
    i++;
    contador++;
  }
  
  // Exibe o texto formatado
  printf("%s\n", texto);
}

int main() {
  char texto[MAX];
  scanf("%[^\0]", texto);

  formatar_texto(texto);

  return 0;
}