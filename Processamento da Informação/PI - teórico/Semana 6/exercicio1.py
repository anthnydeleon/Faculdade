# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita repetir uma palavra um número determinado de vezes.
# Considere que o usuário digitará apenas palavras sem acento.

valores_entrada = []


def saida():
    if valores_entrada[1] != 0:
        print(f"{valores_entrada[0]} , " *
              (valores_entrada[1]-1) + f"{valores_entrada[0]}")


def entrada():
    for i in range(1):
        valores_entrada.append(str(input()))
        valores_entrada.append(int(input()))


entrada()
saida()
