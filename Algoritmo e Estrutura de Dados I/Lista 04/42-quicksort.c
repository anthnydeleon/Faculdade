#include <stdio.h>
#include <stdlib.h>

void imprimir(int *v, int e, int d) {
    for (int i = e; i < d; i++) {
        if (i > e) printf(" ");
        printf("%d", v[i]);
    }
    printf("\n");
}

void troca(int *v, int i, int j) {
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
}

int particiona(int *v, int e, int d, int p) {
    int pivot = v[p];
    troca(v, p, d - 1);

    int i = e;
    for (int j = e; j < d - 1; j++) {
        if (v[j] <= pivot) {
            troca(v, i, j);
            i++;
        }
    }

    troca(v, i, d - 1);
    return i;
}

void quickSortR(int *v, int e, int d) {
    if (d - e <= 1) return;

    int p = d - 1;
    int pos = particiona(v, e, d, p);

    printf("Pivo: [%d] %d\n", pos, v[pos]);
    imprimir(v, e, d);

    quickSortR(v, e, pos);
    quickSortR(v, pos + 1, d);
}

void quickSort(int *v, int n) {
    quickSortR(v, 0, n);
}

int main() {
    int n;
    scanf("%d", &n);

    int *v = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);

    quickSort(v, n);

    imprimir(v, 0, n);

    free(v);
    return 0;
}