#include <stdio.h>
#include <stdlib.h>

typedef struct tHeap {
    int *v;
    int tamanho;
    int quantidade;
} Heap;

Heap *criaHeapVazio (int tam) {
    Heap *h = (Heap *) malloc (sizeof (Heap));
    h->v = (int *) malloc (tam * sizeof (int));
    h->tamanho = tam;
    h->quantidade = 0;
    return h;
}

void liberaHeap (Heap *h) {
    free (h->v);
    free (h);
}

int pai (int i) {
    return (i-1)/2;
}

int filhoDir (int i) {
    return 2*i + 2;
}

int ultimoPai (Heap *h) {
    return pai (h->quantidade-1);
}

void desceChave (Heap *h, int pai) {
    int filhoE = 2*pai + 1;
    int filhoD = 2*pai + 2;
    int maior = pai;
    if (filhoE < h->quantidade && h->v[filhoE] > h->v[maior]) {
        maior = filhoE;
    }
    if (filhoD < h->quantidade && h->v[filhoD] > h->v[maior]) {
        maior = filhoD;
    }
    if (maior != pai) {
        int temp = h->v[pai];
        h->v[pai] = h->v[maior];
        h->v[maior] = temp;
        desceChave (h, maior);
    }
}

void constroiHeap (Heap *h) {
    for (int i = ultimoPai (h); i >= 0; i--) {
        desceChave (h, i);
    }
}

void imprime (int *v, int n) {
    for (int i = 0; i < n; i++) {
        printf ("%d ", v[i]);
    }
    printf ("\n");
}

int vazio (Heap *h) {
    return h->quantidade == 0;
}

int cheio (Heap *h) {
    return h->quantidade == h->tamanho;
}

void sobeChave (Heap *h, int i) {
    if (i > 0) {
        int p = pai (i);
        if (h->v[i] > h->v[p]) {
            int temp = h->v[i];
            h->v[i] = h->v[p];
            h->v[p] = temp;
            sobeChave (h, p);
        }
    }
}

void inserir (Heap *h, int x) {
    if (!cheio (h)) {
        h->v[h->quantidade] = x;
        sobeChave (h, h->quantidade);
        h->quantidade++;
    }
}

int extraiMaximo (Heap *h) {
    int max = h->v[0];
    h->v[0] = h->v[h->quantidade-1];
    h->quantidade--;
    desceChave (h, 0);
    return max;
}

void remover (Heap *h, int x) {
    int i;
    for (i = 0; i < h->quantidade; i++) {
        if (h->v[i] == x) {
            break;
        }
    }
    if (i < h->quantidade) {
        h->v[i] = h->v[h->quantidade-1];
        h->quantidade--;
        desceChave (h, i);
    }
}

int main () {
    int tam;
    scanf ("%d", &tam);
    Heap *h = criaHeapVazio (tam);
    char op;
    int x;
    while (scanf (" %c", &op) != EOF) {
        switch (op) {
            case 'I': scanf ("%d", &x); 
                      inserir (h, x);
                      break;
            case 'R': scanf ("%d", &x); 
                      remover (h, x);
                      break;
            case 'E': if (!vazio (h)) {
                          x = extraiMaximo (h);
                          printf ("[%d]\n", x);
                      }
                      break;
            case 'P': if (!vazio (h)) {
                          imprime (h->v, h->quantidade);
                      }
                      break;
        }
    }
    liberaHeap (h);
    return 0;
}