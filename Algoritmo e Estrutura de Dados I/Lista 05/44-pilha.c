#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *itens;
    int topo;
    int tamanho;
} Pilha;

Pilha *criaPilhaVazia(int tamanho) {
    Pilha *p = (Pilha *) malloc(sizeof(Pilha));
    p->itens = (int *) malloc(sizeof(int) * tamanho);
    p->topo = -1;
    p->tamanho = tamanho;
    return p;
}

void liberaPilha(Pilha *pilha) {
    free(pilha->itens);
    free(pilha);
}

int cheia(Pilha *pilha) {
    return pilha->topo == pilha->tamanho - 1;
}

int vazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void empilha(Pilha *pilha, int x) {
    if (cheia(pilha)) return;
    pilha->itens[++pilha->topo] = x;
}

int desempilha(Pilha *pilha) {
    if (vazia(pilha)) return -1;
    return pilha->itens[pilha->topo--];
}

void imprimePilhaTopo(Pilha *pilha) {
    for (int i = pilha->topo; i >= 0; i--)
        printf("%d\n", pilha->itens[i]);
    printf("\n");
}

void imprimePilhaBase(Pilha *pilha) {
    for (int i = 0; i <= pilha->topo; i++)
        printf("%d\n", pilha->itens[i]);
    printf("\n");
}

int main() {
    int n, x;
    char op;

    scanf("%d", &n);
    Pilha *p = criaPilhaVazia(n);

    while (scanf(" %c", &op) != EOF) {
        if (op == 'E') {
            scanf("%d", &x);
            empilha(p, x);
        }
        else if (op == 'D') {
            desempilha(p);
        }
        else if (op == 'T') {
            imprimePilhaTopo(p);
        }
        else if (op == 'B') {
            imprimePilhaBase(p);
        }
    }

    liberaPilha(p);
    return 0;
}