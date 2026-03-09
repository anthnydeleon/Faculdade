#include <stdio.h>

int main() {
  int num;
  scanf("%d", &num);
  
  if (num < 0) {
    printf("nenhum numero informado");
    return 0;
  }

  int menor = num;
  while (num >= 0) {
    if (num < menor) {
      menor = num;
    }
    scanf("%d", &num);
  }
  
  printf("%d", menor);
  
  return 0;
}