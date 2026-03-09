#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implemente a função char aluno_cmp(Aluno *a, Aluno *b) que recebe dois ponteiros para a
// estrutura Aluno e returna 1 se o conteúdo dos alunos apontados por a e b é o mesmo e 0, caso
// contrário. Uma observação importante: não estou pedindo para comparar os endereços de a e b, mas
// sim os conteúdos desses objetos.

typedef struct _aluno {
  int ra;
  char nome[1000];
  char sexo;
  int idade;
}* Aluno;

Aluno cria_aluno() {
  Aluno aluno;
  aluno = calloc(1, sizeof(struct _aluno));

  printf("Digite o RA: ");
  scanf("%d", &aluno->ra);
  getchar();

  printf("Digite o Nome: ");
  scanf("%[^\n]", aluno->nome);
  getchar();

  printf("Digite o Sexo: ");
  scanf("%c", &aluno->sexo);
  getchar();

  printf("Digite o Idade: ");
  scanf("%d", &aluno->idade);

  return aluno;
}

char aluno_cmp(Aluno *a, Aluno *b) {
  if ((*a)->ra == (*b)->ra && 
    (*a)->idade == (*b)->idade 
    && (*a)->sexo == (*b)->sexo 
    && strcmp((*a)->nome, (*b)->nome) == 0) return 1;

  return 0;  
}

void print_aluno(Aluno aluno) {
  printf("RA: %d\n", aluno->ra);
  printf("NOME: %s\n", aluno->nome);
  printf("SEXO: %c\n", aluno->sexo);
  printf("IDADE: %d\n", aluno->idade);
}

int main() {
  Aluno a = cria_aluno();
  Aluno b = cria_aluno();

  int result = aluno_cmp(&a, &b);

  (result == 1) ? printf("iguais") : printf("diferentes"); 

  free(a);
  free(b);

  return 0;
}