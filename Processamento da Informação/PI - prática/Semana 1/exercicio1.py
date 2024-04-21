# Anthony Deleon de Almeida Costa
# Exercício: A fórmula para calcular a área de uma circunferência é: área = pi*raio².
# Considerando para este problema que pi é igual a 3.14159. Efetue o cálculo da área,
# elevando o valor de raio ao quadrado e multiplicando por pi.

def area_circulo(num1):
    retorno = (num1**2)*3.14159
    return retorno


entrada = float(input("Insira o valor: "))
print(f"A área do circulo com raio {entrada} é %.4f" % area_circulo(entrada))
