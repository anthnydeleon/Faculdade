#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    int chave;
    struct tItem *prox;
} Item;

typedef struct tPilha {
    Item *topo;
    int quantidade;
} Pilha;

Pilha *criaPilhaVazia() {
    Pilha *p = (Pilha *)malloc(sizeof(Pilha));
    p->topo = NULL;
    p->quantidade = 0;
    return p;
}

Item *criaItem(int x) {
    Item *item = (Item *)malloc(sizeof(Item));
    item->chave = x;
    item->prox = NULL;
    return item;
}

void liberaPilha(Pilha *pilha) {
    Item *atual = pilha->topo;
    while (atual != NULL) {
        Item *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(pilha);
}

int vazia(Pilha *pilha) {
    return pilha->quantidade == 0;
}

void imprimir(Pilha *pilha) {
    Item *atual = pilha->topo;
    while (atual != NULL) {
        if (atual->prox != NULL)
            printf("%d ", atual->chave);
        else
            printf("%d", atual->chave);
        atual = atual->prox;
    }
}

void empilha(Pilha *pilha, int x) {
    Item *novo = criaItem(x);
    novo->prox = pilha->topo;
    pilha->topo = novo;
    pilha->quantidade++;
}

int desempilha(Pilha *pilha) {
    if (vazia(pilha)) return -1;
    Item *removido = pilha->topo;
    int chave = removido->chave;
    pilha->topo = removido->prox;
    pilha->quantidade--;
    free(removido);
    printf("[%d]\n", chave);
    return chave;
}

int main() {
    Pilha *pilha = criaPilhaVazia();
    char op;
    int x;

    while (scanf(" %c", &op) != EOF) {
        switch (op) {
            case 'E':
                scanf("%d", &x);
                empilha(pilha, x);
                break;
            case 'D':
                if (!vazia(pilha))
                    desempilha(pilha);
                break;
            case 'M':
                imprimir(pilha);
                printf("\n");
                break;
        }
    }

    liberaPilha(pilha);
    return 0;
}