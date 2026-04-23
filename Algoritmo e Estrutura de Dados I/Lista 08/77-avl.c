#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
    int chave;
    int altura;
    int fb;
    struct tItem *pai, *esq, *dir;
} Item;

typedef struct tArvore {
    Item *raiz;
    int quantidade;
} Arvore;

Item *criaItem (int x) {
    Item *novo = (Item *) malloc (sizeof (Item));
    novo->chave = x;
    novo->altura = 0;
    novo->fb = 0;
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

int alturaNo (Item *r) {
    return (r == NULL) ? -1 : r->altura;
}

void atualizaAltura (Item *r) {
    if (r == NULL) return;
    int ae = alturaNo (r->esq);
    int ad = alturaNo (r->dir);
    r->altura = 1 + (ae > ad ? ae : ad);
    r->fb = ae - ad;
}

void atualizarTodos (Item *r) {
    if (r == NULL) return;
    atualizarTodos (r->esq);
    atualizarTodos (r->dir);
    atualizaAltura (r);
}

void rotacaoEsquerda (Arvore *arv, Item *x) {
    Item *y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULL)
        y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        arv->raiz = y;
    else if (x->pai->esq == x)
        x->pai->esq = y;
    else
        x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    atualizaAltura (x);
    atualizaAltura (y);
}

void rotacaoDireita (Arvore *arv, Item *y) {
    Item *x = y->esq;
    y->esq = x->dir;
    if (x->dir != NULL)
        x->dir->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL)
        arv->raiz = x;
    else if (y->pai->esq == y)
        y->pai->esq = x;
    else
        y->pai->dir = x;
    x->dir = y;
    y->pai = x;
    atualizaAltura (y);
    atualizaAltura (x);
}

Item *encontraDesbalanceado (Item *r) {
    if (r == NULL) return NULL;
    Item *esq = encontraDesbalanceado (r->esq);
    if (esq != NULL) return esq;
    if (r->fb > 1 || r->fb < -1) return r;
    return encontraDesbalanceado (r->dir);
}

void balancear (Arvore *arv) {
    atualizarTodos (arv->raiz);
    Item *des = encontraDesbalanceado (arv->raiz);
    if (des == NULL) return;

    printf ("Balanceamento: %d\n", des->chave);

    if (des->fb == -2 && des->dir->fb <= -1) {
        printf ("Caso 1\n");
        rotacaoEsquerda (arv, des);
    } else if (des->fb == -2 && des->dir->fb >= 1) {
        printf ("Caso 2\n");
        rotacaoDireita (arv, des->dir);
        rotacaoEsquerda (arv, des);
    } else if (des->fb == 2 && des->esq->fb >= 1) {
        printf ("Caso 3\n");
        rotacaoDireita (arv, des);
    } else if (des->fb == 2 && des->esq->fb <= -1) {
        printf ("Caso 4\n");
        rotacaoEsquerda (arv, des->esq);
        rotacaoDireita (arv, des);
    }

    atualizarTodos (arv->raiz);
}

void imprimeNo (Item *r) {
    printf ("%d[h=%d; fb=%d;", r->chave, r->altura, r->fb);
    if (r->pai != NULL) printf (" pai=%d;", r->pai->chave);
    if (r->esq != NULL) printf (" esq=%d;", r->esq->chave);
    if (r->dir != NULL) printf (" dir=%d;", r->dir->chave);
    printf ("]\n");
}

void imprimeInOrdem (Item *r) {
    if (r == NULL) return;
    imprimeInOrdem (r->esq);
    imprimeNo (r);
    imprimeInOrdem (r->dir);
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
    atualizarTodos (arv->raiz);
    imprimeInOrdem (arv->raiz);
    balancear (arv);
    imprimeInOrdem (arv->raiz);
    liberaArvore (arv);
    return 0;
}