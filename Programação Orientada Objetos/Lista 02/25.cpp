#include <iostream>
using namespace std;

struct Data {
  int dia;
  int mes;
  int ano;
};

int is_bissexto(int n) {
    if( (n % 400) == 0 || ( (n % 4) == 0 && (n % 100) != 0 ) ) {
      return 1;  
    } else {
      return 0;
    }
}

Data previous_day(Data data) {
    int ultimo_dia_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(is_bissexto(data.ano)) {
      ultimo_dia_mes[2] = 29;
    }

    data.dia--;
    if(data.dia < 1) {
      data.mes--;
      if(data.mes < 1) {
        data.mes = 12;
        data.ano--;
      }
      data.dia = ultimo_dia_mes[data.mes];
    }
    return data;
}

int main() {
  Data data;
  cin >> data.dia >> data.mes >> data.ano;
  
  Data dia_anterior = previous_day(data);
  cout << dia_anterior.dia << " " << dia_anterior.mes << " " << dia_anterior.ano << endl;
  
  return 0;
}