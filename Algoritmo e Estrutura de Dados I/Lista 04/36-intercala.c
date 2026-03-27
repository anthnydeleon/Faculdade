#include <stdio.h>
#include <stdlib.h>

int main() {
    int q1, q2;
    scanf("%d %d", &q1, &q2);

    int *v1 = (int *) malloc(sizeof(int) * q1);
    int *v2 = (int *) malloc(sizeof(int) * q2);

    for (int i = 0; i < q1; i++)
        scanf("%d", &v1[i]);

    for (int i = 0; i < q2; i++)
        scanf("%d", &v2[i]);

    int i = 0, j = 0;

    while (i < q1 && j < q2) {
        if (v1[i] <= v2[j])
            printf("%d\n", v1[i++]);
        else
            printf("%d\n", v2[j++]);
    }

    while (i < q1)
        printf("%d\n", v1[i++]);

    while (j < q2)
        printf("%d\n", v2[j++]);

    free(v1);
    free(v2);

    return 0;
}