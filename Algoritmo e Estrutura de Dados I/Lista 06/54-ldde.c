#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    int chave;
    struct tItem *ant, *prox;
} Item;

typedef struct tLista {
    Item *primeiro, *ultimo;
    int quantidade;
} Lista;

Lista *criaListaVazia() {
    Lista *l = (Lista *)malloc(sizeof(Lista));
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->quantidade = 0;
    return l;
}

Item *criaItem(int x) {
    Item *item = (Item *)malloc(sizeof(Item));
    item->chave = x;
    item->ant = NULL;
    item->prox = NULL;
    return item;
}

void liberaLista(Lista *lista) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        Item *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(lista);
}

int vazia(Lista *lista) {
    return lista->quantidade == 0;
}

void imprimir(Lista *lista) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        printf("%d ", atual->chave);
        atual = atual->prox;
    }
    printf("\n");
}

void imprimirReverso(Lista *lista) {
    Item *atual = lista->ultimo;
    while (atual != NULL) {
        printf("%d ", atual->chave);
        atual = atual->ant;
    }
    printf("\n");
}

void inserir(Lista *lista, int x) {
    // lista vazia
    if (vazia(lista)) {
        Item *novo = criaItem(x);
        lista->primeiro = novo;
        lista->ultimo = novo;
        lista->quantidade++;
        return;
    }

    Item *atual = lista->primeiro;
    while (atual != NULL && atual->chave < x)
        atual = atual->prox;

    if (atual != NULL && atual->chave == x)
        return;

    Item *novo = criaItem(x);

    // inserção no fim
    if (atual == NULL) {
        novo->ant = lista->ultimo;
        lista->ultimo->prox = novo;
        lista->ultimo = novo;

    // inserção no início
    } else if (atual == lista->primeiro) {
        novo->prox = lista->primeiro;
        lista->primeiro->ant = novo;
        lista->primeiro = novo;

    // inserção no meio
    } else {
        novo->prox = atual;
        novo->ant = atual->ant;
        atual->ant->prox = novo;
        atual->ant = novo;
    }

    lista->quantidade++;
}

void remover(Lista *lista, int x) {
    if (vazia(lista)) return;

    Item *atual = lista->primeiro;
    while (atual != NULL && atual->chave != x)
        atual = atual->prox;

    if (atual == NULL) return;

    // remoção do início
    if (atual == lista->primeiro) {
        lista->primeiro = atual->prox;
        if (lista->primeiro != NULL)
            lista->primeiro->ant = NULL;
        else
            lista->ultimo = NULL;

    // remoção do fim
    } else if (atual == lista->ultimo) {
        lista->ultimo = atual->ant;
        lista->ultimo->prox = NULL;

    // remoção do meio
    } else {
        atual->ant->prox = atual->prox;
        atual->prox->ant = atual->ant;
    }

    free(atual);
    lista->quantidade--;
}

int main() {
    Lista *lista = criaListaVazia();
    char op;
    int x;

    while (scanf(" %c", &op) != EOF) {
        switch (op) {
            case 'I':
                scanf("%d", &x);
                inserir(lista, x);
                break;
            case 'E':
                scanf("%d", &x);
                remover(lista, x);
                break;
            case 'M':
                imprimir(lista);
                break;
            case 'R':
                imprimirReverso(lista);
                break;
        }
    }

    liberaLista(lista);
    return 0;
}