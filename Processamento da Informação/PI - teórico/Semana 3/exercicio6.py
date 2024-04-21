# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que permita receber apenas um número inteiro (de no máximo 3 dígitos) e imprima a sua formação em centenas, dezenas e milhares.


def formatacao_numeros(numero):
    centena = numero // 100
    dezena = numero // 10 % 10
    unidade = numero % 10
    if centena == 0 and dezena != 0 and unidade != 0:
        print(f"{dezena} dezenas e {unidade} unidades")
    elif centena != 0 and dezena == 0 and unidade != 0:
        print(f"{centena} centenas e {unidade} unidades")
    elif centena != 0 and dezena != 0 and unidade == 0:
        print(f"{centena} centenas e {dezena} dezenas")
    elif centena == 0 and dezena == 0 and unidade != 0:
        print(f"{unidade} unidades")
    elif centena == 0 and dezena != 0 and unidade == 0:
        print(f"{dezena} dezenas")
    elif centena != 0 and dezena == 0 and unidade == 0:
        print(f"{centena} centenas")
    else:
        print(f"{centena} centenas e {dezena} dezenas e {unidade} unidades")


numero = int(input("Digite o número: "))
formatacao_numeros(numero)
