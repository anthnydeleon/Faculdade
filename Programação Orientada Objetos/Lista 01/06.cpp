#include <iostream>
using namespace std;

int main() {
    long ano, anoNascimento;
    string nome;
    
    cin >> ano >> nome >> anoNascimento;
    cout << nome << ", voce completa " << ano-anoNascimento << " anos de idade neste ano.";

    return 0;
}