# Anthony Deleon de Almeida Costa
# Exercício: Leia quatro números inteiros A, B, C e D. A seguir, calcule e mostre a diferença
# do produto de A e B pelo produto de C e D segundo a fórmula: DIFERENCA = (A * B - C * D).

def diferenca(a, b, c, d):
    retorno = a*b - c*d
    return retorno


a = int(input("Valor A: "))
b = int(input("Valor B: "))
c = int(input("Valor C: "))
d = int(input("Valor D: "))

print("A diferença entre o produto A*B - C*D é: ", diferenca(a, b, c, d))
