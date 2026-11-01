#include <bits/stdc++.h>
using namespace std;

class DataJuliana {
    private:
        int diaJuliano;
    public:
        DataJuliana();
        DataJuliana(int);
        void setDiaJuliano(int);
        int getDiaJuliano();
};

DataJuliana::DataJuliana() {
    this->diaJuliano = 0;
}
DataJuliana::DataJuliana(int d) {
    this->diaJuliano = d;
}
void DataJuliana::setDiaJuliano(int d) {
    this->diaJuliano = d;
}
int DataJuliana::getDiaJuliano() {
    return this->diaJuliano;
}

class DataGregoriana {
    private:
        int dia, mes, ano;
    public:
        DataGregoriana();
        DataGregoriana(int, int, int);
        DataGregoriana(DataJuliana);
        void setDia(int);
        void setMes(int);
        void setAno(int);
        int getDia();
        int getMes();
        int getAno();
        operator DataJuliana();
};

DataJuliana gregoriana2juliana(DataGregoriana dt) {
    int a, b, c, d, e, diaJuliano;
    int dia = dt.getDia();
    int mes = dt.getMes();
    int ano = dt.getAno();

    if (mes < 3) { ano--; mes += 12; }
    a = ano / 100;
    b = a / 4;
    c = 2 - a + b;
    d = (int)(365.25 * (ano + 4716));
    e = (int)(30.6001 * (mes + 1));
    diaJuliano = (int)(d + e + dia + 0.5 + c - 1524.5);

    return DataJuliana(diaJuliano);
}

DataGregoriana::DataGregoriana() {
    this->dia = this->mes = this->ano = 0;
}
DataGregoriana::DataGregoriana(int d, int m, int a) {
    this->dia = d;
    this->mes = m;
    this->ano = a;
}
DataGregoriana::DataGregoriana(DataJuliana dj) {
    this->dia = this->mes = this->ano = 0;
}

void DataGregoriana::setDia(int d) { this->dia = d; }
void DataGregoriana::setMes(int m) { this->mes = m; }
void DataGregoriana::setAno(int a) { this->ano = a; }
int DataGregoriana::getDia()        { return this->dia; }
int DataGregoriana::getMes()        { return this->mes; }
int DataGregoriana::getAno()        { return this->ano; }

DataGregoriana::operator DataJuliana() {
    return gregoriana2juliana(*this);
}

int main() {
    int d, m, a;
    DataGregoriana dt;
    DataJuliana dj;

    while (cin >> d >> m >> a) {
        dt.setDia(d);
        dt.setMes(m);
        dt.setAno(a);
        dj = dt;
        cout << dj.getDiaJuliano() << "\n";
    }

    return 0;
}