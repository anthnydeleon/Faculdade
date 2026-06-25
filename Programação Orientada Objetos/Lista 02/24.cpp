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

Data next_day(Data data) {
    int ultimo_dia_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(is_bissexto(data.ano)) {
      ultimo_dia_mes[2] = 29;
    }

    data.dia++;
    if(data.dia > ultimo_dia_mes[data.mes]) {
      data.dia = 1;
      data.mes++;
      if(data.mes > 12) {
        data.mes = 1;
        data.ano++;
      }
    }
    return data;
}

int main() {
  Data data;
  cin >> data.dia >> data.mes >> data.ano;
  
  Data proximo_dia = next_day(data);
  cout << proximo_dia.dia << " " << proximo_dia.mes << " " << proximo_dia.ano << endl;
  
  return 0;
}