# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa para converter uma fração imprópria (representada por 2 números) em fração mista.

def fracao_impro_to_mista(numerador: int, denominador: int):
    if denominador != 0:
        quociente = numerador//denominador
        resto = numerador % denominador
        print("{}({}/{})".format(quociente, resto, denominador))
    else:
        print("ERRO")


numerador = int(input("Digite o númerador: "))
denominador = int(input("Digite o denominador: "))

fracao_impro_to_mista(numerador, denominador)
