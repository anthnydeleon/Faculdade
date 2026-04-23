#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tItem {
    int chave;
    char palavra[20];
    struct tItem *pai, *esq, *dir;
} Item;

typedef struct tArvore {
    Item *raiz;
    int quantidade;
} Arvore;

int calcularChave (char *palavra) {
    int soma = 0;
    for (int i = 0; palavra[i] != '\0'; i++)
        soma += (int) palavra[i];
    return soma;
}

Item *criaItem (int chave, char *palavra) {
    Item *novo = (Item *) malloc (sizeof (Item));
    novo->chave = chave;
    strcpy (novo->palavra, palavra);
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

void inserir (Arvore *arv, char *palavra) {
    int chave = calcularChave (palavra);
    Item *novo = criaItem (chave, palavra);
    Item *atual = arv->raiz;
    Item *anterior = NULL;

    while (atual != NULL) {
        anterior = atual;
        if (chave < atual->chave)
            atual = atual->esq;
        else if (chave > atual->chave)
            atual = atual->dir;
        else {
            free (novo);
            return;
        }
    }

    novo->pai = anterior;

    if (anterior == NULL)
        arv->raiz = novo;
    else if (chave < anterior->chave)
        anterior->esq = novo;
    else
        anterior->dir = novo;

    arv->quantidade++;
}

Item *buscar (Item *raiz, int chave) {
    if (raiz == NULL)
        return NULL;
    if (chave == raiz->chave)
        return raiz;
    if (chave < raiz->chave)
        return buscar (raiz->esq, chave);
    return buscar (raiz->dir, chave);
}

Item *sucessor (Item *i) {
    if (i->dir == NULL)
        return NULL;
    Item *atual = i->dir;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

void remover (Arvore *arv, char *palavra) {
    int chave = calcularChave (palavra);
    Item *it = buscar (arv->raiz, chave);
    if (it == NULL)
        return;

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
        strcpy (it->palavra, suc->palavra);
        if (suc->pai->esq == suc)
            suc->pai->esq = suc->dir;
        else
            suc->pai->dir = suc->dir;
        if (suc->dir != NULL)
            suc->dir->pai = suc->pai;
        free (suc);
    }

    arv->quantidade--;
}

void preOrdem (Item *r) {
    if (r == NULL) return;
    printf ("[%d]:%s\n", r->chave, r->palavra);
    preOrdem (r->esq);
    preOrdem (r->dir);
}

void inOrdem (Item *r) {
    if (r == NULL) return;
    inOrdem (r->esq);
    printf ("[%d]:%s\n", r->chave, r->palavra);
    inOrdem (r->dir);
}

void posOrdem (Item *r) {
    if (r == NULL) return;
    posOrdem (r->esq);
    posOrdem (r->dir);
    printf ("[%d]:%s\n", r->chave, r->palavra);
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
    char palavra[20];
    Arvore *arv = criaArvoreVazia ();
    while (scanf ("%s", op) != EOF) {
        if (strcmp (op, "insert") == 0) {
            scanf ("%s", palavra);
            inserir (arv, palavra);
        }
        else if (strcmp (op, "delete") == 0) {
            scanf ("%s", palavra);
            remover (arv, palavra);
        }
        else if (strcmp (op, "pre-order") == 0) {
            preOrdem (arv->raiz);
            printf ("\n");
        }
        else if (strcmp (op, "in-order") == 0) {
            inOrdem (arv->raiz);
            printf ("\n");
        }
        else if (strcmp (op, "post-order") == 0) {
            posOrdem (arv->raiz);
            printf ("\n");
        }
    }
    printf ("\n");
    liberaArvore (arv);
    return 0;
}