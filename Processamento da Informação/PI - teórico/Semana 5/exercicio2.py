# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita verificar se um número pertence à sequência de Fibonacci.

valores_entrada = []


def saida():
    a = 0
    b = 1
    condicao = "Falso"
    while a <= valores_entrada[0]:
        if a == valores_entrada[0]:
            condicao = "Verdadeiro"
        soma = a + b
        a = b
        b = soma
    return print(condicao)


def entrada():
    for i in range(1):
        valores_entrada.append(int(input("Número: ")))


entrada()
saida()
