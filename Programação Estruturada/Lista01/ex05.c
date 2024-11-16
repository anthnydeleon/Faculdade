#include <stdio.h>
// No Brasil, medimos a temperatura em Celsius. 
// Já em países de lingua inglesa é muito comum que a medida de temperatura seja feita em Fahrenheit. 
// A seguinte forma nos permite estabelecer uma relação entre as duas unidades de medida

double convertTemperature(char scale, double temperature) {
  if (scale == 'c') return 32 + (temperature/5)*9;
  return ( ( temperature-32 ) / 9 ) * 5;
}

int main() {
  float temperature, temperatureConvert;
  char scale;

  printf("Digite a escala (c para Celsius, f para Fahrenheit): " );
  scanf("%c", &scale);

  printf("Digite a temperatura: " );
  scanf("%f", &temperature);

  temperatureConvert = convertTemperature(scale, temperature);
  printf("Temperatura convertida: %.1f", temperatureConvert);

  return 0;
}