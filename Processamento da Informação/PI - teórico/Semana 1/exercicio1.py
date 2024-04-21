# Anthony Deleon de Almeida Costa
# Exercício: Leia dois números inteiros e calcule o produto entre estes dois valores.

def produto_dois_numeros(num1, num2):
    retorno = num1 * num2
    print("produto = ", retorno)


n1 = int(input("primeiro número: "))
n2 = int(input("segundo número: "))
resultado = produto_dois_numeros(n1, n2)
