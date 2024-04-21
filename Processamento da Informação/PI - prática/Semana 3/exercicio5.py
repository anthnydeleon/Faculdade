# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia um número N e uma string C. Se a string C for igual “TROCA” ou “INSERE”, então
# o seu programa deve ler mais dois inteiros I e D, onde 0 <= D <= 9, nesta respectiva ordem.


def manipulacao_de_digitos(inteiro, condicao):
    if condicao == "TROCA":
        posicao = int(input("Digite a posição do número: "))
        numero = int(input("Digite pelo número que deseja trocar: "))
        numero_posicao_original = (inteiro//10**(posicao - 1)) % 10
        diferanca = numero_posicao_original - numero
        diferenca_na_base = diferanca * 10**(posicao-1) * -1
        return inteiro + diferenca_na_base
    elif condicao == "INSERE":
        posicao = int(input("Digite a posição que deseja inserir o número: "))
        numero = int(input("Digite pelo número que deseja colocar: "))
        resto = inteiro % 10**(posicao-1)
        passo_dois = (inteiro - resto)*10
        passo_tres = numero * 10**(posicao-1)
        return passo_tres + passo_dois + resto
    elif condicao == "APAGA":
        posicao = int(input("Digite a posição do número que deseja apagar: "))
        resto = inteiro % 10**(posicao-1)
        passo_dois = inteiro//10**(posicao) * 10**(posicao-1)
        return resto + passo_dois


inteiro = int(input("Digite o inteiro: "))
condicao = input("Digite o comando: ")

print(manipulacao_de_digitos(inteiro, condicao))
