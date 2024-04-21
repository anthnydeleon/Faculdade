# Anthony Deleon de Almeida Costa
# Exercicio: O seu programa deve imprimir EH GRACIOSA se a rotulação dada for graciosa e NAO EH GRACIOSA caso contrário..

valores_entrada = []


def saida():
    vetor = []
    final = []
    for i in range(len(valores_entrada)):
        vetor += valores_entrada[i].split()
    for i in range(len(vetor)):
        if i % 2 == 0:
            final.append(abs(int(vetor[i])-int(vetor[i+1])))
    if len(final) == len(set(final)):
        print("EH GRACIOSA")
    else:
        print("NAO EH GRACIOSA")


def entrada():
    n = int(input())
    for i in range(n-1):
        valores_entrada.append(str(input()))


entrada()
saida()
