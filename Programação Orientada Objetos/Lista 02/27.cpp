#include <iostream>
#include <string>
using namespace std;

struct tComplex {
    int real;
    int imag;
};

struct tComplex complexo(int a, int b) {
    struct tComplex c;
    c.real = a;
    c.imag = b;
    return c;
}

struct tComplex soma(struct tComplex m1, struct tComplex m2) {
    return complexo(m1.real + m2.real, m1.imag + m2.imag);
}

struct tComplex sub(struct tComplex m1, struct tComplex m2) {
    return complexo(m1.real - m2.real, m1.imag - m2.imag);
}

struct tComplex mult(struct tComplex m1, struct tComplex m2) {
    int r = m1.real * m2.real - m1.imag * m2.imag;
    int i = m1.real * m2.imag + m1.imag * m2.real;
    return complexo(r, i);
}

struct tComplex divid(struct tComplex m1, struct tComplex m2) {
    int denom = m2.real * m2.real + m2.imag * m2.imag;
    int r = (m1.real * m2.real + m1.imag * m2.imag) / denom;
    int i = (m1.imag * m2.real - m1.real * m2.imag) / denom;
    return complexo(r, i);
}

void print(struct tComplex c) {
    if (c.imag >= 0)
        cout << c.real << " +" << c.imag << "i" << endl;
    else
        cout << c.real << " " << c.imag << "i" << endl;
}

int main() {
    int a, b, c, d;
    char bi, di, op;

    while (cin >> a >> b >> bi >> op >> c >> d >> di) {
        struct tComplex m1 = complexo(a, b);
        struct tComplex m2 = complexo(c, d);
        struct tComplex result;

        switch (op) {
            case '+': result = soma(m1, m2);  break;
            case '-': result = sub(m1, m2);   break;
            case '*': result = mult(m1, m2);  break;
            case '/': result = divid(m1, m2); break;
        }

        print(result);
    }

    return 0;
}