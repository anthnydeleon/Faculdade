# Anthony Deleon de Almeida Costa
# Exercicio: O seu programa deve imprimir o campo trocando o caractere ‘.’ pelo número de minas em ladrilhos adjacentes.

valor = []


def saida():
    for i in range(len(valor)):
        for j in range(len(valor[0])):
            count = 0
            if valor[i][j] == '.':
                valor[i][j] = '0'

                if i-1 >= 0 and j-1 >= 0 and valor[i-1][j-1] == '*':
                    count += 1
                if i-1 >= 0 and valor[i-1][j] == '*':
                    count += 1
                if i-1 >= 0 and j+1 < len(valor[0]) and valor[i-1][j+1] == '*':
                    count += 1
                if j-1 >= 0 and valor[i][j-1] == '*':
                    count += 1
                if j+1 < len(valor[0]) and valor[i][j+1] == '*':
                    count += 1
                if i+1 < len(valor) and j-1 >= 0 and valor[i+1][j-1] == '*':
                    count += 1
                if i+1 < len(valor) and valor[i+1][j] == '*':
                    count += 1
                if i+1 < len(valor) and j+1 < len(valor[0]) and valor[i+1][j+1] == '*':
                    count += 1

                if count != 0:
                    valor[i][j] = str(count)

    for i in range(len(valor)):
        b = ''
        for j in range(len(valor[i])):
            b += str(valor[i][j])
        print(b)


def entrada():
    n = input().split()
    for i in range(int(n[0])):
        linha = list()
        for elemento in input():
            linha.append(str(elemento))
        valor.append(linha)


entrada()
saida()
