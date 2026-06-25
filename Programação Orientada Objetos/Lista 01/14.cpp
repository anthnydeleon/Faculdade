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
        cout << v[i];
    }
}

int soma_vetor(int *v, int n) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += v[i];
    }
    
    return sum;
}

int main()  {
    int n;
    
    cin >> n;
    int *v = new int[n];
    
    le_vetor(v, n);
    int sum = soma_vetor(v, n);
    
    cout << sum;
    
    delete[] v;
    
    return 0;
}