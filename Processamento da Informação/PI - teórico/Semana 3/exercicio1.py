# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que permita receber cinco nomes de pessoas e identificar se todas tem o mesmo nome. O nome de uma pessoa pode ter espaços em branco

def nomes_iguais(nomes):
    if nomes[0] == nomes[1] == nomes[2] == nomes[3] == nomes[4]:
        print("Verdadeiro")
    else:
        print("Falso")


nomes = []
for i in range(5):
    nomes.append(input(f"Digite o {i+1}° nome: "))

nomes_iguais(nomes)
