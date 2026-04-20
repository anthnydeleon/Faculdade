#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tItem {
    char palavra[101];
    int count;
    struct tItem *ant, *prox;
} Item;

typedef struct tLista {
    Item *primeiro, *ultimo;
} Lista;

Lista *criaListaVazia() {
    Lista *l = (Lista *)malloc(sizeof(Lista));
    l->primeiro = NULL;
    l->ultimo = NULL;
    return l;
}

Item *criaItem(char *palavra) {
    Item *item = (Item *)malloc(sizeof(Item));
    strcpy(item->palavra, palavra);
    item->count = 1;
    item->ant = NULL;
    item->prox = NULL;
    return item;
}

void sobePosicao(Lista *lista, Item *item) {
    while (item->ant != NULL && item->count > item->ant->count) {
        Item *anterior = item->ant;

        if (item->prox != NULL)
            item->prox->ant = anterior;
        else
            lista->ultimo = anterior;

        if (anterior->ant != NULL)
            anterior->ant->prox = item;
        else
            lista->primeiro = item;

        item->ant = anterior->ant;
        anterior->prox = item->prox;
        item->prox = anterior;
        anterior->ant = item;
    }

    if (item->ant == NULL)
        lista->primeiro = item;
}

void buscarOuInserir(Lista *lista, char *palavra) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            atual->count++;
            sobePosicao(lista, atual);
            return;
        }
        atual = atual->prox;
    }

    Item *novo = criaItem(palavra);
    if (lista->primeiro == NULL) {
        lista->primeiro = novo;
        lista->ultimo = novo;
    } else {
        novo->ant = lista->ultimo;
        lista->ultimo->prox = novo;
        lista->ultimo = novo;
    }
}

void imprimir(Lista *lista) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        printf("%s [%d]\n", atual->palavra, atual->count);
        atual = atual->prox;
    }
    printf("\n");
}

void liberaLista(Lista *lista) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        Item *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(lista);
}

int main() {
    Lista *lista = criaListaVazia();
    char palavra[101];

    while (scanf("%s", palavra) != EOF)
        buscarOuInserir(lista, palavra);

    imprimir(lista);
    liberaLista(lista);
    return 0;
}