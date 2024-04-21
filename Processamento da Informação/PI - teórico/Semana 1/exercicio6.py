# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia três números inteiros e apresente o maior dos três valores.

def maior_numero(num1, num2, num3):
    retorno1 = ((num1+num2)/2) + (abs(num2-num1)/2)
    retorno = ((num3+retorno1)/2) + (abs(retorno1-num3)/2)
    return retorno


valor_1 = int(input("Primeiro número: "))
valor_2 = int(input("Segundo número: "))
valor_3 = int(input("Terceiro número: "))

print("O maior inteiro: %.0f" % maior_numero(valor_1, valor_2, valor_3))
