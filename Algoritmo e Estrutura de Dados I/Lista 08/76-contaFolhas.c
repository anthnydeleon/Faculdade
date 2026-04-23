#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    int chave;
    struct tItem *pai, *esq, *dir;
} Item;

typedef struct tArvore {
    Item *raiz;
    int quantidade;
} Arvore;

Item *criaItem (int x) {
    Item *novo = (Item *) malloc (sizeof (Item));
    novo->chave = x;
    novo->pai = NULL;
    novo->esq  = NULL;
    novo->dir  = NULL;
    return novo;
}

Arvore *criaArvoreVazia () {
    Arvore *arv = (Arvore *) malloc (sizeof (Arvore));
    arv->raiz = NULL;
    arv->quantidade = 0;
    return arv;
}

void inserir (Arvore *arv, int x) {
    Item *novo = criaItem (x);
    Item *atual = arv->raiz;
    Item *anterior = NULL;

    while (atual != NULL) {
        anterior = atual;
        if (x < atual->chave)
            atual = atual->esq;
        else if (x > atual->chave)
            atual = atual->dir;
        else {
            free (novo);
            return;
        }
    }

    novo->pai = anterior;

    if (anterior == NULL)
        arv->raiz = novo;
    else if (x < anterior->chave)
        anterior->esq = novo;
    else
        anterior->dir = novo;

    arv->quantidade++;
}

int contarFolhas (Item *r) {
    if (r == NULL)
        return 0;
    if (r->esq == NULL && r->dir == NULL)
        return 1;
    return contarFolhas (r->esq) + contarFolhas (r->dir);
}

void liberaRecursivo (Item *item) {
    if (item == NULL) return;
    liberaRecursivo (item->esq);
    liberaRecursivo (item->dir);
    free (item);
}

void liberaArvore (Arvore *arvore) {
    liberaRecursivo (arvore->raiz);
    free (arvore);
}

int main () {
    int n, i, x;
    scanf ("%d", &n);
    Arvore *arv = criaArvoreVazia ();
    for (i = 0; i < n; i++) {
        scanf ("%d", &x);
        inserir (arv, x);
    }
    printf ("%d\n", contarFolhas (arv->raiz));
    liberaArvore (arv);
    return 0;
}