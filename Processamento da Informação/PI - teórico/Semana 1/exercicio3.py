# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia dois números inteiros e apresente a divisão de ambos os números.

def divisao1(num1, num2):
    retorno = num1//num2
    return retorno


def divisao2(num1, num2):
    retorno = num1/num2
    return retorno


n1 = int(input("Primeiro valor: "))
n2 = int(input("Segundo valor: "))

a = divisao1(n1, n2)
b = divisao2(n1, n2)
print(a)
print("%.2f" % b)
