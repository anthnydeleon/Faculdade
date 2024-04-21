# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita imprimir uma representação de um tabuleiro quadrado de xadrez.

valores_entrada = []


def saida():
    cronometro = 0
    while valores_entrada[0] >= 0:
        valores_entrada[0] = valores_entrada[0]-cronometro
        if valores_entrada[0] < 0:
            print("Acabou")
            break
        print(f"Faltam {valores_entrada[0]} segundos")
        cronometro = cronometro + 2


def entrada():
    for i in range(1):
        valores_entrada.append(int(input()))


entrada()
saida()
