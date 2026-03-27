#include <stdio.h>
#include <stdlib.h>

int main() {
    int q1, q2;
    scanf("%d %d", &q1, &q2);

    int *v = (int *) malloc(sizeof(int) * (q1 + q2));

    for (int i = 0; i < q1 + q2; i++)
        scanf("%d", &v[i]);

    int i = 0;
    int j = q1;

    while (i < q1 && j < q1 + q2) {
        if (v[i] <= v[j])
            printf("%d\n", v[i++]);
        else
            printf("%d\n", v[j++]);
    }

    while (i < q1)
        printf("%d\n", v[i++]);

    while (j < q1 + q2)
        printf("%d\n", v[j++]);

    free(v);
    return 0;
}