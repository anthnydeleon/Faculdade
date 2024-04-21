# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que recebe como entrada a temperatura em graus Celsius e realize duas
# conversões: uma para Fahrenheit e outra para Kelvin.

def conversao_temperatura_fahrenheit(celsius):
    retorno = int((celsius*1.8)+32)
    return retorno


def conversao_temperatura_kelvin(celsius):
    retorno = int((celsius+273.15))
    return retorno


celsius = float(
    input("Digite o valor da temperatura em Celsius para converter: "))
print(conversao_temperatura_fahrenheit(celsius), "F")
print(conversao_temperatura_kelvin(celsius), "K")
