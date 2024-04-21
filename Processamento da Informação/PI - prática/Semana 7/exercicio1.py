# Anthony Deleon de Almeida Costa
# Exercicio: Impressão do valores dos números reais repetidos (um número em cada linha).

valores_entrada = []


def saida():
    a = False
    for i in range(len(valores_entrada)):
        for j in range(i, len(valores_entrada)):
            if i != j:
                if (valores_entrada[i] == valores_entrada[j]):
                    print(f"{valores_entrada[i]}")
                    a = True
    if a == False:
        print("")


def entrada():
    n = int(input())
    for i in range(n):
        valores_entrada.append(float(input()))


entrada()
saida()
