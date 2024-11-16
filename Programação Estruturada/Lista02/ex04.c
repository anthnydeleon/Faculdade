#include <stdio.h>
#define MAX 100

void le_matriz(int M[][MAX], int linhas, int colunas) {
    printf("Digite os elementos da matriz de tamanho %dx%d: ", linhas, colunas);
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            scanf("%d", &M[i][j]);
        }
    }
}

void imprime_matriz(int M[][MAX], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
}

void soma_matriz(int A[][MAX], int B[][MAX], int C[][MAX], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

int main() {
    int linhas, colunas;

    printf("Digite a quantidade de linhas da matriz: ");
    scanf("%d", &linhas);
    printf("Digite a quantidade de colunas da matriz: ");
    scanf("%d", &colunas);

    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

    le_matriz(A, linhas, colunas);
    le_matriz(B, linhas, colunas);
    soma_matriz(A, B, C, linhas, colunas);

    printf("\nResultado da soma das matrizes:\n");
    imprime_matriz(C, linhas, colunas);

    return 0;
}