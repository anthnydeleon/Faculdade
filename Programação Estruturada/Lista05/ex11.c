#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Questão 11. Crie um TAD Turma para armazenar o nome de todos os alunos de uma turma. Para
// manipular esse tipo, você deverá implementar as seguintes funções:
// 1. turma_matricula(turma, nome) essa função recebe o nome de um aluno a ser inserido na turma;
// 2. turma_lista(turma) essa função lista o nome de todos os alunos matriculados em turma.
// 3. turma_jubila(turma, nome) essa função remove o aluno nome da turma.

typedef struct {
  char** alunos;
  int quantidade_de_alunos;
  int capacidade;
} *Turma;

Turma criar_turma(void) {
  Turma turma = malloc(sizeof(*turma));
  turma->capacidade = 1;
  turma->quantidade_de_alunos = 0;
  turma->alunos = malloc(turma->capacidade * sizeof(char*));
  return turma;
}

void turma_matricula(Turma turma, char* nome) {
  if (turma->quantidade_de_alunos == turma->capacidade) {
      int nova_capacidade = turma->capacidade * 2;
      turma->alunos = realloc(turma->alunos, nova_capacidade * sizeof(char*));
      turma->capacidade = nova_capacidade;
  }
  turma->alunos[turma->quantidade_de_alunos] = malloc((strlen(nome) + 1) * sizeof(char));
  strcpy(turma->alunos[turma->quantidade_de_alunos], nome);
  turma->quantidade_de_alunos++;
}

void turma_lista(Turma turma) {
  printf("Lista da turma: \n");
  for (int i = 0; i < turma->quantidade_de_alunos; i++) {
    printf("%s\n", turma->alunos[i]);
  }
}

void turma_jubila(Turma turma, char* nome) {
  for (int i = 0; i < turma->quantidade_de_alunos; i++) {
    if (strcmp(turma->alunos[i], nome) == 0) {
      for (int j = i; j < turma->quantidade_de_alunos-1; j++) {
        turma->alunos[j] = turma->alunos[j+1]; 
      }
      turma->quantidade_de_alunos--;
      i--;
    }
  }

  if (turma->quantidade_de_alunos <= turma->capacidade / 2 && turma->capacidade > 1) {
    int nova_capacidade = turma->capacidade / 2;
    turma->alunos = realloc(turma->alunos, nova_capacidade * sizeof(char*));
    turma->capacidade = nova_capacidade;
  }
}

void liberar_memoria(Turma turma) {
  for (int i = 0; i < turma->quantidade_de_alunos; i++) {
    free(turma->alunos[i]);
  }
  free(turma->alunos);
  free(turma);
}

int main() {
  Turma turma = criar_turma();
  
  printf("Criando turma...\n");
  turma_matricula(turma, "Anthony");
  turma_matricula(turma, "Ulisses");
  turma_matricula(turma, "Pedro");

  turma_lista(turma);
  printf("\nPedro jubilou...\n");
  turma_jubila(turma, "Pedro");

  turma_lista(turma);
  liberar_memoria(turma);

  return 0;
}