#include <stdio.h>
#include <stdlib.h>

typedef struct tData {
    int dia, mes, ano;
} Data;

int is_leap_year (int year) {
  if(year % 400 == 0) return 1;
  if(year % 100 == 0) return 0;
  if(year % 4 == 0) return 1;
  return 0;
}
int is_valid(Data dt);

int main() {

    return 0;
}