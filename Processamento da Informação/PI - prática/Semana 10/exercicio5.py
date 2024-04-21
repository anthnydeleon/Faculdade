# Anthony Deleon de Almeida Costa
# Exercicio: A saída do seu programa deve ser “SIM” caso as duas palavras estiverem no tabuleiro. Caso contrário deve ser “NAO”.

valor = []


n1 = input()
n2 = input()


def saida():
    concatenar = ''
    for i in range(len(valor)):
        for j in range(len(valor[i])):
            concatenar += str(valor[i][j])

    cond1 = False
    cond2 = False

    if n1 in concatenar or n1[::-1] in concatenar:
        cond1 = True
    if n2 in concatenar or n2[::-1] in concatenar:
        cond2 = True

    if cond1 == True and cond2 == True:
        print("SIM")
    else:
        print("NAO")


def entrada():
    for i in range(16):
        linha = list()
        for elemento in input().strip().split():
            linha.append(str(elemento))
        valor.append(linha)


entrada()
saida()
