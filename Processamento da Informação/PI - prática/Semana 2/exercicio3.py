# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que leia os comprimentos (números inteiros positivos) dos lados de um triângulo.
# Após isso, verifique se é um triângulo válido. Observação: para um triângulo ser válido, nenhum dos lados
# pode ser maior que a soma dos outros dois lados. Adicionalmente, verifique se é um triângulo retângulo.


def triangulo_retangulo(a, b, c):
    if a > b+c or b > a+c or c > a+b:
        triangulo = "Não é um triângulo"
        print(triangulo)
    else:
        triangulo = "É um triângulo"
        print(triangulo)
    if triangulo == "É um triângulo" and (a == (b**2 + c**2)**0.5 or b == (a**2 + c**2)**0.5 or c == (a**2 + b**2)**0.5):
        print("É um triângulo retângulo")


a = int(input("Valor do lado A: "))
b = int(input("Valor do lado B: "))
c = int(input("Valor do lado C: "))
triangulo_retangulo(a, b, c)
