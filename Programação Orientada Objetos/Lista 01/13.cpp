#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


void le_vetor(int *v, int n) {
  for(int i = 0; i < n; i++) {
      cin >> v[i];
    }
}

int count_less_than_x(int *v, int n, int x) {
  int count = 0;
  for(int i = 0; i < n; i++) {
      if(v[i] >= x) count++;
    }
  
  return count;
}

int main()  {
    int n, *v, x;
    
    cin >> n;
    v = new int[n];
    
    le_vetor(v, n);
    
    cin >> x;
    int count = count_less_than_x(v, n, x);
    cout << count;
    
    delete[] v;
    
    return 0;
}