# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia um número qualquer e apresente uma mensagem dizendo em qual dos seguintes
# intervalos ([0,25], (25,50], (50,75], (75,100]) este valor se encontra. Se o valor não estiver em nenhum destes
# intervalos, deverá ser impressa a mensagem “Fora de intervalo”.

def intervalo(x):
    if x < 0:
        print("Fora de intervalo")
    elif x <= 25:
        print("Intervalo [0,25]")
    elif x < 50:
        print("Intervalo (25,50]")
    elif x < 75:
        print("Intervalo (50,75]")
    elif x <= 100:
        print("Intervalo (75,100]")
    elif x > 100:
        print("Fora de intervalo")


x = float(input("Digite um número: "))
intervalo(x)
