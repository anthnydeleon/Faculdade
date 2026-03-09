#include <stdio.h>

void readNumber(double *number) {
  scanf("%lf", number);
}

double calculateIMC(double weight, double height) {
  return weight / (height * height);
}

void printIMCClassification(double imc) {
    if(imc < 16) {
    printf("Magreza grave");
  } else if(imc >= 16 && imc < 17) {
    printf("Magreza moderada");
  } else if(imc >= 17 && imc < 18.5) {
    printf("Magreza leve");
  } else if(imc >= 18.5 && imc < 25) {
    printf("Saudavel");
  } else if(imc >= 25 && imc < 30) {
    printf("Sobrepeso");
  } else if(imc >= 30 && imc < 35) {
    printf("Obesidade Grau I");
  } else if(imc >= 35 && imc < 40) {
    printf("Obesidade Grau II (severa)");
  } else {
    printf("Obesidade Grau III (morbida)");
  }
}

int main() {
  double weight, height, imc;
  readNumber(&height);
  readNumber(&weight);

  imc = calculateIMC(weight, height);
  printIMCClassification(imc);

  return 0;
}