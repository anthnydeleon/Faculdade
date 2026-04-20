#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ATLETAS 5000
#define MAX_NOME 100
#define MAX_SERIES 200

typedef struct {
    char nome[MAX_NOME];
    int min, seg, cent;
    int semTempo;
    int tempoTotal;
    int posicao;
    int raia;
} Atleta;

typedef struct {
    int atletas[MAX_ATLETAS];
    int tam;
} Serie;

Atleta atletas[MAX_ATLETAS];
Serie  series[MAX_SERIES];

void lerAtleta(Atleta *a) {
    scanf("%s %d %d %d", a->nome, &a->min, &a->seg, &a->cent);
    if (a->min == 0 && a->seg == 0 && a->cent == 0) {
        a->semTempo = 1;
        a->tempoTotal = 0;
    } else {
        a->semTempo = 0;
        a->tempoTotal = a->min * 6000 + a->seg * 100 + a->cent;
    }
}

void determinarPosicoes(int M) {
    for (int i = 0; i < M - 1; i++) {
        for (int j = 0; j < M - i - 1; j++) {
            Atleta *a = &atletas[j];
            Atleta *b = &atletas[j + 1];
            int trocar = 0;
            if (a->semTempo && !b->semTempo) trocar = 1;
            else if (!a->semTempo && !b->semTempo && a->tempoTotal > b->tempoTotal) trocar = 1;
            if (trocar) {
                Atleta temp = atletas[j];
                atletas[j] = atletas[j + 1];
                atletas[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < M; i++)
        atletas[i].posicao = i + 1;
}

void calcularSeries(int M, int N, int *numSeries) {
    *numSeries = (M + N - 1) / N;

    int primeiraSerie = M - (*numSeries - 1) * N;
    if (*numSeries > 1 && primeiraSerie < 3)
        primeiraSerie = 3;

    int tamanhos[MAX_SERIES];
    tamanhos[0] = primeiraSerie;

    int restantes = M - primeiraSerie;
    for (int i = *numSeries - 1; i >= 1; i--) {
        tamanhos[i] = (restantes < N) ? restantes : N;
        restantes -= tamanhos[i];
    }

    int offset = M;
    for (int s = 0; s < *numSeries; s++) {
        series[s].tam = tamanhos[s];
        offset -= tamanhos[s];
        for (int i = 0; i < tamanhos[s]; i++)
            series[s].atletas[i] = offset + i;
    }
}

void imprimirResultado(int numSeries, int N) {
    if (numSeries == 1)
        printf("%d serie\n", numSeries);
    else
        printf("%d series\n", numSeries);

    int central = (N + 1) / 2;

    for (int s = 0; s < numSeries; s++) {
        printf("%da. serie:\n", s + 1);
        int tam = series[s].tam;

        int raias[MAX_ATLETAS];
        raias[0] = central;
        int esq = central - 1;
        int dir = central + 1;
        for (int r = 1; r < tam; r++) {
            if (r % 2 == 1) raias[r] = dir++;
            else             raias[r] = esq--;
        }

        for (int r = 0; r < tam; r++) {
            int idx = series[s].atletas[r];
            printf("Raia %d: %s\n", raias[r], atletas[idx].nome);
        }
    }
    printf("\n");
}

int main() {
    int N, M;
    while (scanf("%d", &N) == 1 && scanf("%d", &M) == 1) {
        for (int i = 0; i < M; i++)
            lerAtleta(&atletas[i]);

        int numSeries;
        determinarPosicoes(M);
        calcularSeries(M, N, &numSeries);
        imprimirResultado(numSeries, N);
    }
    return 0;
}