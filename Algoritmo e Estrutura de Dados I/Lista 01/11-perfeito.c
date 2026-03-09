#include <stdio.h>

int sumDivisors(int n) {
  int sum = 0;
  for (int i = 1; i < n; i++) {
    if (n % i == 0) {
      sum += i;
    }
  }
  return sum;
}

int main() {
  int n, sum = 0;
  scanf("%d", &n);

  sum = sumDivisors(n);

  if (sum == n) {
    printf("SIM");
  } else {
    printf("NAO");
  }
  return 0;
}