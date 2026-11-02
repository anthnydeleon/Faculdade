#include <bits/stdc++.h>
using namespace std;

class Data {
    private:
        int dia, mes, ano;
    public:
        Data();
        Data(int, int, int);
        bool isValida();
        bool isBissexto();
        int getDiasDoMes();
        void imprime();
        void diaAnterior();
        Data operator-(int);
};

Data::Data() {
    this->dia = this->mes = this->ano = 0;
}

Data::Data(int d, int m, int a) {
    this->dia = d;
    this->mes = m;
    this->ano = a;
}

bool Data::isBissexto() {
    int a = this->ano;
    return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
}

int Data::getDiasDoMes() {
    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (this->mes == 2 && this->isBissexto())
        return 29;
    return diasPorMes[this->mes];
}

bool Data::isValida() {
    if (this->mes < 1 || this->mes > 12) return false;
    if (this->dia < 1 || this->dia > this->getDiasDoMes()) return false;
    return true;
}

void Data::diaAnterior() {
    this->dia--;
    if (this->dia == 0) {
        this->mes--;
        if (this->mes == 0) {
            this->mes = 12;
            this->ano--;
        }
        this->dia = this->getDiasDoMes();
    }
}

Data Data::operator-(int q) {
    Data result(this->dia, this->mes, this->ano);
    for (int i = 0; i < q; i++)
        result.diaAnterior();
    return result;
}

void Data::imprime() {
    cout << this->dia << " " << this->mes << " " << this->ano << "\n";
}

int main() {
    int d, m, a, q;
    while (cin >> d >> m >> a >> q) {
        Data dt(d, m, a);
        dt = dt - q;
        dt.imprime();
    }
    return 0;
}