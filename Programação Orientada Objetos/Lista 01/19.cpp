#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

void le_vetor(int *v, int n) {
    for(int i = 0; i < n; i++) {
        cin >> v[i];
    }
}

void imprime_vetor(int *v, int n) {
    for(int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
}

void libera_memoria(int *v) {
    delete[] v;
}

int *concatena_vetor(int *v, int *w, int n, int m) {
  int *z = new int[n+m];
  
  for(int i = 0; i < n; i++) z[i] = v[i];
  for(int i = 0; i < m; i++) z[n+i] = w[m-i-1];
  
  return z;
}

int main()  {
    int n, m;
    
    cin >> n >> m;
    
    int *v = new int[n];
    int *w = new int[m];
    int *z = new int[n+m];
    
    le_vetor(v, n);
    le_vetor(w, m);
    
    z = concatena_vetor(v, w, n, m);
    imprime_vetor(z, n+m);
    
    libera_memoria(v);
    libera_memoria(w);
    libera_memoria(z);
    
    return 0;
}