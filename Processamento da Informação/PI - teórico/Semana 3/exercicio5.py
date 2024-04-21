# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que recebe marcadores para a presença das cores vermelha, verde e azul e responde com a cor resultante.

def cores(cores):
    if cores[0] == 1 and cores[1] == 0 and cores[2] == 0:
        print("3")
    elif cores[0] == 0 and cores[1] == 1 and cores[2] == 0:
        print("5")
    elif cores[0] == 0 and cores[1] == 0 and cores[2] == 1:
        print("1")
    elif cores[0] == 1 and cores[1] == 1 and cores[2] == 0:
        print("4")
    elif cores[0] == 1 and cores[1] == 0 and cores[2] == 1:
        print("2")
    elif cores[0] == 0 and cores[1] == 1 and cores[2] == 1:
        print("6")
    elif cores[0] == 1 and cores[1] == 1 and cores[2] == 1:
        print("7")
    elif cores[0] == 0 and cores[1] == 0 and cores[2] == 0:
        print("8")


cor = []
for i in range(3):
    cor.append(int(input("")))

cores(cor)
