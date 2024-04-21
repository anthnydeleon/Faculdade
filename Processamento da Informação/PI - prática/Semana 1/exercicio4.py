# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia dois números inteiros e apresente o maior dos dois valores.
# Nesta questão está proibido usar if (isto é, não deve se usar nenhuma estrutura condicional) ou a função max.

def maior_numero(num1, num2):
    retorno = ((num1+num2)/2) + (abs(num2-num1)/2)
    return retorno


valor_1 = int(input("Primeiro número: "))
valor_2 = int(input("Segundo número: "))

print("O maior inteiro: %.0f" % maior_numero(valor_1, valor_2))
