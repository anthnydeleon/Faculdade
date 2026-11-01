#include <iostream>
using namespace std;

class Racional {
    private:
        // int numerador, denominador;
    public:
        int numerador, denominador;
        Racional(int,int);
        void negativo(); // Torna negativo o objeto que invocou o método
        void soma(Racional); // Recebe um racional e efetua a adição entre o objeto do argumento com o objeto que invocou o método.
        void subtracao(Racional); // Recebe um racional e efetua a subtração entre o objeto do argumento com o objeto que invocou o método.
        void produto(Racional); // Recebe um racional e efetua o produto entre o objeto do argumento com o objeto que invocou o método.
        void quociente(Racional); // Recebe um racional e efetua o quociente entre o objeto do argumento com o objeto que invocou o método.
        void simplifica (); // Reduz ao máximo o objeto Racional que invocou o método.

};

Racional::Racional(int numerador, int denominador) {
    this->numerador = numerador; 
    this->denominador = denominador;
}

void Racional::negativo() {
    this->numerador = -numerador;    
}

void Racional::soma(Racional r) {
    int a, b, c, d;
    a = this->numerador, b = this->denominador;
    c = r.numerador, d = r.denominador;
    this->numerador = a * d + b * c;
    this->denominador =  b * d;
}

void Racional::subtracao(Racional r) {
    r.negativo();
    soma(r);
}

void Racional::produto(Racional r) {
    this->numerador = this->numerador * r.numerador;
    this->denominador = this->denominador * r.denominador;
}

void Racional::quociente(Racional r) {
    this->numerador = this->numerador * r.denominador;
    this->denominador = this->denominador * r.numerador;
}

void Racional::simplifica() {
    int gcd = 1;
    for (int i = 1; i <= min(abs(numerador), abs(denominador)); i++) {
        if (numerador % i == 0 && denominador % i == 0) {
            gcd = i;
        }
    }
    numerador /= gcd;
    denominador /= gcd;
}

int main() {
    int n, a, b, c, d;
    char operacao;


    while (cin >> a >> b >> operacao >> c >> d) {
        Racional r1(a, b);
        Racional r2(c, d);
        
        switch (operacao) {
        case '+': r1.soma(r2); break;
        case '-': r1.subtracao(r2); break;
        case '*': r1.produto(r2); break;
        case '/': r1.quociente(r2); break;
        }
        r1.simplifica();
        cout << r1.numerador << " " << r1.denominador << endl;
    }

    return 0;
}