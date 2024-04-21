# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve imprimir na tela o resultado usando duas casas decimais. Veja nos exemplos
# abaixo que na saída há o texto “Resultado = ” precedendo o valor do resultado propriamente.

valores_entrada = []


def saida():
    soma = 0
    for i in range(0, len(valores_entrada)):
        for j in range(0, len(valores_entrada)):
            if i > j:
                soma += valores_entrada[i][j]
            if i < j:
                soma += valores_entrada[i][j]
    for i in range(len(valores_entrada)):
        for j in range(len(valores_entrada[0])):
            if i == j:
                soma -= valores_entrada[i][j]

    print(f"Resultado ={soma: .2f}")


def entrada():
    n = input().split()
    for i in range(int(n[0])):
        linha = list()
        for elemento in input().strip().split():
            linha.append(float(elemento))
        valores_entrada.append(linha)


entrada()
saida()
