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

void imprimeInOrdem (Item *r) {
    if (r == NULL) return;
    imprimeInOrdem (r->esq);
    printf ("%d ", r->chave);
    imprimeInOrdem (r->dir);
}

void imprimir (Arvore *arv) {
    imprimeInOrdem (arv->raiz);
    printf ("\n");
}

void imprimeFolhasRecursivo (Item *r) {
    if (r == NULL) return;
    imprimeFolhasRecursivo (r->esq);
    if (r->esq == NULL && r->dir == NULL)
        printf ("%d ", r->chave);
    imprimeFolhasRecursivo (r->dir);
}

void imprimirFolhas (Arvore *arv) {
    printf ("Folhas: ");
    imprimeFolhasRecursivo (arv->raiz);
    printf ("\n");
}

void liberaRecursivo (Item *item) {
    if (item == NULL) return;
    liberaRecursivo (item->esq);
    liberaRecursivo (item->dir);
    printf ("libera: %d\n", item->chave);
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
    imprimir (arv);
    imprimirFolhas (arv);
    liberaArvore (arv);
    return 0;
}