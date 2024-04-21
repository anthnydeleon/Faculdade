# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que leia dois vetores com N inteiros cada. Em seguida, verifique quais elementos do
# segundo vetor são iguais a algum elemento do primeiro vetor. Se não houver elementos comuns, o programa deve
# informar que isso ocorreu.


def saida():
    a = False
    for i in range(len(vetor1)):
        if vetor1[i] in vetor2:
            print(vetor1[i])
            a = True
    if a == False:
        print("NENHUM ELEMENTO EM COMUM")


a = input()
b = input()
vetor1 = a.split()
vetor2 = b.split()


saida()
