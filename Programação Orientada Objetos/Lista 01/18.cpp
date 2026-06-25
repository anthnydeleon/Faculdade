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
    delete[] matriz[n];
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

double soma_diagonal(int **matriz, int n) {
  int soma = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < i; j++) {
        soma += matriz[i][j];
    }
  }
  
  return static_cast<double>(soma);
}

double media_diagonal(int **matriz, int n) {
  int soma = 0, count = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < i; j++) {
        soma += matriz[i][j];
        count++;
    }
  }
  
  return static_cast<double>(soma)/count;
}

int main()  {
    string metodo;
    int n, **matriz;
    
    
    cin >> metodo >> n;
    
    matriz = new int *[n];
    aloca_memoria(matriz, n);
    le_matriz(matriz, n);
    
    if (metodo == "S") {
      cout << fixed << setprecision(1) << soma_diagonal(matriz, n);
    } else {
      cout << fixed << setprecision(1) << media_diagonal(matriz, n);
    }
    
    libera_memoria(matriz, n);
    
    return 0;
}