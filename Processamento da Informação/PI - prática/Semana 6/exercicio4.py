# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que calcula o tamanho da maior palavra de um texto.

valores_entrada = []


def saida():
    tam_palavras = list()
    for palavra in valores_entrada:
        tam_palavras.append(len(palavra))
    maior = max(tam_palavras)
    print("Tam. Maior Palavra:", maior)


def entrada():
    j = int(input())
    for i in range(j):
        frase = input().split()
        tam_palavras = list()
        for palavra in frase:
            tam_palavras.append(len(palavra))

        maior = max(tam_palavras)
        for a, b in zip(frase, tam_palavras):
            if b == maior:
                valores_entrada.append(a)


entrada()
saida()
