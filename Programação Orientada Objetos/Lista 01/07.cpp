#include <iostream>
using namespace std;

int main() {
    int segundos, minutos, horas;
    
    cin >> segundos;
    
    minutos = (segundos / 60) % 60;
    horas = segundos / 60 / 60;
    segundos = segundos % 60;
    
    cout << horas << ":" << minutos << ":" << segundos;
  
    return 0;
}