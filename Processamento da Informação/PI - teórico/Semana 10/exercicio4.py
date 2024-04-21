# Anthony Deleon de Almeida Costa
# Exercicio: Responda “yes” caso todos os números tenham sido visitados; “no” caso contrário.


valores_entrada = []
visita = []


def saida():
    for i in range(len(valores_entrada)):
        if visita[i] in valores_entrada:
            valores_entrada.remove(visita[i])

    if len(valores_entrada) == 0:
        print("yes")
    else:
        print("no")


def entrada():
    n = input().split()

    for i in range(int(n[0])):
        for elemento in input().strip().split():
            valores_entrada.append(int(elemento))

    for i in range(len(valores_entrada)):
        visita.append(int(input()))


entrada()
saida()
