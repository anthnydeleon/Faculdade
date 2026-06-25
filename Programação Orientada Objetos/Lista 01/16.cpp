#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


void aloca_memoria(int **matriz, int n) {
  for(int i = 0; i < n; i++) {
    matriz[i] = new int[n];
  }
} 

void libera_memoria(int **matriz, int n) {
  for(int i = 0; i < n; i++) {
    delete[] matriz[i];
  }
  
  delete[] matriz;
} 

void le_matriz(int **matriz, int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
        cin >> matriz[i][j];
    }
  }
}

void imprime_matriz(int **matriz, int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
        cout << matriz[i][j] << " ";
    }
    cout << endl;
  }
}

void soma_matriz(int **matriz_A, int **matriz_B, int **matriz_C, int n) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
        matriz_C[i][j] = matriz_A[i][j] + matriz_B[i][j];
    }
  }
}

int main()  {
    int n, **matriz_A, **matriz_B, **matriz_C;
    
    cin >> n;
    matriz_A = new int *[n];
    matriz_B = new int *[n];
    matriz_C = new int *[n];
    
    aloca_memoria(matriz_A, n);
    aloca_memoria(matriz_B, n);
    aloca_memoria(matriz_C, n);
    
    le_matriz(matriz_A, n);
    le_matriz(matriz_B, n);
    le_matriz(matriz_C, n);
    
    soma_matriz(matriz_A, matriz_B, matriz_C, n);
    
    imprime_matriz(matriz_C, n);
    
    libera_memoria(matriz_A, n);
    libera_memoria(matriz_B, n);
    libera_memoria(matriz_C, n);
    
    return 0;
}