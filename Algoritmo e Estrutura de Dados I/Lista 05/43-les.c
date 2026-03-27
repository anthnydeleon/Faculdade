#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *itens;
    int quantidade;
    int tamanho;
} Lista;

Lista *criaListaVazia(int tamanho) {
    Lista *l = (Lista *) malloc(sizeof(Lista));
    l->itens = (int *) malloc(sizeof(int) * tamanho);
    l->quantidade = 0;
    l->tamanho = tamanho;
    return l;
}

void liberaLista(Lista *lista) {
    free(lista->itens);
    free(lista);
}

int cheia(Lista *lista) {
    return lista->quantidade == lista->tamanho;
}

int vazia(Lista *lista) {
    return lista->quantidade == 0;
}

int existe(Lista *lista, int x) {
    for (int i = 0; i < lista->quantidade; i++)
        if (lista->itens[i] == x)
            return 1;
    return 0;
}

void inserir(Lista *lista, int x) {
    if (cheia(lista) || existe(lista, x))
        return;

    int i = lista->quantidade - 1;

    while (i >= 0 && lista->itens[i] > x) {
        lista->itens[i + 1] = lista->itens[i];
        i--;
    }

    lista->itens[i + 1] = x;
    lista->quantidade++;
}

void remover(Lista *lista, int x) {
    int i;
    for (i = 0; i < lista->quantidade; i++) {
        if (lista->itens[i] == x) {
            // desloca à esquerda
            for (int j = i; j < lista->quantidade - 1; j++)
                lista->itens[j] = lista->itens[j + 1];

            lista->quantidade--;
            return;
        }
    }
}

int main() {
    int q, x;
    char op;

    scanf("%d", &q);

    Lista *lista = criaListaVazia(q);

    for (int i = 0; i < q; i++) {
        scanf(" %c %d", &op, &x);

        if (op == 'I')
            inserir(lista, x);
        else if (op == 'E')
            remover(lista, x);
    }

    for (int i = 0; i < lista->quantidade; i++)
        printf("%d\n", lista->itens[i]);

    printf("\n");

    liberaLista(lista);
    return 0;
}