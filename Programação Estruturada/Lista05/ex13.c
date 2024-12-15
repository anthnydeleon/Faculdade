#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _aluno {
int ra;
char nome[1000];
char sexo;
int idade;
}* Aluno;

char aluno_cmp(Aluno* a, Aluno* b) {
  if ((*a)->ra == (*b)->ra && 
    strcmp((*a)->nome, (*b)->nome) == 0 && 
    (*a)->sexo == (*b)->sexo && 
    (*a)->idade == (*b)->idade) {
    return 1;
  }
  return 0;
}

void limpar_memoria(Aluno* a) {
  if (*a != NULL) {
    free(*a);
    *a = NULL;
  } 
}

void ler_aluno(Aluno* aluno) {
  printf("Digite o RA: ");
  scanf("%d", &(*aluno)->ra);
  getchar();

  printf("Digite o nome: ");
  scanf("%[^\n]", (*aluno)->nome);
  getchar();
  
  printf("Digite o sexo: ");
  scanf("%c", &(*aluno)->sexo);

  printf("Digite a idade: ");
  scanf("%d", &(*aluno)->idade);
}

int main() {
  Aluno aluno_a = malloc(sizeof(struct _aluno));
  ler_aluno(&aluno_a);
  printf("Aluno: %s\n", aluno_a->nome);

  Aluno aluno_b = malloc(sizeof(struct _aluno));
  ler_aluno(&aluno_b);
  printf("Aluno: %s\n", aluno_b->nome);

  (aluno_cmp(&aluno_a, &aluno_b)) ? printf("Os alunos sao iguais!\n") : printf("Os alunos sao diferentes!\n");

  limpar_memoria(&aluno_a);
  limpar_memoria(&aluno_b);
  
  return 0;
}