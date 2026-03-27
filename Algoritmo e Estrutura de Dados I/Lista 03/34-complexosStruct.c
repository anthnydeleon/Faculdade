#include <stdio.h>

typedef struct {
    int a;
    int b;
} tComplex;

tComplex cria(int a, int b) {
    tComplex c = {a, b};
    return c;
}

tComplex soma(tComplex x, tComplex y) {
    return cria(x.a + y.a, x.b + y.b);
}

tComplex subtrai(tComplex x, tComplex y) {
    return cria(x.a - y.a, x.b - y.b);
}

tComplex multiplica(tComplex x, tComplex y) {
    return cria(x.a * y.a - x.b * y.b,
                 x.a * y.b + x.b * y.a);
}

tComplex divide(tComplex x, tComplex y) {
    int den = y.a * y.a + y.b * y.b;
    return cria((x.a * y.a + x.b * y.b) / den,
                (x.b * y.a - x.a * y.b) / den);
}

int main() {
    int a1, b1, a2, b2;
    char op;

    while (scanf("%d %di %c %d %di", &a1, &b1, &op, &a2, &b2) == 5) {
        tComplex m1 = cria(a1, b1);
        tComplex m2 = cria(a2, b2);
        tComplex r;

        if (op == '+') r = soma(m1, m2);
        else if (op == '-') r = subtrai(m1, m2);
        else if (op == '*') r = multiplica(m1, m2);
        else r = divide(m1, m2);

        if (r.b >= 0)
            printf("%d %di\n", r.a, r.b);
        else
            printf("%d -%di\n", r.a, -r.b);
    }

    return 0;
}