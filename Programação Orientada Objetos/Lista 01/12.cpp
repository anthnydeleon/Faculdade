#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()  {
    int value, min;
    
    cin >> value;
    min = value;
    
    if(value == 0) {
      cout << "nenhum numero informado";
    } else {
      while (value != 0) {
        if(value < min) min = value;
        cin >> value;
      }
      
      cout << min;
    }
    
    return 0;
}