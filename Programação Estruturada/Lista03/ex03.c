#include <stdio.h>
#define MAX 9

// Questão 3. A matriz do jogo Sudoku é uma matriz de inteiros 9 x 9. Para ser uma solução do
// problema, cada linha e coluna deve conter todos os números de 1 a 9. Além disso, se dividirmos
// a matriz em 9 regiões 3 x 3, cada uma destas regiões também deve conter os números de 1 a 9. O
// exemplo abaixo mostra uma matriz que é uma solução do problema.
// Escreva um programa que dada uma matriz 9 x 9 preenchida, verifique se ela é uma solução válida do
// Sudoku

void le_sudoku(int sudoku[][MAX]) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      scanf("%d", &sudoku[i][j]);
    }
  }
}

int verifySudokuSubGroup(int sudoku[][MAX]) {
  for (int row = 0; row < 9; row += 3) {
    for (int col = 0; col < 9; col += 3) {
      int array[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          int num = sudoku[row + i][col + j];
          if (array[num] == 1) return -1;
          array[num] = 1;
        }
      }
    }
  }
  return 1;
}

int verifySudoku(int sudoku[][MAX]) {
  for (int i = 0; i < MAX; i++) {
    for (int j = 0; j < MAX; j++) {
      for (int k = 0; k < MAX; k++) {
        if (j != k && sudoku[i][j] == sudoku[i][k]) return -1; // linha
        if (i != k && sudoku[i][j] == sudoku[k][j]) return -1; // coluna
      }
    }
  }
  return verifySudokuSubGroup(sudoku);
}

int main() {
  int sudoku[MAX][MAX], isValid;
  printf("Digite os numeros do sudoku %dx%d: ", MAX, MAX);
  le_sudoku(sudoku);

  isValid = verifySudoku(sudoku);
  isValid == 1 ? printf("Solucao valida!") : printf("Solucao invalida!");
  return 0;
}