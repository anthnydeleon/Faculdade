#include <stdio.h>

int main() {
    long long a, b;
    scanf("%lld %lld", &a, &b);

    if (b == 0) return 1;
    printf("%lld %lld", a/b, a%b);

    return 0;
}