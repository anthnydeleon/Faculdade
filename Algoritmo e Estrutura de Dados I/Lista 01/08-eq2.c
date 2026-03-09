#include <stdio.h>
#include <math.h>


int main() {
  int a, b, c, delta;
  double x1, x2;
  scanf("%d %d %d", &a, &b, &c);

  delta = pow(b, 2) - 4*a*c;
  x1 = (-b + sqrt(delta)) / (2*a);
  x2 = (-b - sqrt(delta)) / (2*a);

  printf("%.4f %.4f\n", x1, x2);

  return 0;
}