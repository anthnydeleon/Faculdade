#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    int chave;
    struct tItem *prox;
} Item;

typedef struct tFila {
    Item *inicio, *fim;
} Fila;

Fila *criaFilaVazia() {
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

Item *criaItem(int x) {
    Item *item = (Item *)malloc(sizeof(Item));
    item->chave = x;
    item->prox = NULL;
    return item;
}

void liberaFila(Fila *fila) {
    Item *atual = fila->inicio;
    while (atual != NULL) {
        Item *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(fila);
}

int vazia(Fila *fila) {
    return fila->inicio == NULL;
}

void imprimir(Fila *fila) {
    Item *atual = fila->inicio;
    while (atual != NULL) {
        if (atual->prox != NULL)
            printf("%d ", atual->chave);
        else
            printf("%d", atual->chave);
        atual = atual->prox;
    }
    printf("\n");
}

void enfileirar(Fila *fila, int x) {
    Item *novo = criaItem(x);
    if (vazia(fila)) {
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }
}

int desenfileirar(Fila *fila) {
    if (vazia(fila)) return -1;
    Item *removido = fila->inicio;
    int chave = removido->chave;
    fila->inicio = removido->prox;
    if (fila->inicio == NULL)
        fila->fim = NULL;
    free(removido);
    printf("<%d>\n", chave);
    return chave;
}

int main() {
    Fila *fila = criaFilaVazia();
    char op;
    int x;

    while (scanf(" %c", &op) != EOF) {
        switch (op) {
            case 'E':
                scanf("%d", &x);
                enfileirar(fila, x);
                break;
            case 'D':
                if (!vazia(fila))
                    desenfileirar(fila);
                break;
            case 'M':
                imprimir(fila);
                break;
        }
    }

    liberaFila(fila);
    return 0;
}