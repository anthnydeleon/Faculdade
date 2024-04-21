# Anthony Deleon de Almeida Costa
# Exercicio: A saída do seu programa deve consistir de uma única linha contendo a palavra “X = ”, seguida do valor da soma dos elementos no X da matriz.

valores_entrada = []


def saida():
    R = []
    for linha in valores_entrada:
        R.append(linha[::-1])

    soma = -valores_entrada[len(valores_entrada)//2][len(valores_entrada)//2]
    for i in range(len(valores_entrada)):
        for j in range(len(valores_entrada)):
            if i == j:
                soma += valores_entrada[i][j]
                soma += R[i][j]
    print("X = ", soma)


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(int(elemento))
        valores_entrada.append(linha)


entrada()
saida()
