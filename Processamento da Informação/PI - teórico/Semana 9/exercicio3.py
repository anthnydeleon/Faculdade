# Anthony Deleon de Almeida Costa
# Exercicio: A saída é uma única linha com um inteiro, indicando o valor mínimo encontrado nos elementos da matriz na forma Z.


valores_entrada = []


def saida():
    R = []
    for linha in valores_entrada:
        R.append(linha[::-1])
    menor = valores_entrada[0][0]
    for i in range(len(valores_entrada)):
        for j in range(len(valores_entrada)):
            if i == 0 or i == len(valores_entrada)-1:
                if valores_entrada[i][j] < menor:
                    menor = valores_entrada[i][j]
            elif i == j:
                if R[i][j] < menor:
                    menor = R[i][j]
    print(menor)


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(int(elemento))
        valores_entrada.append(linha)


entrada()
saida()
