# Anthony Deleon de Almeida Costa
# Exercicio: Leia uma sequência de inteiros e imprima os inteiros pares em ordem decrescente.

valores_entrada = []
resultado = []


def saida():
    for i in range(len(valores_entrada)):
        if (valores_entrada[i] % 2) == 0:
            resultado.append(valores_entrada[i])
    print(resultado)


def entrada():
    n = int(input())
    for i in range(n):
        valores_entrada.append(int(input()))
        valores_entrada.sort(reverse=True)


entrada()
saida()
