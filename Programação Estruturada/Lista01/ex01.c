#include <stdio.h>
// Questão 1. Escreva um programa em C que leia um número n do usuário e imprima todos os números pares do intervalo [0,n]

void printEvenNumbers(int n) {
    for (int i = 0; i <= n; i++) {
        if (i % 2 == 0) printf("%d  ", i);
    }
}

int main() {
    int n;
    printf("Digite um numero: ");
    scanf("%d", &n);

    printEvenNumbers(n);
    return 0;
}