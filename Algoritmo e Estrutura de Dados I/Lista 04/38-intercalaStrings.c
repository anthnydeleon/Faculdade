#include <stdio.h>
#include <stdlib.h>

int main() {
    int n1, n2;

    scanf("%d", &n1);
    char *s1 = (char *) malloc(sizeof(char) * n1);

    for (int i = 0; i < n1; i++)
        scanf(" %c", &s1[i]);

    scanf("%d", &n2);
    char *s2 = (char *) malloc(sizeof(char) * n2);

    for (int i = 0; i < n2; i++)
        scanf(" %c", &s2[i]);

    int i = 0, j = 0;

    while (i < n1 && j < n2) {
        if (s1[i] <= s2[j])
            printf("%c", s1[i++]);
        else
            printf("%c", s2[j++]);
    }

    while (i < n1)
        printf("%c", s1[i++]);

    while (j < n2)
        printf("%c", s2[j++]);

    printf("\n");

    free(s1);
    free(s2);
    return 0;
}