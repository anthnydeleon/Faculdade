# Anthony Deleon de Almeida Costa
# Exercicio: Se o vetor original é dado por v = [4, 5, 8, 9, 3, 7, 6, 1] o vetor resultante da dobra ao meio
# será v’ = [5, 11, 15, 12] (em que, 5=4+1, 11=5+6, 15=8+7 e 12=9+3).

valores_entrada = []
novo_vetor = []


def saida():
    m = len(valores_entrada)
    n = int(m/2)
    for i in range(n):
        novo_vetor.append(valores_entrada[i]+valores_entrada[m-1-i])

    print(novo_vetor)


def entrada():
    n = int(input(""))
    while n % 2 != 0:
        n = int(input(""))

    for i in range(n):
        valores_entrada.append(int(input()))


entrada()
saida()
