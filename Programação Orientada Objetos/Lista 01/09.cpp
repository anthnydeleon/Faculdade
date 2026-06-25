#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

string classificacao_imc(double imc) {
    if(imc < 16) return "Magreza grave";
    if(imc < 17) return "Magreza moderada";
    if(imc < 18.5) return "Magreza leve";
    if(imc < 25) return "Saudavel";
    if(imc < 30) return "Sobrepeso";
    if(imc < 35) return "Obesidade Grau I";
    if(imc < 40) return "Obesidade Grau II (severa)";
    return "Obesidade Grau III (morbida)";
}

double calcula_imc(double altura, double peso) {
    return peso / (altura * altura);
}

int main() {
    double altura, peso, imc;
    string classificacao;
    
    cin >> altura >> peso;
    imc = calcula_imc(altura, peso);
    cout << classificacao_imc(imc);
    
    return 0;
}