# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que leia dois números reais (x e y) que devem representar as coordenadas de
# um ponto em um plano. A seguir, determine qual o quadrante ao qual pertence o ponto, ou se está sobre um dos eixos
# cartesianos ou na origem (x = y = 0).

def coordenadas(x, y):
    if x > 0 and y > 0:
        print("Q1")
    elif x > 0 and y < 0:
        print("Q4")
    elif x < 0 and y > 0:
        print("Q2")
    elif x < 0 and y < 0:
        print("Q3")
    elif x == 0 and y == 0:
        print("Origem")
    elif (x == 0 and y > 0) or (x == 0 and y < 0):
        print("Eixo Y")
    elif (y == 0 and x > 0) or (y == 0 and x < 0):
        print("Eixo X")


x = float(input("Digite a coordenada do eixo x: "))
y = float(input("Digite a coordenada do eixo y: "))
coordenadas(x, y)
