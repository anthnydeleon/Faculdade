# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que leia um número natural positivo N e determine quantos dígitos este número possui.

valores_entrada = []


def saida():
    valores_entrada[0] = abs(int(valores_entrada[0]))
    if valores_entrada[0] < 2:
        return print("1")
    contar = 0
    valor = 1
    while valor <= valores_entrada[0]:
        valor *= 10
        contar += 1
    return print(contar)


def entrada():
    for i in range(1):
        valores_entrada.append(int(input("Número: ")))


entrada()
saida()
