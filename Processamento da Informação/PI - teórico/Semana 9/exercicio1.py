# Anthony Deleon de Almeida Costa
# Exercicio: A saída do seu programa deve consistir de um único número inteiro que representa a soma dos menores valores de cada coluna da matriz.

valores_entrada = []


def saida():
    soma = 0
    for i in range(len(valores_entrada[0])):
        menor = valores_entrada[0][i]
        for j in range(len(valores_entrada)):
            if valores_entrada[j][i] <= menor:
                menor = valores_entrada[j][i]
        soma += menor
    print(soma)


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(int(elemento))
        valores_entrada.append(linha)


entrada()
saida()
