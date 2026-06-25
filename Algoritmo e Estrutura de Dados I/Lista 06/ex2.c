#include <stdio.h>
#include <stdlib.h>

typedef struct tItem {
  int chave;
  struct tItem *prox;
} Item;

typedef struct tFila {
  Item *inicio;
  Item *fim;
} Fila;

Fila *criaFilaVazia(void) {
  Fila *fila = malloc(sizeof(Fila));

  fila->inicio = NULL;
  fila->fim    = NULL;
  return fila;
}

Item *criaItem(int x) {
  Item *item = malloc(sizeof(Item));

  item->chave = x;
  item->prox  = NULL;
  
  return item;
}

int vazia(Fila *fila) {
  return fila->inicio == NULL;
}

void liberaFila(Fila *fila) {
  Item *cur = fila->inicio;
  
  while (cur != NULL) {
    Item *prox = cur->prox;
    free(cur);
    cur = prox;
  }
  
  free(fila);
}

void imprimir(Fila *fila) {
  Item *p = fila->inicio;
  int primeiro = 1;
  
  while (p != NULL) {
    if (!primeiro) {
      printf(" ");
    }
    
    printf("%d", p->chave);
    
    primeiro = 0;
    p = p->prox;
  }
  
  printf("\n");
}

void enfileirar(Fila *fila, int x) {
  Item *novo = criaItem(x);
  
  if (vazia(fila)) {
    fila->inicio = novo;
    fila->fim    = novo;
    return;
  }
  
  if (x % 2 == 0) {
    novo->prox   = fila->inicio;
    fila->inicio = novo;
    return;
  }
  
  fila->fim->prox = novo;
  fila->fim       = novo;
}

int desenfileirar(Fila *fila) {
  if (vazia(fila)) {
    return -1;
  }
  
  Item *rem = fila->inicio;
  int chave = rem->chave;
  
  fila->inicio = rem->prox;
  
  if (fila->inicio == NULL) {
    fila->fim = NULL;
  }
  
  free(rem);
  
  return chave;
}

int main(void) {
  Fila *fila = criaFilaVazia();
  char op;
  int x;
  
  while (scanf(" %c", &op) != EOF) {
    
    switch (op) {
      
      case 'E':
      if (scanf("%d", &x) == 1) {
        enfileirar(fila, x);
      }
      break;
      
      case 'D': {
        int r = desenfileirar(fila);
        
        if (r != -1) {
          printf("%d\n", r);
        }
        break;
      }
      
      case 'M':
      imprimir(fila);
      break;
      
      default:
      break;
    }
  }
  
  liberaFila(fila);
  return 0;
}