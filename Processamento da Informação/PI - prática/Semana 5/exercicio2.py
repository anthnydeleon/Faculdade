# Anthony Deleon de Almeida Costa
# Exercicio: O programa deve mostrar todos os números naturais a partir de 1. Contudo, ao chegar em um múltiplo de 4,
# o programa deve imprimir a palavra “PIM” e pular uma linha.

valores_entrada = []


def saida():
    for i in range(valores_entrada[0]):
        a = 1 + i*4
        b = 2 + i*4
        c = 3 + i*4
        d = "PIM"
        print(a, b, c, d)


def entrada():
    for i in range(1):
        valores_entrada.append(int(input()))


entrada()
saida()
