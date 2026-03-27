#include <stdio.h>
#include <stdlib.h>

void imprimir(int *v, int n) {
    for (int i = 0; i < n; i++) {
        if (i) printf(" ");
        printf("%d", v[i]);
    }
    printf("\n");
}

void intercala(int *v, int e, int m, int d) {
    int n = d - e;
    int *aux = (int *) malloc(sizeof(int) * n);

    int i = e, j = m, k = 0;

    while (i < m && j < d) {
        if (v[i] <= v[j])
            aux[k++] = v[i++];
        else
            aux[k++] = v[j++];
    }

    while (i < m)
        aux[k++] = v[i++];

    while (j < d)
        aux[k++] = v[j++];

    imprimir(aux, n);

    for (int t = 0; t < n; t++)
        v[e + t] = aux[t];

    free(aux);
}

void mergeSortR(int *v, int e, int d) {
    if (d - e <= 1) return;

    int m = (e + d) / 2;

    mergeSortR(v, e, m);
    mergeSortR(v, m, d);
    intercala(v, e, m, d);
}

void mergeSort(int *v, int n) {
    mergeSortR(v, 0, n);
}

int main() {
    int n;
    scanf("%d", &n);

    int *v = (int *) malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);

    mergeSort(v, n);

    free(v);
    return 0;
}