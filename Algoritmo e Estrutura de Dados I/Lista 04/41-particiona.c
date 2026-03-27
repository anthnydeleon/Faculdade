#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, p;
    scanf("%d %d", &n, &p);

    int *v = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);

    int pivot = v[p];
    int temp = v[p];
    v[p] = v[n-1];
    v[n-1] = temp;

    int i = 0;

    for (int j = 0; j < n-1; j++) {
        if (v[j] < pivot) {
            int t = v[i];
            v[i] = v[j];
            v[j] = t;
            i++;
        }
    }

    v[n-1] = v[i];
    v[i] = pivot;

    printf("%d\n", i);

    for (int k = 0; k < n; k++) {
        if (k) printf(" ");
        printf("%d", v[k]);
    }
    printf("\n");

    free(v);
    return 0;
}