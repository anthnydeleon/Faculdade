# Anthony Deleon de Almeida Costa
# Exercicio: O seu programa receber um número natural N que representa a dimensão (em linhas e colunas)
# dos lados do triângulo que você deverá desenhar.

valores_entrada = []


def saida():
    for i in range(valores_entrada[0]):
        print("*"*(i+1))


def entrada():
    for i in range(1):
        valores_entrada.append(int(input("Digite o tamanho da pirâmide: ")))


entrada()
saida()
