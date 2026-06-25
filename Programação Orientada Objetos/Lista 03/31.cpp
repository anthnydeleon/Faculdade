#include <iostream>
using namespace std;

class Complexo {
public:
    int real, imag;
    Complexo(int a, int b);
    void soma(Complexo other);
    void sub(Complexo other);
    void mult(Complexo other);
    void div(Complexo other);
};

Complexo::Complexo(int a, int b) {
    this->real = a;
    this->imag = b;
}

void Complexo::soma(Complexo other) {
    this->real = this->real + other.real;
    this->imag = this->imag + other.imag;
}

void Complexo::sub(Complexo other) {
    this->real = this->real - other.real;
    this->imag = this->imag - other.imag;
}

void Complexo::mult(Complexo other) {
    int r = this->real * other.real - this->imag * other.imag;
    int i = this->real * other.imag + this->imag * other.real;
    this->real = r;
    this->imag = i;
}

void Complexo::div(Complexo other) {
    int denom = other.real * other.real + other.imag * other.imag;
    int r = (this->real * other.real + this->imag * other.imag) / denom;
    int i = (this->imag * other.real - this->real * other.imag) / denom;
    this->real = r;
    this->imag = i;
}

int main() {
    int a, b, c, d;
    char bi, di, op;

    while (cin >> a >> b >> bi >> op >> c >> d >> di) {
        Complexo m1(a, b);
        Complexo m2(c, d);

        switch (op) {
            case '+': m1.soma(m2); break;
            case '-': m1.sub(m2);  break;
            case '*': m1.mult(m2); break;
            case '/': m1.div(m2);  break;
        }

        cout << m1.real << " + " << m1.imag << "i\n";
    }

    return 0;
}