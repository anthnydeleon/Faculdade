#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *itens;
    int inicio, fim;
    int tamanho;
} FilaCircular;

FilaCircular *criaFilaCircularVazia(int tamanho) {
    FilaCircular *f = (FilaCircular *) malloc(sizeof(FilaCircular));
    f->itens = (int *) malloc(sizeof(int) * tamanho);
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = tamanho;
    return f;
}

void liberaFilaCircular(FilaCircular *fila) {
    free(fila->itens);
    free(fila);
}

int vazia(FilaCircular *fila) {
    return fila->inicio == fila->fim;
}

int cheia(FilaCircular *fila) {
    return (fila->fim + 1) % fila->tamanho == fila->inicio;
}

void enfileirar(FilaCircular *fila, int x) {
    if (cheia(fila)) return;

    fila->itens[fila->fim] = x;
    fila->fim = (fila->fim + 1) % fila->tamanho;
}

int desenfileirar(FilaCircular *fila) {
    if (vazia(fila)) return -1;

    int valor = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->tamanho;

    return valor;
}

int main() {
    int n, x;
    char op;

    scanf("%d", &n);

    FilaCircular *f = criaFilaCircularVazia(n + 1);

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

    liberaFilaCircular(f);
    return 0;
}