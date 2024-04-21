# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deverá imprimir a maior soma possível de uma fatia contínua do vetor.


valores_entrada = []


def saida():
    maior_soma = 0
    for i in range(len(valores_entrada)):
        for j in range(len(valores_entrada)):
            soma_atual = sum(valores_entrada[i:j+1])
            if maior_soma < soma_atual:
                maior_soma = soma_atual
    print(maior_soma)


def entrada():
    n = int(input())
    for i in range(n):
        valores_entrada.append(int(input()))


entrada()
saida()
