# Anthony Deleon de Almeida Costa
# Exercicio: A primeira linha contém um número inteiro N (1 < N ≤ 100) representando a quantidade de estímulos em um
# teste de P300. Cada uma das próximas N linhas consiste de uma string que pode ser apenas uma dentre duas palavras: "ALVO"
# ou "padrao". A saída é o número M de estímulos alvo contido na sequência de N estímulos.

valores_entrada = []


def saida():
    total = 0
    for i in range(len(valores_entrada)):
        if valores_entrada[i] == "ALVO":
            total += 1
    print(total)


def entrada():
    N = int(input())
    for i in range(N):
        valores_entrada.append(str(input()))


entrada()
saida()
