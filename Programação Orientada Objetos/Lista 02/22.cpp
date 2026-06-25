#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int is_bissexto(int n) {
    if( (n % 400) == 0 || ( (n % 4) == 0 && (n % 100) != 0 ) ) {
      return 1;  
    } else {
      return 0;
    }
}

int main() {
    int n;
    cin >> n;
    
    if(is_bissexto(n)) {
      cout << "ANO BISSEXTO";  
    } else {
      cout << "ANO NAO BISSEXTO";
    }
    
    return 0;
}
