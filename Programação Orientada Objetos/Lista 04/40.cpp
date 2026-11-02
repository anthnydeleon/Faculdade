#include <iostream>
using namespace std;
class Racional {
    private: // encapsulamento dos atributos
        int num, denom;
    public:
        Racional(int, int);  // construtor
        void soma(Racional);
        void sub(Racional);
        void mult(Racional);
        void div(Racional);
        void neg();
        void inv();
        void reduz();
        void imprime();
        // sobrecarga de operadores: + - * /
        Racional operator + (Racional);
        Racional operator - (Racional);
        Racional operator * (Racional);
        Racional operator / (Racional);
};
Racional Racional::operator + (Racional r) { this->soma(r); return *this; }
Racional Racional::operator - (Racional r) { this->sub(r); return *this; }
Racional Racional::operator * (Racional r) { this->mult(r); return *this; }
Racional Racional::operator / (Racional r) { this->div(r); return *this; }
Racional::Racional(int n, int d) {
    this->num=n; this->denom=d;
}
void Racional::neg() {
    this->num = -this->num;  // inverte sinal
}
// maximo divisor comum
int mdc(int a, int b) {
    if (b==0) { return a; }
    return mdc(b,a%b);
}
void Racional::reduz() { // simplifica fracao
    int a, b, md;
    a=this->num; b=this->denom;
    if (a<0) { a=-a; }
    if (b<0) { b=-b; }
    md = mdc(a,b);
    this->num /= md; this->denom /= md;
   
    if (this->denom < 0) {
        this->num = -this->num;
        this->denom = -this->denom;
    }
   
}
void Racional::soma(Racional r) {
    int a, b, c, d;
    a=this->num; b=this->denom;
    c=r.num; d=r.denom;
    // a/b + c/d = (ad + bc)/(bd)
    this->num = a*d + b*c;
    this->denom = b*d;  // this = this + r
}
void Racional::sub(Racional r) {
    r.neg();
    this->soma(r);  // this = this - r
}
void Racional::inv() {  // troca:  num <-> denom
    int tmp = this->num;  
    this->num = this->denom;
    this->denom = tmp;
}
void Racional::mult(Racional r) {
    int a, b, c, d;
     a=this->num; b=this->denom;
     c=r.num; d=r.denom;
     // a/b * c/d = (ac)/(bd)
     this->num = a*c;
     this->denom = b*d;   // this = this * r
}
void Racional::div(Racional r) {
    r.inv();
    this->mult(r);   // this = this * 1/r
}
void Racional::imprime() {
    cout << this->num << " " << this->denom << "\n";
}
int main()
{
    int a, b, c, d;
    char op;
    while (cin >> a >> b >> op >> c >> d) {
        Racional r1(a,b), r2(c,d);
        switch(op) {
            case '+': r1=r1+r2; break;
                      //r1.soma(r2); break;
            case '-': r1=r1-r2; break;
                      //r1.sub(r2); break;
            case '*': r1=r1*r2; break;
                      //r1.mult(r2); break;
            case '/': r1=r1/r2; break;
                      //r1.div(r2); break;
        }
        r1.reduz();  // simplica fracao
        r1.imprime();
    }
    return 0;
}