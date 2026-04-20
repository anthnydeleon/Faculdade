#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tItem {
    char nome[101];
    int rank;
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

Item *criaItem(char *s) {
    Item *item = (Item *)malloc(sizeof(Item));
    strcpy(item->nome, s);
    item->rank = 0;
    item->ant = NULL;
    item->prox = NULL;
    return item;
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

int vazia(Lista *lista) {
    return lista->primeiro == NULL;
}

void imprimir(Lista *lista) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        printf("%s [%d]\n", atual->nome, atual->rank);
        atual = atual->prox;
    }
    printf("\n");
}

Item *buscaNome(Lista *lista, char *x) {
    Item *atual = lista->primeiro;
    while (atual != NULL) {
        if (strcmp(x, atual->nome) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

void troca(Lista *lista, Item *ant, Item *atu) {
    Item *ant2 = ant->ant;
    Item *prox = atu->prox;

    if (ant2 != NULL)
        ant2->prox = atu;
    else
        lista->primeiro = atu;
    atu->ant = ant2;

    atu->prox = ant;
    ant->ant = atu;

    ant->prox = prox;
    if (prox != NULL)
        prox->ant = ant;
    else
        lista->ultimo = ant;
}

void inserir(Lista *lista, char *s) {
    Item *item = buscaNome(lista, s);

    if (item == NULL) {
        Item *novo = criaItem(s);
        novo->rank = 1;
        if (vazia(lista)) {
            lista->primeiro = novo;
            lista->ultimo = novo;
        } else {
            novo->ant = lista->ultimo;
            lista->ultimo->prox = novo;
            lista->ultimo = novo;
        }
        return;
    }

    item->rank++;
    while (item->ant != NULL && item->rank > item->ant->rank)
        troca(lista, item->ant, item);
}

int main() {
    char s[101];
    Lista *lista = criaListaVazia();
    while (scanf("%s", s) != EOF)
        inserir(lista, s);
    imprimir(lista);
    liberaLista(lista);
    return 0;
}