# Anthony Deleon de Almeida Costa
# Exercicio: A saída do seu programa deve consistir do segundo maior valor da matriz de entrada. Caso a matriz não
# contenha um segundo maior valor, o seu programa deve imprimir “NOT FOUND”.


valores_entrada = []
maior = []


def saida():
    for i in range(len(valores_entrada)):
        maior.append(max(valores_entrada[i]))
        if len(sorted(set(valores_entrada[i]))) > 1:
            maior.append(sorted(set(valores_entrada[i]), reverse=True)[1])
        else:
            maior.append(sorted(set(valores_entrada[i]), reverse=True)[0])
    if len(sorted(set(maior))) > 1:
        print(sorted(set(maior), reverse=True)[1])
    else:
        maior.append(sorted(set(maior), reverse=True)[0])
        print("NOT FOUND")


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(int(elemento))
        valores_entrada.append(linha)


entrada()
saida()
