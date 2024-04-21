# Anthony Deleon de Almeida Costa
# Exercicio: A saída é uma única linha com 2 inteiros X e Y separados por um espaço em branco.
# Eles representam a coordenada (X, Y) do sabre de luz, caso encontrado. Se o terreno não tem um padrão de sabre de luz, X e Y
# são ambos zero.

valores_entrada = []


def saida():
    a = False
    for i in range((len(valores_entrada))):
        for j in range((len(valores_entrada[0]))):
            try:
                if valores_entrada[i][j] == 42:
                    if 7 == valores_entrada[i-1][j-1] and 7 == valores_entrada[i-1][j] and 7 == valores_entrada[i-1][j+1] and 7 == valores_entrada[i][j-1] and 7 == valores_entrada[i][j+1] and 7 == valores_entrada[i+1][j-1] and 7 == valores_entrada[i+1][j] and 7 == valores_entrada[i+1][j+1]:
                        print(i+1, j+1)
                        a = True
                        break
            except IndexError:
                continue

    if a == False:
        print("0 0")


def entrada():
    n = input().split()
    for i in range(int(n[0])):
        linha = list()
        for elemento in input().strip().split():
            linha.append(int(elemento))
        valores_entrada.append(linha)


entrada()
saida()
