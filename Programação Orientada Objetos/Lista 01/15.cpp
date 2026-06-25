#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


void le_vetor(int *v, int n) {
  for(int i = 0; i < n; i++) {
        cin >> v[i];
    }
}

int find_x(int *v, int n, int x) {
    for(int i = 0; i < n; i++) {
        if(v[i] == x) return 1;
    }
    
    return 0;
}

int main()  {
    int n, *v, x;
    
    cin >> n;
    v = new int[n];
    
    le_vetor(v, n);
    
    while(cin >> x) {
        if(find_x(v, n, x)) {
          cout << "SIM" << endl;
        } else {
          cout << "CHAVE NAO ENCONTRADA" << endl;
        }
    }
    
    delete[] v;
    
    return 0;
}