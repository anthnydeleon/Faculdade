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

int is_valid_date(Data data) {
    int ultimo_dia_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(is_bissexto(data.ano)) {
      ultimo_dia_mes[2] = 29;
    }
    if(data.mes >= 1 && data.mes <= 12 && data.dia >= 1 && data.dia <= ultimo_dia_mes[data.mes]) {
      return 1;
    } else {
      return 0;
    }
}

int main() {
    int n;
    cin >> n;
    
    for(int i = 0; i < n; i++) {
      Data data;
      cin >> data.dia >> data.mes >> data.ano;
      
      if(is_valid_date(data)) {
        cout << "DATA VALIDA" << endl;
      } else {
        cout << "DATA INVALIDA" << endl;
      }
    }
    
    return 0;
}
