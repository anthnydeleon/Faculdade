#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **itens;
    int topo;
    int tamanho;
} Pilha;

Pilha *criaPilha(int n) {
    Pilha *p = (Pilha *) malloc(sizeof(Pilha));
    p->itens = (char **) malloc(sizeof(char *) * n);

    for (int i = 0; i < n; i++)
        p->itens[i] = (char *) malloc(sizeof(char) * 100);

    p->topo = -1;
    p->tamanho = n;
    return p;
}

void liberaPilha(Pilha *p) {
    for (int i = 0; i < p->tamanho; i++)
        free(p->itens[i]);
    free(p->itens);
    free(p);
}

int cheia(Pilha *p) {
    return p->topo == p->tamanho - 1;
}

int vazia(Pilha *p) {
    return p->topo == -1;
}

void empilha(Pilha *p, char *x) {
    if (cheia(p)) return;
    strcpy(p->itens[++p->topo], x);
}

void desempilha(Pilha *p) {
    if (vazia(p)) return;
    p->topo--;
}

void topo(Pilha *p) {
    if (vazia(p)) return;
    printf("%s\n", p->itens[p->topo]);
}

void imprimeTopoBase(Pilha *p) {
    for (int i = p->topo; i >= 0; i--) {
        if (i != p->topo) printf(" ");
        printf("%s", p->itens[i]);
    }
    printf("\n");
}

void imprimeBaseTopo(Pilha *p) {
    for (int i = 0; i <= p->topo; i++) {
        if (i != 0) printf(" ");
        printf("%s", p->itens[i]);
    }
    printf("\n");
}

int main() {
    int n;
    char op;
    char str[100];

    scanf("%d", &n);
    Pilha *p = criaPilha(n);

    while (scanf(" %c", &op) != EOF) {
        if (op == 'E') {
            scanf("%s", str);
            empilha(p, str);
        }
        else if (op == 'D') {
            desempilha(p);
        }
        else if (op == 'T') {
            topo(p);
        }
        else if (op == 'X') {
            imprimeTopoBase(p);
        }
        else if (op == 'B') {
            imprimeBaseTopo(p);
        }
    }

    printf("\n");

    liberaPilha(p);
    return 0;
}