#include <stdio.h>
#include <string.h>

#define MAX 210

typedef struct {
    char num[MAX];
} tInteiro;

void normaliza(char *s) {
    int i = 0;
    while (s[i] == '0' && s[i+1] != '\0') i++;
    if (i > 0) memmove(s, s + i, strlen(s) - i + 1);
}

tInteiro inteiro(char *s) {
    tInteiro x;
    strcpy(x.num, s);
    normaliza(x.num);
    return x;
}


tInteiro soma(tInteiro a, tInteiro b) {
    char r[MAX];
    int i = strlen(a.num) - 1, j = strlen(b.num) - 1, k = 0, carry = 0;

    while (i >= 0 || j >= 0 || carry) {
        int da = (i >= 0) ? a.num[i--] - '0' : 0;
        int db = (j >= 0) ? b.num[j--] - '0' : 0;
        int s = da + db + carry;
        r[k++] = (s % 10) + '0';
        carry = s / 10;
    }

    for (int l = 0; l < k/2; l++) {
        char tmp = r[l];
        r[l] = r[k-1-l];
        r[k-1-l] = tmp;
    }
    r[k] = '\0';

    return inteiro(r);
}

int cmp(tInteiro a, tInteiro b) {
    int la = strlen(a.num), lb = strlen(b.num);
    if (la != lb) return (la > lb) ? 1 : -1;
    return strcmp(a.num, b.num);
}

tInteiro subAbs(tInteiro a, tInteiro b) {
    char r[MAX];
    int i = strlen(a.num) - 1, j = strlen(b.num) - 1, k = 0, borrow = 0;

    while (i >= 0) {
        int da = a.num[i--] - '0';
        int db = (j >= 0) ? b.num[j--] - '0' : 0;

        da -= borrow;
        if (da < db) {
            da += 10;
            borrow = 1;
        } else borrow = 0;

        r[k++] = (da - db) + '0';
    }

    while (k > 1 && r[k-1] == '0') k--;

    for (int l = 0; l < k/2; l++) {
        char tmp = r[l];
        r[l] = r[k-1-l];
        r[k-1-l] = tmp;
    }
    r[k] = '\0';

    return inteiro(r);
}

tInteiro subtrai(tInteiro a, tInteiro b) {
    if (cmp(a, b) >= 0)
        return subAbs(a, b);
    else {
        tInteiro r = subAbs(b, a);
        char temp[MAX];
        sprintf(temp, "-%s", r.num);
        return inteiro(temp);
    }
}

tInteiro multiplica(tInteiro a, tInteiro b) {
    int la = strlen(a.num), lb = strlen(b.num);
    int res[MAX] = {0};

    for (int i = la - 1; i >= 0; i--) {
        for (int j = lb - 1; j >= 0; j--) {
            int p = (a.num[i] - '0') * (b.num[j] - '0');
            int pos = (la - 1 - i) + (lb - 1 - j);
            res[pos] += p;
        }
    }

    for (int i = 0; i < MAX - 1; i++) {
        res[i+1] += res[i] / 10;
        res[i] %= 10;
    }

    int k = MAX - 1;
    while (k > 0 && res[k] == 0) k--;

    char r[MAX];
    int idx = 0;
    for (int i = k; i >= 0; i--)
        r[idx++] = res[i] + '0';
    r[idx] = '\0';

    return inteiro(r);
}

int main() {
    char n1[110], n2[110], op;

    while (scanf("%s %c %s", n1, &op, n2) == 3) {
        tInteiro a = inteiro(n1);
        tInteiro b = inteiro(n2);
        tInteiro r;

        if (op == '+') r = soma(a, b);
        else if (op == '-') r = subtrai(a, b);
        else r = multiplica(a, b);

        printf("%s\n", r.num);
    }

    return 0;
}