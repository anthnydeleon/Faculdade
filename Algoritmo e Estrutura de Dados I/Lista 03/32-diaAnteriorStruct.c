#include <stdio.h>
#include <stdlib.h>

typedef struct tDate {
    int day, month, year;
    int previous_day, previous_month, previous_year;
} Date;

int is_leap_year (int year) {
  if(year % 400 == 0) return 1;
  if(year % 100 == 0) return 0;
  if(year % 4 == 0) return 1;
  return 0;
}

void calculate_previous_day(Date *dt) {
    int days_on_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(dt->year)) days_on_months[1] = 29;

    dt->previous_day = dt->day - 1;
    dt->previous_month = dt->month;
    dt->previous_year = dt->year;

    if (dt->previous_day < 1) {
        dt->previous_month--;
        if (dt->previous_month < 1) {
            dt->previous_month = 12;
            dt->previous_year--;
        }
        dt->previous_day = days_on_months[dt->previous_month - 1];
    }
}

int main() {
    Date dt;
    scanf("%d %d %d", &dt.day, &dt.month, &dt.year);

    calculate_previous_day(&dt);
    printf("%d %d %d", dt.previous_day, dt.previous_month, dt.previous_year);

    return 0;
}