#include <stdio.h>

typedef struct tRacional {
    int numerador;
    int denominador;
} tRacional;

int mdc(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

tRacional racional(int num, int den) {
    tRacional r;
    r.numerador   = num;
    r.denominador = den;
    return r;
}

tRacional reduz(tRacional r) {
    int m = mdc(r.numerador, r.denominador);
    r.numerador   /= m;
    r.denominador /= m;

    if (r.denominador < 0) {
        r.numerador   = -r.numerador;
        r.denominador = -r.denominador;
    }
    return r;
}

tRacional negativo(tRacional r) {
    return racional(-r.numerador, r.denominador);
}

tRacional soma(tRacional a, tRacional b) {
    int num = a.numerador * b.denominador + b.numerador * a.denominador;
    int den = a.denominador * b.denominador;
    return reduz(racional(num, den));
}

tRacional multiplica(tRacional a, tRacional b) {
    int num = a.numerador * b.numerador;
    int den = a.denominador * b.denominador;
    return reduz(racional(num, den));
}

tRacional divide(tRacional a, tRacional b) {
    return multiplica(a, racional(b.denominador, b.numerador));
}

int main() {
    int n1, d1, n2, d2;
    char op;

    while (scanf("%d %d %c %d %d", &n1, &d1, &op, &n2, &d2) != EOF) {
        tRacional a = racional(n1, d1);
        tRacional b = racional(n2, d2);
        tRacional resultado;

        if (op == '+') resultado = soma(a, b);
        else if (op == '-') resultado = soma(a, negativo(b));
        else if (op == '*') resultado = multiplica(a, b);
        else resultado = divide(a, b);

        printf("%d/%d\n", resultado.numerador, resultado.denominador);
    }
    return 0;
}