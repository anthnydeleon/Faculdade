#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


void aloca_memoria(int **matriz, int n, int m) {
  for(int i = 0; i < n; i++) {
    matriz[i] = new int[m];
  }
} 

void libera_memoria(int **matriz, int n) {
  for(int i = 0; i < n; i++) {
    delete [] matriz[i];
  }
  delete [] matriz;
}

void le_matriz(int **matriz, int n, int m) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
        cin >> matriz[i][j];
    }
  }
}

void imprime_matriz(int **matriz, int n, int m) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
        cout << matriz[j][i] << " ";
    }
    cout << endl;
  }
}

int main()  {
    int n, m, **matriz_A;
    
    cin >> n >> m;;
    matriz_A = new int *[n];
    
    aloca_memoria(matriz_A, n, m);
    le_matriz(matriz_A, n, m);
    imprime_matriz(matriz_A, m, n);
    libera_memoria(matriz_A, n);
    
    return 0;
}