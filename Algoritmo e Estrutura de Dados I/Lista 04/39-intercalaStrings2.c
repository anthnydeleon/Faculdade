#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    char *v = (char *) malloc(sizeof(char) * (n + m));
    char *res = (char *) malloc(sizeof(char) * (n + m));

    for (int i = 0; i < n + m; i++)
        scanf(" %c", &v[i]);

    int i = 0, j = n, k = 0;

    while (i < n && j < n + m) {
        if (v[i] <= v[j])
            res[k++] = v[i++];
        else
            res[k++] = v[j++];
    }

    while (i < n)
        res[k++] = v[i++];

    while (j < n + m)
        res[k++] = v[j++];

    for (int t = 0; t < n + m; t++)
        printf("%c", res[t]);

    printf("\n");

    free(v);
    free(res);
    return 0;
}