#include <stdio.h>
#include <stdlib.h>

int is_leap_year(int year) {
  if(year % 400 == 0) return 1;
  if(year % 100 == 0) return 0;
  if(year % 4 == 0) return 1;
  
  return 0;
}

int main() {
  int year;
  scanf("%d", &year);

  is_leap_year(year) ? printf("ANO BISSEXTO\n") : printf("ANO NAO BISSEXTO\n");
  return 0;
}