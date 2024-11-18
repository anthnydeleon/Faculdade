#include <stdio.h>
// Questão 8. 
// Crie um struct data com os campos dia, mes e ano (todos do tipo int); 
// crie um struct aluno com um campo nome, data_nascimento (do tipo struct data) e ra. 
// Então, escreva um programa que seja capaz de:
// • Cadastrar alunos
// • Buscar alunos por nome (liste todos os alunos que casem com o nome ou sobrenome fornecido)
// • Listar todos os alunos cuja data de nascimento esteja em um determinado intervalo

#define MAX 100

typedef struct {
  int dia, mes, ano;
} Data;

typedef struct {
  char nome[MAX];
  Data data_nascimento;
  int ra;
} Aluno;

Aluno cadastrar_alunos(Aluno alunos[], Aluno aluno) {
  
}

int main() {

  return 0;
}