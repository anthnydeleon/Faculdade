# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita imprimir uma representação de um tabuleiro quadrado de xadrez.

valores_entrada = []


def saida():
    for i in range(valores_entrada[0]):
        for i2 in range(valores_entrada[0]):
            if i % 2 == 0:
                if i2 % 2 == 0:
                    print("o", end="")
                else:
                    print("*", end="")
            else:
                if i2 % 2 == 0:
                    print("*", end="")
                else:
                    print("o", end="")
        print()


def entrada():
    for i in range(1):
        valores_entrada.append(int(input("")))


entrada()
saida()
