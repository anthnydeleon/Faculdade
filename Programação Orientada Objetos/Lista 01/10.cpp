#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    int n, count;
    cin >> n;
    
    count = 0;
    for(int i = 1; i <= n; i++) {
      if((n % i) == 0 && (i % 2) == 0) count++;
    }
    
    cout << count;
    
    return 0;
}