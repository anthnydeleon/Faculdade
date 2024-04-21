# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia quatro números correspondentes aos eixos x e y de dois pontos quaisquer
# no plano, p1(x1,y1) e p2(x2,y2) e calcule a distância de Manhattan entre eles, mostrando quatro casas decimais,
# segundo a fórmula: |x1-x2|+|y1-y2|. Em que o valor absoluto está representado por |.|

def distancia_manhattan(x1, x2, y1, y2):
    retorno = abs(x1-x2) + abs(y1-y2)
    print("A distância de P1 e P2 é: %.4f" % retorno)


x1 = float(input("Digite o ponto x1 da coordenada P1: "))
y1 = float(input("Digite o ponto y1 da coordenada P1: "))
x2 = float(input("Digite o ponto x2 da coordenada P2: "))
y2 = float(input("Digite o ponto y2 da coordenada P2: "))

distancia_manhattan(x1, x2, y1, y2)
