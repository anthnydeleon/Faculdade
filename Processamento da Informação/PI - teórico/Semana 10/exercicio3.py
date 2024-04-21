# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve produzir uma única linha, contendo um único inteiro, o peso máximo entre todas as posições do tabuleiro.


valores_entrada = []


def saida():
    soma = 0
    maior = 0
    SumColunas = []
    SumLinhas = []

    # soma colunas
    for i in range(len(valores_entrada[0])):
        soma = 0
        for j in range(len(valores_entrada)):
            soma += valores_entrada[j][i]
        SumColunas.append(soma)

    # soma linha
    for i in range(len(valores_entrada)):
        soma = 0
        for j in range(len(valores_entrada[0])):
            soma += valores_entrada[i][j]
        SumLinhas.append(soma)

    for i in range(len(valores_entrada[0])):
        for j in range(len(valores_entrada)):
            soma = SumColunas[j] + SumLinhas[i] - 2*valores_entrada[i][j]
            if maior < soma:
                maior = soma
    print(maior)


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(int(elemento))
        valores_entrada.append(linha)


entrada()
saida()
