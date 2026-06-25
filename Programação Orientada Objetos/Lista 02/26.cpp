#include <iostream>
using namespace std;

struct tRacional {
  int numerador;
  int denominador;
};

struct tRacional negativo(tRacional r) {
  tRacional resultado;
  resultado.numerador = -r.numerador;
  resultado.denominador = r.denominador;
  return resultado;
}

struct tRacional soma(tRacional a, tRacional b) {
  tRacional resultado;
  resultado.numerador = a.numerador * b.denominador + b.numerador * a.denominador;
  resultado.denominador = a.denominador * b.denominador;
  return resultado;
}

struct tRacional mult(tRacional a, tRacional b) {
  tRacional resultado;
  resultado.numerador = a.numerador * b.numerador;
  resultado.denominador = a.denominador * b.denominador;
  return resultado;
}

struct tRacional div(tRacional a, tRacional b) {
  tRacional resultado;
  resultado.numerador = a.numerador * b.denominador;
  resultado.denominador = a.denominador * b.numerador;
  return resultado;
}

struct tRacional reduz(tRacional r) {
    tRacional resultado;
    int a = r.numerador;
    int b = r.denominador;
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    resultado.numerador = r.numerador / a;
    resultado.denominador = r.denominador / a;
    if (resultado.denominador < 0) {
        resultado.numerador = -resultado.numerador;
        resultado.denominador = -resultado.denominador;
    }

    return resultado;
}

int main() {
  tRacional r1, r2;
  char operacao;

  while (cin >> r1.numerador >> r1.denominador >> operacao >> r2.numerador >> r2.denominador) {
    tRacional resultado;
    switch (operacao) {
      case '+':
        resultado = soma(r1, r2);
        break;
      case '-':
        resultado = soma(r1, negativo(r2));
        break;
      case '*':
        resultado = mult(r1, r2);
        break;
      case '/':
        resultado = div(r1, r2);
        break;
    }
    resultado = reduz(resultado);
    cout << resultado.numerador << " " << resultado.denominador << endl;
  }
  
  return 0;
}