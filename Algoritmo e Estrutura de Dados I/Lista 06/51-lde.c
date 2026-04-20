#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    int chave;
    struct tItem *prox;
} Item;

typedef struct tLista {
    Item *inicio;
    int quantidade;
} Lista;

Lista * criaListaVazia () {
    Lista *lista = (Lista *) malloc(sizeof(Lista));
    lista->inicio = NULL;
    lista->quantidade = 0;
    return lista;
}

Item * criaItem (int x) {
    Item *item = (Item *) malloc(sizeof(Item));
    item->chave = x;
    item->prox = NULL;
    return item;
}

void liberaLista (Lista *lista) {
    Item *atual = lista->inicio;
    while (atual != NULL) {
        Item *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(lista);
}

int vazia (Lista *lista) {
    return lista->quantidade == 0;
}

void imprimir (Lista *lista) {
    Item *atual = lista->inicio;
    while (atual != NULL) {
        printf("%d ", atual->chave);
        atual = atual->prox;
    }
    printf("\n");
}

int buscar (Lista *lista, int x) {
    Item *atual = lista->inicio;
    while (atual != NULL) {
        if (atual->chave == x) {
            return 1;
        }
        atual = atual->prox;
    }
    return 0;
}

void inserir (Lista *lista, int x) {
    if (buscar(lista, x)) {
        return;
    }
    Item *novo = criaItem(x);
    if (vazia(lista) || lista->inicio->chave > x) {
        novo->prox = lista->inicio;
        lista->inicio = novo;
    } else {
        Item *atual = lista->inicio;
        while (atual->prox != NULL && atual->prox->chave < x) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
    lista->quantidade++;
}

void remover (Lista *lista, int x) {
    if (vazia(lista)) {
        return;
    }
    if (lista->inicio->chave == x) {
        Item *temp = lista->inicio;
        lista->inicio = lista->inicio->prox;
        free(temp);
        lista->quantidade--;
        return;
    }
    Item *atual = lista->inicio;
    while (atual->prox != NULL && atual->prox->chave != x) {
        atual = atual->prox;
    }
    if (atual->prox != NULL) {
        Item *temp = atual->prox;
        atual->prox = atual->prox->prox;
        free(temp);
        lista->quantidade--;
    }
}

int main () {
    Lista *lista = criaListaVazia();
    char op;
    int x;
    while (scanf(" %c", &op) != EOF) {
        switch (op) {
            case 'I': scanf("%d", &x); inserir(lista, x); break;
            case 'R': scanf("%d", &x); remover(lista, x); break;
            case 'B': scanf("%d", &x); printf(buscar(lista, x) ? "SIM\n" : "NAO\n"); break;
            case 'L': imprimir(lista); break;
        }
    }
    liberaLista(lista);
    return 0;
}