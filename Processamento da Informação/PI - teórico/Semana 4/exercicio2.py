# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que apresente a tabuada de um número.

valores_entrada = []


def saida():
    for i in range(valores_entrada[1]+1):
        print(
            f"{valores_entrada[0]} x {i} = {valores_entrada[0]*i}")


def entrada():
    for i in range(2):
        valores_entrada.append(int(input("Entradas: ")))


entrada()
saida()
