#include <iostream>
using namespace std;

class Data {
    private:
        int dia, mes, ano;
    public:
        Data(int,int,int);
        bool isBissexto();
        int getDiasDoMes();
        bool isValida();
};

Data::Data(int dia, int mes, int ano) {
    this->dia = dia; 
    this->mes = mes; 
    this->ano = ano;
}

bool Data::isBissexto() {
    int ano = this->ano;
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 100 == 0;
}

int Data::getDiasDoMes() {
    int mes = this->mes;
    int tmp[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (this->isBissexto()) { tmp[2] = 29; }
    return tmp[mes];
}

bool Data::isValida() {
    int dia = this->dia, mes = this->mes, ano = this->ano;
    int ultimo = this->getDiasDoMes();
    return dia >=1 && dia <= ultimo && mes >= 1 && mes <= 12;
}

int main() {
    int n, i, d, m, a;
    cin >> n;  
    for (i=0; i<n; i++) {
        cin >> d >> m >> a;
        Data dt(d,m,a); 
        if (dt.isValida()) {
            cout << "DATA VALIDA\n";
        }
        else {
            cout << "DATA INVALIDA\n";
        }
    }
    return 0;
}