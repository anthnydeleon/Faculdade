#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int confere_parenteses(char string[], int pos, int abriu, int tamanho) {
    if (abriu < 0) return 0;
    if (string[pos] == '(') abriu++;
    if (string[pos] == ')') abriu--;

    if (pos == tamanho) { 
      if (abriu == 0) return 1;
      else return 0;      
    }

    confere_parenteses(string, pos+1, abriu, tamanho);
}


int main () {
  char string[6] = {'(', ')', '(', '(', ')', ')'};

  int result = confere_parenteses(string, 0, 0, 5);
  (result == 1) ? printf("certo"): printf("errado");

  return 0;
}