#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *itens;
    int inicio, fim;
    int tamanho;
} Fila;

Fila *criaFilaVazia(int tamanho) {
    Fila *f = (Fila *) malloc(sizeof(Fila));
    f->itens = (int *) malloc(sizeof(int) * tamanho);
    f->inicio = -1;
    f->fim = -1;
    f->tamanho = tamanho;
    return f;
}

void liberaFila(Fila *fila) {
    free(fila->itens);
    free(fila);
}

int vazia(Fila *fila) {
    return fila->inicio == -1;
}

int cheia(Fila *fila) {
    return fila->fim == fila->tamanho - 1;
}

void enfileirar(Fila *fila, int x) {
    if (cheia(fila)) return;

    if (vazia(fila))
        fila->inicio = 0;

    fila->itens[++fila->fim] = x;
}

int desenfileirar(Fila *fila) {
    if (vazia(fila)) return -1;

    int valor = fila->itens[fila->inicio++];

    if (fila->inicio > fila->fim) {
        fila->inicio = -1;
        fila->fim = -1;
    }

    return valor;
}

int main() {
    int n, x;
    char op;

    scanf("%d", &n);
    Fila *f = criaFilaVazia(n);

    while (scanf(" %c", &op) != EOF) {
        if (op == 'E') {
            scanf("%d", &x);
            enfileirar(f, x);
        }
        else if (op == 'D') {
            int r = desenfileirar(f);
            if (r != -1)
                printf("%d\n", r);
        }
    }

    printf("\n");

    liberaFila(f);
    return 0;
}