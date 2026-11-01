#include <iostream>
using namespace std;
class Inteiro {
    private:
        int v[100];
    public:
        Inteiro(string);
        void soma(Inteiro);
        void sub(Inteiro);
        void mult(Inteiro);
        void imprime();
};

Inteiro::Inteiro(string s) {
    int i, j=0;
    for (i=0; i<100; i++) { this->v[i] = 0; }
    for (i=s.size()-1; i>=0; i--) {
        this->v[j++] = s[i]-'0';
    }
}
void Inteiro::imprime() {
    int i;
    for (i = 100-1; i > 0 && this->v[i] == 0; i--);
    for (; i >= 0; i--) { cout << this->v[i]; }
    cout << "\n";
}
void Inteiro::soma(Inteiro a) {
    int i, vaiUm = 0;
    for (i = 0; i < 100; i++) {
        int somaD = (this->v[i] + a.v[i] + vaiUm);
        this->v[i] = somaD % 10;
        vaiUm = somaD / 10;
    }
}
void Inteiro::sub(Inteiro a) {
    int i, emprestaUm = 0;
    for (i = 0; i < 100; i++) {
        int subD = this->v[i] - a.v[i] - emprestaUm;
        if (subD < 0) {
            subD = subD + 10;
            emprestaUm = 1;
        }
        else { emprestaUm = 0; }
        this->v[i] = subD;
    }
} 
void Inteiro::mult(Inteiro a) {
    Inteiro res("0");
    int i, j;
    for (j = 0; j < 100; j++) {
        Inteiro tmp("0");
        int vaiX = 0;
        for (i = 0; i < 100-j; i++) {
            int multD = this->v[i]*a.v[j] + vaiX;
            tmp.v[i+j] = multD % 10;
            vaiX = multD / 10;
        }
        res.soma (tmp);
    }

    for (i=0; i<100; i++) { this->v[i] = res.v[i]; }
} 
int main () {
    string s1, s2;
    char op;
    while (cin >> s1 >> op >> s2) {
        Inteiro i1(s1), i2(s2);
        switch (op) {
            case '+': i1.soma(i2); break;
            case '-': i1.sub(i2); break;
            case '*': i1.mult(i2); break;
        }
        i1.imprime();
    }
    return 0;
}