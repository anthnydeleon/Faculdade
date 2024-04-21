# Anthony Deleon de Almeida Costa
# Exercicio: A saída deve dizer se a matriz fornecida é: nao triangular, triangular superior, triangular inferior ou diagonal.


valores_entrada = []


def saida():
    matriz_sup = True
    matriz_inf = True
    for i in range(len(valores_entrada)):
        for j in range(i+1, len(valores_entrada[0])):
            if valores_entrada[i][j] != 0:
                matriz_inf = False
    for i in range(1, len(valores_entrada)):
        for j in range(0, i):
            if valores_entrada[i][j] != 0:
                matriz_sup = False

    if (matriz_sup and matriz_inf) == False:
        print("diagonal")
    else:
        if matriz_sup == False:
            print("triangular superior")
        if matriz_inf == False:
            print("triangular inferior")


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(float(elemento))
        valores_entrada.append(linha)


entrada()
saida()
