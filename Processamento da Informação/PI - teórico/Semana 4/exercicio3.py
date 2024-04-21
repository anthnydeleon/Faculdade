# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita imprimir apenas as bordas de um retângulo.

valores_entrada = []


def saida():
    for i in range(valores_entrada[0]):
        if i == 0 or i == valores_entrada[0]-1:
            print("*"*valores_entrada[1])
        else:
            if valores_entrada[1] == 1:
                print("*"*1 + " "*(valores_entrada[1]-2))
            else:
                print("*"*1 + " "*(valores_entrada[1]-2) + "*"*1)


def entrada():
    for i in range(2):
        valores_entrada.append(int(input("Entradas: ")))


entrada()
saida()
