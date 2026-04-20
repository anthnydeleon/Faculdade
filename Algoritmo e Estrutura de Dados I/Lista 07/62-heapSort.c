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
    return (h->quantidade/2) - 1;
}

void peneirar (Heap *h, int pai) {
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
        peneirar (h, maior);
    }
}

void constroiHeap (Heap *h) {
    for (int i = ultimoPai (h); i >= 0; i--) {
        peneirar (h, i);
    }
}

void imprime (int *v, int n) {
    for (int i = 0; i < n; i++) {
        printf ("%d ", v[i]);
    }
    printf ("\n");
}

void heapSort (Heap *h) {
    constroiHeap (h);
    imprime (h->v, h->quantidade);
    for (int i = h->quantidade - 1; i > 0; i--) {
        int temp = h->v[0];
        h->v[0] = h->v[i];
        h->v[i] = temp;
        h->quantidade--;
        peneirar (h, 0);
        imprime (h->v, h->quantidade);
    }
}

int main () {
    int n, i;
    scanf ("%d", &n);
    Heap *h = criaHeapVazio (n);
    for (i = 0; i < n; i++) {
        scanf ("%d", &h->v[i]);
    }
    h->quantidade = n;
    heapSort (h);
    liberaHeap (h);
    return 0;
}