# Anthony Deleon de Almeida Costa
# Exercicio: Um circuito lógico é construído a partir de portas lógicas. Uma porta lógica recebe entradas e produz uma saída.


def circuito_logico(A, B):
    if (not A == B and B != 0) or (not B == A and A != 0):
        print("1")
    else:
        print("0")


A = int(input("Digite 1 ou 0 para definir o valor de A: "))
B = int(input("Digite 1 ou 0 para definir o valor de B: "))

circuito_logico(A, B)
