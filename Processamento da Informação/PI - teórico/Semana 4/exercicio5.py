# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita imprimir uma representação de um tabuleiro quadrado de xadrez.

valores_entrada = []
divisores = []


def saida():
    for i in range(1, valores_entrada[0]+1):
        if valores_entrada[0] % i == 0 and i % 2 == 0:
            divisores.append(i)
    print(len(divisores))


def entrada():
    for i in range(1):
        valores_entrada.append(int(input("Valor: ")))


entrada()
saida()
