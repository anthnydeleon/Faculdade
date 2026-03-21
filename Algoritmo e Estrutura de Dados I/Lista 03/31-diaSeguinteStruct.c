#include <stdio.h>
#include <stdlib.h>

typedef struct tDate {
    int day, month, year;
    int next_day, next_month, next_year;
} Date;

int is_leap_year (int year) {
  if(year % 400 == 0) return 1;
  if(year % 100 == 0) return 0;
  if(year % 4 == 0) return 1;
  return 0;
}

void calculate_next_day(Date *dt) {
    int days_on_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(dt->year)) days_on_months[1] = 29;

    dt->next_day = dt->day + 1;
    dt->next_month = dt->month;
    dt->next_year = dt->year;

    if (dt->next_day > days_on_months[dt->month - 1]) {
        dt->next_day = 1;
        dt->next_month++;
        if (dt->next_month > 12) {
            dt->next_month = 1;
            dt->next_year++;
        }
    }
}

int main() {
    Date dt;
    scanf("%d %d %d", &dt.day, &dt.month, &dt.year);

    calculate_next_day(&dt);
    printf("%d %d %d", dt.next_day, dt.next_month, dt.next_year);

    return 0;
}