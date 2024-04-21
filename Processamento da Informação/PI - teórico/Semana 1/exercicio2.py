# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia um número real e apresente o triplo desse valor.

def triplo(num1):
    retorno = num1*3
    print('O triplo de %.2f' % num1, "é %.2f" % retorno)


n1 = float(input("Digite o número: "))
triplo(n1)
