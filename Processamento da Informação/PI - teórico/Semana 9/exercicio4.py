# Anthony Deleon de Almeida Costa
# Exercicio: Imprima, em uma só linha, a quantidade de elementos de cada coluna que são menores do que a média da coluna.
# As quantidades devem estar na sequência das colunas e separadas por espaço.


valores_entrada = []


def saida():
    for i in range(len(valores_entrada[0])):
        soma = 0
        count = 0
        qnt = 0
        for j in range(len(valores_entrada)):
            soma += valores_entrada[j][i]
            count += 1
        media = soma/count
        for j in range(len(valores_entrada)):
            if valores_entrada[j][i] < media:
                qnt += 1
        print(qnt)


def entrada():
    n = input().split()
    for i in range(int(n[0])):
        linha = list()
        for elemento in input().strip().split():
            linha.append(float(elemento))
        valores_entrada.append(linha)


entrada()
saida()
