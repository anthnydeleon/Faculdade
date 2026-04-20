// Descrição

// Esta atividade é para te ajudar a ter um melhor entendimento das operações básicas de heap de mínimo. 

// Você receberá requisições de inserção, remoção e impressão.

 

// typedef struct tHeap {
//     int *v;
//     int tamanho;    // tamanho do vetor alocado
//     int quantidade; // qtde elementos inseridos
// } Heap;

// // Recebe o tamanho do vetor para o heap.
// // Aloca um heap, aloca o vetor do heap, ajusta o tamanho,
// // a quantidade inicia com zero.
// // Devolve um ponteiro para o heap recem-criado.
// Heap *criaHeapVazio (int tam);

// // Recebe um ponteiro para um heap.
// // Libera o vetor e depois o ponteiro do heap.
// void liberaHeap (Heap *h);

// // Devolve o indice do pai de i.
// int pai (int i);

// // Devolve o indice do filho direito de i.
// int filhoDir (int i);

// // Recebe um ponteiro para um heap.
// // Devolve o indice do ultimo pai: chao(n/2)-1,
// // onde n é a quantidade de elementos do heap.
// int ultimoPai (Heap *h);

// // Recebe um ponteiro para um heap e um indice pai.
// // Se a chave do pai eh maior que a de algum filho,
// // faz a troca e chama recursivamente.
// // A troca deve ser feita pela chave do menor filho.
// // No caso de empate, prefira trocar com o filho esquerdo.
// void desceChave (Heap *h, int pai);

// // Recebe um ponteiro para um heap.
// // Constroi um heap, chamando desceChave() para 
// // cada um dos pais, do ultimo pai ate a raiz.
// void constroiHeap (Heap *h);

// // Recebe um vetor e o seu tamanho.
// // Imprime o conteudo do vetor.
// void imprime (int *v, int n);

// // Recebe um ponteiro para um heap.
// // Devolve 1 se heap vazio, 0 caso contrario.
// int vazio (Heap *h);

// // Recebe um ponteiro para um heap.
// // Devolve 1 se heap cheio, 0 caso contrario.
// int cheio (Heap *h);

// // Apos inserir uma nova chave em um heap, pode ser necessario 
// // "subir" esta nova chave para manter a propriedade estrutural do heap.
// // Este procedimento recursivo recebe o ponteiro para o heap e o indice i.
// void sobeChave (Heap *h, int i);

// // Recebe um ponteiro para um heap e uma chave x.
// // Insere a chave no fim do vetor e chama sobeChave().
// void inserir (Heap *h, int x);

// // Recebe um ponteiro para um heap.
// // Assume que o heap nao estah vazio.
// // Remove a chave da raiz e devolve o seu valor.
// // Para remover a raiz, troque as chaves da raiz com o ultimo elemento.
// // Depois, chame desceChave() para a nova raiz.
// int extraiMinimo (Heap *h);

// // Recebe um ponteiro para um heap e uma chave x.
// // Procura o indice i da chave x.
// // Se encontrar, remove x do heap.
// // Para remover x, troque as chaves de i com o ultimo elemento.
// // Depois, chame desceChave() para i.
// void remover (Heap *h, int x);
 
 

// Entrada

 

 

// A primeira linha contém um inteiro positivo representando o tamanho do vetor do heap a ser alocado.

 

// A seguir, o programa receberá uma sequência indefinida de comandos no seguinte formato:
// OPERAÇÃO VALOR

// Onde OPERAÇÃO será um dos seguintes caracteres:

// Caractere    Descrição
//  I <valor>       Inserção do valor no Heap
//  R <valor>     Remoção do valor do Heap
//  E                   Extrai (remove) a chave mínima do Heap e imprime o seu valor entre [ ] 
//  P                   Imprime os elementos do heap separados por espaço em branco

// E VALOR será um inteiro.

 

 

 

// Saída

 

// A cada requisição do tipo 3, mostre o valor mínimo e salte uma linha.

// int main () {
//     int tam;
//     scanf ("%d", &tam);
//     Heap *h = criaHeapVazio (tam);
//     char op;
//     int x;
//     while (scanf (" %c", &op) != EOF) {
//         switch (op) {
//             case 'I': scanf ("%d", &x); 
//                       inserir (h, x);
//                       break;
//             case 'R': scanf ("%d", &x); 
//                       remover (h, x);
//                       break;
//             case 'E': if (!vazio (h)) {
//                           x = extraiMinimo (h);
//                           printf ("[%d]\n", x);
//                       }
//                       break;
//             case 'P': if (!vazio (h)) {
//                           imprime (h->v, h->quantidade);
//                       }
//                       break;
//         }
//     }
//     liberaHeap (h);
//     return 0;
// }
 

// Exemplo 

// Entrada: 
// 10
// I 50
// P
// I 30
// P
// I 70
// P
// I 90
// P
// I 10
// P
// I 20
// P
// I 40
// P
// I 80
// P
// I 60
// P
// I 50
// P
// E
// P
// R 50
// P
// E
// P
// E
// P
// R 90
// P
// E
// P
// E
// P
// E
// P
// E
// P
// E
// P
// I 111
// P
// I 222
// P
// I 5
// P

// Saída:
// 50
// 30 50
// 30 50 70
// 30 50 70 90
// 10 30 70 90 50
// 10 30 20 90 50 70
// 10 30 20 90 50 70 40
// 10 30 20 80 50 70 40 90
// 10 30 20 60 50 70 40 90 80
// 10 30 20 60 50 70 40 90 80 50
// [10]
// 20 30 40 60 50 70 50 90 80
// 20 30 40 60 80 70 50 90
// [20]
// 30 60 40 90 80 70 50
// [30]
// 40 60 50 90 80 70
// 40 60 50 70 80
// [40]
// 50 60 80 70
// [50]
// 60 70 80
// [60]
// 70 80
// [70]
// 80
// [80]
// 111
// 111 222
// 5 222 111

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
    return pai (h->quantidade-1);
}

void desceChave (Heap *h, int pai) {
    int filhoE = 2*pai + 1;
    int filhoD = 2*pai + 2;
    int menor = pai;
    if (filhoE < h->quantidade && h->v[filhoE] < h->v[menor]) {
        menor = filhoE;
    }
    if (filhoD < h->quantidade && h->v[filhoD] < h->v[menor]) {
        menor = filhoD;
    }
    if (menor != pai) {
        int temp = h->v[pai];
        h->v[pai] = h->v[menor];
        h->v[menor] = temp;
        desceChave (h, menor);
    }
}

void constroiHeap (Heap *h) {
    for (int i = ultimoPai (h); i >= 0; i--) {
        desceChave (h, i);
    }
}

void imprime (int *v, int n) {
    for (int i = 0; i < n; i++) {
        printf ("%d ", v[i]);
    }
    printf ("\n");
}

int vazio (Heap *h) {
    return h->quantidade == 0;
}

int cheio (Heap *h) {
    return h->quantidade == h->tamanho;
}

void sobeChave (Heap *h, int i) {
    if (i > 0) {
        int p = pai (i);
        if (h->v[i] < h->v[p]) {
            int temp = h->v[i];
            h->v[i] = h->v[p];
            h->v[p] = temp;
            sobeChave (h, p);
        }
    }
}

void inserir (Heap *h, int x) {
    if (!cheio (h)) {
        h->v[h->quantidade] = x;
        sobeChave (h, h->quantidade);
        h->quantidade++;
    }
}

int extraiMinimo (Heap *h) {
    int min = h->v[0];
    h->v[0] = h->v[h->quantidade-1];
    h->quantidade--;
    desceChave (h, 0);
    return min;
}

void remover (Heap *h, int x) {
    int i;
    for (i = 0; i < h->quantidade; i++) {
        if (h->v[i] == x) {
            break;
        }
    }
    if (i < h->quantidade) {
        h->v[i] = h->v[h->quantidade-1];
        h->quantidade--;
        desceChave (h, i);
    }
}

int main () {
    int tam;
    scanf ("%d", &tam);
    Heap *h = criaHeapVazio (tam);
    char op;
    int x;
    while (scanf (" %c", &op) != EOF) {
        switch (op) {
            case 'I': scanf ("%d", &x); 
                      inserir (h, x);
                      break;
            case 'R': scanf ("%d", &x); 
                      remover (h, x);
                      break;
            case 'E': if (!vazio (h)) {
                          x = extraiMinimo (h);
                          printf ("[%d]\n", x);
                      }
                      break;
            case 'P': if (!vazio (h)) {
                          imprime (h->v, h->quantidade);
                      }
                      break;
        }
    }
    liberaHeap (h);
    return 0;
}