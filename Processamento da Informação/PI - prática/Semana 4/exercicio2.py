# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que dado dois números naturais determine se eles são amigos.

def numeros_amigos(valor_um, valor_dois, soma1, soma2):
    for i in range(valor_um):
        if i != 0 and valor_um % (i) == 0:
            soma1.append(i)
    soma_um = sum(soma1)
    for i in range(valor_dois):
        if i != 0 and valor_dois % (i) == 0:
            soma2.append(i)
    soma_dois = sum(soma2)
    if valor_um == soma_dois and valor_dois == soma_um:
        print("amigos")
    else:
        print("nao amigos")


valor_um = int(input("Digite o primeiro valor: "))
valor_dois = int(input("Digite o segundo valor: "))
soma_um = []
soma_dois = []

numeros_amigos(valor_um, valor_dois, soma_um, soma_dois)
