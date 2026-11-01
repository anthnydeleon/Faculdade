#include <bits/stdc++.h>
using namespace std;

class DataJuliana{
    private:
        int diaJuliano;                 //atributo
    public:
        DataJuliana();                  //construtores                                  
        DataJuliana(int);
        void setDiaJuliano(int);        //metodos
        int getDiaJuliano();
};

//Construtores
DataJuliana::DataJuliana(){
    this->diaJuliano = 0;
}
DataJuliana::DataJuliana(int d){
    this->diaJuliano = d;
}

//Metodos
void DataJuliana::setDiaJuliano(int d){
    this->diaJuliano = d;
}
int DataJuliana::getDiaJuliano(){
    return this->diaJuliano;
}


class DataGregoriana{
    private:
        int dia, mes, ano;              //atributos
    public:
        DataGregoriana();               //construtores
        DataGregoriana(int, int, int);  
        DataGregoriana (DataJuliana);   //             que converte juliana p/ gregoriana

        void setDia(int);               //metodos
        void setMes(int);
        void setAno(int);
        int getDia();
        int getMes();
        int getAno();
};

//Função convertora
DataGregoriana juliana2gregoriana(DataJuliana dj) {
    int q, z, w, x, a, b, c, d, e, f, dia, mes, ano;
    int diaJuliano = dj.getDiaJuliano();

    // conversao de juliano para gregoriano
    q = diaJuliano + 0.5;
    z = q;
    w = (z - 1867216.25)/36524.25;
    x = w/4;
    a = z+1+w-x;
    b = a+1524;
    c = (b-122.1)/365.25;
    d = 365.25*c;
    e = (b-d)/30.6001;
    f = 30.6001*e;

    dia = b-d-f+(q-z);
    
    if (e<14){
        mes = e-1;
    } else {
        mes = e-13;
    }
    
    if (mes<3){
        ano = c-4715;
    } else {
        ano = c-4716;
    }

    DataGregoriana dt(dia,mes,ano);

    return dt;
}

//Construtores
DataGregoriana::DataGregoriana(){
    this->dia = this->mes = this->ano = 0;
}
DataGregoriana::DataGregoriana(int d, int m, int a){
    this->dia = d;
    this->mes = m;
    this->ano = a;
}
//            "convertor"
DataGregoriana::DataGregoriana(DataJuliana dj){
    DataGregoriana dt = juliana2gregoriana(dj);

    this->dia = dt.getDia();
    this->mes = dt.getMes();
    this->ano = dt.getAno();
}

//Metodos
void DataGregoriana::setDia(int d){
    this->dia = d;
}
void DataGregoriana::setMes(int m){
    this->mes = m;
}
void DataGregoriana::setAno(int a){
    this->ano = a;
}
int DataGregoriana::getDia(){
    return this->dia;
}
int DataGregoriana::getMes(){
    return this->mes;
}
int DataGregoriana::getAno(){
    return this->ano;
}


int main() {
    int d;
    DataJuliana dj;
    DataGregoriana dt;

    while (cin >> d){
        dj.setDiaJuliano(d);
        dt = dj;                        // dt = DataGregoriana(dj); --> conversao por construtor
        cout << dt.getDia() << " " << dt.getMes() << " " << dt.getAno() << "\n";
    }

    return 0;
}