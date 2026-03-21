#include <stdio.h>

typedef struct tComplex {
    int a;
    int b_i;
} tComplex;


tComplex soma(tComplex a, tComplex b) {
    return (tComplex){a.a + b.a, a.b_i + b.b_i};
}

tComplex subtrai(tComplex a, tComplex b) {
    return (tComplex){a.a - b.a, a.b_i - b.b_i};
}

tComplex multiplica(tComplex a, tComplex b) {
    return (tComplex){a.a * b.a - a.b_i * b.b_i, a.a * b.b_i + a.b_i * b.a};
}

tComplex divide(tComplex a, tComplex b) {
    int den = b.a * b.a + b.b_i * b.b_i;
    return (tComplex){(a.a * b.a + a.b_i * b.b_i) / den, (a.b_i * b.a - a.a * b.b_i) / den};
}

int main() {
    int a1, b1, a2, b2;
    char op;
    
    while (scanf("%d %d %c %d %d", &a1, &b1, &op, &a2, &b2) == 5) {
        tComplex a = (tComplex){a1, b1};
        tComplex b = (tComplex){a2, b2};
        tComplex resultado;

        if (op == '+') resultado = soma(a, b);
        else if (op == '-') resultado = subtrai(a, b);
        else if (op == '*') resultado = multiplica(a, b);
        else resultado = divide(a, b);

        printf("%d + %di\n", resultado.a, resultado.b_i);
    }

    return 0;
}