# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que permita ler uma string de entrada e conte o número de vogais, consoantes, espaços e
# pontuações (caracteres “.”,“,”,“!”,“?”). A saída do programa deve ser a porcentagem de cada tipo de caractere na string.
# Você pode assumir que somente esses tipos de caracteres (sem acentos) estarão presentes na string.


def saida():
    vogal = "aeiou"
    consoante = "bcdfghjklmnpqrstvwxyz"
    espaco = " "
    pontuacao = ".,!?"
    totalvogal = 0
    totalconsoante = 0
    totalespaco = 0
    totalpontuacao = 0

    for i in valores_entrada.lower():
        if i in vogal:
            totalvogal += 1
        elif i in consoante:
            totalconsoante += 1
        elif i in espaco:
            totalespaco += 1
        elif i in pontuacao:
            totalpontuacao += 1
    print(f"Vogais: {((totalvogal*100)/len(valores_entrada)):,.2f}%")
    print(f"Consoantes: {((totalconsoante*100)/len(valores_entrada)):,.2f}%")
    print(f"Espacos: {((totalespaco*100)/len(valores_entrada)):,.2f}%")
    print(f"Pontuacoes: {((totalpontuacao*100)/len(valores_entrada)):,.2f}%")


valores_entrada = str(input())


saida()
