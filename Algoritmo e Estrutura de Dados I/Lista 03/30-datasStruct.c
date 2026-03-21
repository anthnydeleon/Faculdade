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

int is_valid(Data dt) {
    if (dt.ano < 0) return 0;
    if (dt.mes < 1 || dt.mes > 12) return 0;
    if (dt.dia < 1) return 0;

    int dias_no_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(dt.ano)) dias_no_mes[1] = 29;

    if (dt.dia > dias_no_mes[dt.mes - 1]) return 0;

    return 1;
}

int main() {
    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        Data dt;
        scanf("%d %d %d", &dt.dia, &dt.mes, &dt.ano);
        if (is_valid(dt)) {
            printf("DATA VALIDA\n");
        } else {
            printf("DATA INVALIDA\n");
        }
    }
    return 0;
}