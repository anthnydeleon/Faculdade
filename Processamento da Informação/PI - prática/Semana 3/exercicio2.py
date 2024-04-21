# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia um inteiro que representa um código de DDD para discagem interurbana.
# Em seguida, informe à qual cidade o DDD pertence.

def DDD(ddd):
    if ddd == 61:
        print("Brasilia")
    elif ddd == 71:
        print("Salvador")
    elif ddd == 11:
        print("Sao Paulo")
    elif ddd == 21:
        print("Rio de Janeiro")
    elif ddd == 32:
        print("Juiz de Fora")
    elif ddd == 19:
        print("Campinas")
    elif ddd == 27:
        print("Vitoria")
    elif ddd == 31:
        print("Belo Horizonte")
    else:
        print("DDD nao cadastrado")


ddd = int(input("Digite o DDD para saber o local: "))
DDD(ddd)
