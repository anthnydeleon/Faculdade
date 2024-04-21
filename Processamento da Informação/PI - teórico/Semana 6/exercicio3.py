# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita verificar se uma frase é ou não um pantograma.
# Considere que o usuário digitará apenas uma frase.


def saida():
    alfabeto = "abcdefghijklmnopqrstuvwxyz"
    for i in alfabeto:
        if i not in valores_entrada.lower():
            return False

    return True


valores_entrada = str(input()).lower()
if(saida() == True):
    print("SIM")
else:
    print("NAO")
