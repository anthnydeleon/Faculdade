# Anthony Deleon de Almeida Costa
# Exercicio: O seu programa deve imprimir PALINDROMO, se a palavra recebida for um palíndromo, e NAO EH PALINDROMO, caso contrário.

valores_entrada = []


def saida():
    if valores_entrada[0] == (valores_entrada[0][::-1]):
        print("PALINDROMO")
    else:
        print("NAO EH PALINDROMO")


def entrada():
    for i in range(1):
        valores_entrada.append(str(input()).upper())


entrada()
saida()
