# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que permita identificar se existe algum pedido de ajuda em uma determinada mensagem MORSE.
# Neste exercício, cada mensagem usada na transmissão, inicia e termina com um número inteiro de apenas um digito.
# Cada letra é separada por um espaço em branco. Cada palavra é separada por uma barra.


def codigo_morse(codigo):
    if "/ ... --- ... /" in codigo:
        print("Mensagem com pedido de ajuda!")
    else:
        print("Mensagem sem pedido de ajuda!")


mensagem = str(input("Mensagem em código morse: "))
codigo_morse(mensagem)