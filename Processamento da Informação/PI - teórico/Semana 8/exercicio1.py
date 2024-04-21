# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve produzir uma única linha de saída contendo um caractere, “S” caso a risada seja das mais
# engraçadas, ou “N” caso contrário. Quebre uma linha após a impressão da letra.


valores_entrada = []
vogal = "aeiou"


def saida():
    novo = ""
    for i in valores_entrada[0]:
        if i in vogal:
            novo += i
    if novo == novo[::-1]:
        print("S")
    else:
        print("N")


def entrada():
    for i in range(1):
        valores_entrada.append(str(input()))


entrada()
saida()
