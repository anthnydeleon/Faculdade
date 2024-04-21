# Anthony Deleon de Almeida Costa
# Exercicio: EXERCICIO


def saida():
    valores_entrada = str(input())
    if valores_entrada[:2] == "RA":
        if len(valores_entrada) == 20:
            final = valores_entrada[2:]
            print(int(final))
        else:
            print("RA INVALIDO")
    else:
        print("RA INVALIDO")


saida()
