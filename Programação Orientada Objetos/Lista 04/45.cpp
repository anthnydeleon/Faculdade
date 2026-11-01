#include <iostream>
using namespace std;

class Data {
    private:
        int dia, mes, ano;
    public:
        Data ();
        Data (int, int, int);
        bool isValida ();
        bool isBissexto ();
        int getDiasDoMes ();
        void imprime();
        void diaSeguinte ();
        Data operator + (int); // sobrecarga de operador
};
Data::Data () { this->dia=this->mes=this->ano=0; }
Data::Data (int d, int m, int a) {
    this->dia=d; this->mes=m; this->ano=a;
}
bool Data::isBissexto() {
    int ano = this->ano;
    return (ano%4==0 && ano%100!=0) || ano%400==0;
}
int Data::getDiasDoMes() {
    //                  1    2   3   4   5   6   7   8   9   10  11  12
    int diasDoMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int m=this->mes;
    if (m == 2 && this->isBissexto()) { diasDoMes[2]=29; }
    return diasDoMes[m];
}
bool Data::isValida() {
    int d = this->dia, m = this->mes, a = this->ano;
    int diasDoMes = this->getDiasDoMes();
    return d >= 1 && d <= diasDoMes && m >= 1 && m <= 12;
}
void Data::imprime() {
    cout << this->dia << " " << this->mes << " " << this->ano << endl;
}
void Data::diaSeguinte() {
    this->dia++;
    if (!this->isValida ()) {
        this->dia=1; this->mes++;
        if (!this->isValida ()) { this->mes=1; this->ano++; }
    }
}
Data Data::operator + (int d) { // sobrecarga de operador
    int i;
    for (i=0; i<d; i++) { this->diaSeguinte(); }
    return *this;
}

int main () {
    int d, m, a, q, i;
    while (cin >> d >> m >> a >> q) {
        Data dt(d,m,a);
        dt = dt + q;  // Data = Data + int
        dt.imprime();
    }
    return 0;
}
