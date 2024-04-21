# Anthony Deleon de Almeida Costa
# Exercicio: A entrada é um número inteiro n estritamente positivo ímpar, seguidos dos elementos do vetor V.

valores_entrada = []


def saida():
    rge = len(valores_entrada)-1
    for i in range(len(valores_entrada)):
        if valores_entrada[i] < valores_entrada[rge-i]:
            valores_entrada[i], valores_entrada[rge -
                                                i] = valores_entrada[rge-i], valores_entrada[i]
    print(valores_entrada)


def entrada():
    n = int(input())
    for i in range(n):
        valores_entrada.append(int(input()))


entrada()
saida()
