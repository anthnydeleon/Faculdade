#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Item *buscar (Arvore *arv, int x) {
    Item *atual = arv->raiz;
    while (atual != NULL) {
        if (x < atual->chave)
            atual = atual->esq;
        else if (x > atual->chave)
            atual = atual->dir;
        else
            return atual;
    }
    return NULL;
}

Item *sucessor (Item *i) {
    if (i->dir == NULL)
        return NULL;
    Item *atual = i->dir;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

int remover (Arvore *arv, int x) {
    Item *it = buscar (arv, x);
    if (it == NULL)
        return 0;

    if (it->esq == NULL && it->dir == NULL) {
        if (it->pai == NULL)
            arv->raiz = NULL;
        else if (it->pai->esq == it)
            it->pai->esq = NULL;
        else
            it->pai->dir = NULL;
        free (it);

    } else if (it->esq == NULL || it->dir == NULL) {
        Item *filho = (it->esq != NULL) ? it->esq : it->dir;
        filho->pai = it->pai;
        if (it->pai == NULL)
            arv->raiz = filho;
        else if (it->pai->esq == it)
            it->pai->esq = filho;
        else
            it->pai->dir = filho;
        free (it);

    } else {
        Item *suc = sucessor (it);
        it->chave = suc->chave;
        if (suc->pai->esq == suc)
            suc->pai->esq = suc->dir;
        else
            suc->pai->dir = suc->dir;
        if (suc->dir != NULL)
            suc->dir->pai = suc->pai;
        free (suc);
    }

    arv->quantidade--;
    return 1;
}

void preOrdem (Item *r) {
    if (r == NULL) return;
    printf ("%d ", r->chave);
    preOrdem (r->esq);
    preOrdem (r->dir);
}

void inOrdem (Item *r) {
    if (r == NULL) return;
    inOrdem (r->esq);
    printf ("%d ", r->chave);
    inOrdem (r->dir);
}

void posOrdem (Item *r) {
    if (r == NULL) return;
    posOrdem (r->esq);
    posOrdem (r->dir);
    printf ("%d ", r->chave);
}

void imprimirPreOrdem (Arvore *arv) {
    preOrdem (arv->raiz);
    printf ("\n");
}

void imprimirInOrdem (Arvore *arv) {
    inOrdem (arv->raiz);
    printf ("\n");
}

void imprimirPosOrdem (Arvore *arv) {
    posOrdem (arv->raiz);
    printf ("\n");
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
    char op[20];
    int x;
    Arvore *arv = criaArvoreVazia ();
    while (scanf ("%s", op) != EOF) {
        if (strcmp (op, "insert") == 0) {
            scanf ("%d", &x);
            inserir (arv, x);
        }
        else if (strcmp (op, "delete") == 0) {
            scanf ("%d", &x);
            if (remover (arv, x))
                printf ("%d\n", x);
        }
        else if (strcmp (op, "pre-order") == 0)
            imprimirPreOrdem (arv);
        else if (strcmp (op, "in-order") == 0)
            imprimirInOrdem (arv);
        else if (strcmp (op, "post-order") == 0)
            imprimirPosOrdem (arv);
    }
    printf ("\n");
    liberaArvore (arv);
    return 0;
}