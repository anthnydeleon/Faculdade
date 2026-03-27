#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *itens;
    int quantidade;
    int tamanho;
} Lista;

Lista *criaLista(int n) {
    Lista *l = (Lista *) malloc(sizeof(Lista));
    l->itens = (int *) malloc(sizeof(int) * n);
    l->quantidade = 0;
    l->tamanho = n;
    return l;
}

void liberaLista(Lista *l) {
    free(l->itens);
    free(l);
}

int busca(Lista *l, int x) {
    int ini = 0, fim = l->quantidade - 1;

    while (ini <= fim) {
        int mid = (ini + fim) / 2;
        if (l->itens[mid] == x) return 1;
        else if (l->itens[mid] < x) ini = mid + 1;
        else fim = mid - 1;
    }
    return 0;
}

void inserir(Lista *l, int x) {
    if (l->quantidade == l->tamanho || busca(l, x)) return;

    int i = l->quantidade - 1;

    while (i >= 0 && l->itens[i] > x) {
        l->itens[i + 1] = l->itens[i];
        i--;
    }

    l->itens[i + 1] = x;
    l->quantidade++;
}

void remover(Lista *l, int x) {
    int ini = 0, fim = l->quantidade - 1, pos = -1;

    while (ini <= fim) {
        int mid = (ini + fim) / 2;
        if (l->itens[mid] == x) {
            pos = mid;
            break;
        } else if (l->itens[mid] < x)
            ini = mid + 1;
        else
            fim = mid - 1;
    }

    if (pos == -1) return;

    for (int i = pos; i < l->quantidade - 1; i++)
        l->itens[i] = l->itens[i + 1];

    l->quantidade--;
}

void mostrar(Lista *l) {
    for (int i = 0; i < l->quantidade; i++) {
        if (i) printf(" ");
        printf("%d", l->itens[i]);
    }
    printf("\n");
}

int main() {
    int n, x;
    char op;

    scanf("%d", &n);
    Lista *l = criaLista(n);

    while (scanf(" %c", &op) != EOF) {
        if (op == 'I') {
            scanf("%d", &x);
            inserir(l, x);
        }
        else if (op == 'R') {
            scanf("%d", &x);
            remover(l, x);
        }
        else if (op == 'B') {
            scanf("%d", &x);
            if (busca(l, x)) printf("SIM\n");
            else printf("NAO\n");
        }
        else if (op == 'M') {
            mostrar(l);
        }
    }

    printf("\n");

    liberaLista(l);
    return 0;
}