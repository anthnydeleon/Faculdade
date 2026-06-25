#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    int a, b, c, delta;
    double x1, x2;
    
    cin >> a >> b >> c;
    
    delta = b*b - 4*a*c;
    x1 = ( -b + sqrt(delta) ) / (2*a);
    x2 = ( -b - sqrt(delta) ) / (2*a);
    
    cout << fixed << setprecision(4);
    cout << x1 << " " << x2;
  
    return 0;
}