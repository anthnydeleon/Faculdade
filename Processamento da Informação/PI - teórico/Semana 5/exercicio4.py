# Anthony Deleon de Almeida Costa
# Exercicio: Neste exercício, você deverá escrever um programa que receberá dois números, o primeiro para iniciar
# a equação 2x e o segundo para iniciar a equação x2. Diferentemente do gráfico, seu programa deverá re-aplicar as
# equações n vezes e determinar qual das equações ficou com o maior valor.

valores_entrada = []


def saida():
    for i in range(valores_entrada[2]):
        valores_entrada[0] *= 2
        valores_entrada[1] **= 2
    if valores_entrada[0] > valores_entrada[1]:
        print("0")
    elif valores_entrada[0] < valores_entrada[1]:
        print("1")
    else:
        print("0")


def entrada():
    for i in range(2):
        valores_entrada.append(float(input()))
    valores_entrada.append(int(input()))


entrada()
saida()
