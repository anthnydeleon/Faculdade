#include <stdio.h>

int main() {
  int duracaoEvento;
  scanf("%d", &duracaoEvento);

  int horas, minutos, segundos;

  horas = duracaoEvento / 3600;
  minutos = (duracaoEvento % 3600) / 60;
  segundos = duracaoEvento % 60;

  printf("%d:%d:%d", horas, minutos, segundos);
  return 0;
}