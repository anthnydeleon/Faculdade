#include <iostream>
using namespace std;
class Complexo {
    private:
        int re, im;  // atributos "encapsulados"
    public:
        Complexo(int,int);  // construtor
        void soma(Complexo);
        void sub(Complexo);
        void mult(Complexo);
        void div(Complexo);
        Complexo conjugado();
        void imprime();
        // sobrecarga de operadores: + - * /
        Complexo operator + (Complexo);
        Complexo operator - (Complexo);
        Complexo operator * (Complexo);
        Complexo operator / (Complexo);
};
Complexo Complexo::operator + (Complexo c) { this->soma(c); return *this; }
Complexo Complexo::operator - (Complexo c) { this->sub(c); return *this; }
Complexo Complexo::operator * (Complexo c) { this->mult(c); return *this; }
Complexo Complexo::operator / (Complexo c) { this->div(c); return *this; }
Complexo::Complexo(int r, int i) {
    this->re=r; this->im=i;
}
void Complexo::soma(Complexo k) { // this = this + k  -->  this.soma(k)
    this->re += k.re; this->im += k.im;
}
void Complexo::sub(Complexo k) {
    this->re -= k.re; this->im -= k.im;
}
void Complexo::mult(Complexo k) {
    int a=this->re, b=this->im,
        c=k.re, d=k.im;
    this->re = a*c - b*d;
    this->im = a*d + b*c;
}
Complexo Complexo::conjugado() {
    return Complexo(this->re, -this->im);
}
void Complexo::div(Complexo k) {
    Complexo kbar = k.conjugado();
    this->mult(kbar);
    k.mult(kbar);
    this->re /= k.re;
    this->im /= k.re;
}
void Complexo::imprime() {
    cout << this->re << " "    //<< (this->im>=0 ? "+" : "")
         << this->im << "i\n";
}
int main()
{
    int a,b,c,d;
    char op,i;
    while (cin >> a >> b >> i >> op >> c >> d >> i) {
        Complexo c1(a,b), c2(c,d);
        switch(op) {
            case '+': c1=c1+c2; break;
                      //c1.soma(c2); break;
            case '-': c1=c1-c2; break;
                      //c1.sub(c2); break;
            case '*': c1=c1*c2; break;
                      //c1.mult(c2); break;
            case '/': c1=c1/c2; break;
                      //c1.div(c2); break;
        }
        c1.imprime();
    }
    return 0;
}