# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve imprimir todas as possíveis posições do cavalo. A ordem de impressão deve respeitar
# o percurso do tabuleiro de esquerda valores_entrada direita e de cima para baixo.

#  0 1 2 3 4 5 6 7
# [0 0 0 0 0 0 0 0] 0
# [0 0 0 0 0 0 0 0] 1
# [0 0 0 0 0 0 0 0] 2
# [0 0 0 0 0 0 0 0] 3
# [0 0 0 0 0 0 0 0] 4
# [0 0 0 0 0 0 0 0] 5
# [0 0 0 0 0 0 0 0] 6
# [0 0 0 0 0 0 0 0] 7

valores_entrada = []


def saida():
    if (valores_entrada[0]-2 >= 0 and valores_entrada[1]-1 >= 0):
        if (valores_entrada[0]-2 <= 7 and valores_entrada[1]-1 <= 7):
            print(f"{valores_entrada[0]-2} {valores_entrada[1]-1}")
    if (valores_entrada[0]-2 >= 0 and valores_entrada[1]+1 >= 0):
        if (valores_entrada[0]-2 <= 7 and valores_entrada[1]+1 <= 7):
            print(f"{valores_entrada[0]-2} {valores_entrada[1]+1}")
    if (valores_entrada[0]-1 >= 0 and valores_entrada[1]-2 >= 0):
        if (valores_entrada[0]-1 <= 7 and valores_entrada[1]-2 <= 7):
            print(f"{valores_entrada[0]-1} {valores_entrada[1]-2}")
    if (valores_entrada[0]-1 >= 0 and valores_entrada[1]+2 >= 0):
        if (valores_entrada[0]-1 <= 7 and valores_entrada[1]+2 <= 7):
            print(f"{valores_entrada[0]-1} {valores_entrada[1]+2}")
    if (valores_entrada[0]+1 >= 0 and valores_entrada[1]-2 >= 0):
        if (valores_entrada[0]+1 <= 7 and valores_entrada[1]-2 <= 7):
            print(f"{valores_entrada[0]+1} {valores_entrada[1]-2}")
    if (valores_entrada[0]+1 >= 0 and valores_entrada[1]+2 >= 0):
        if (valores_entrada[0]+1 <= 7 and valores_entrada[1]+2 <= 7):
            print(f"{valores_entrada[0]+1} {valores_entrada[1]+2}")
    if (valores_entrada[0]+2 >= 0 and valores_entrada[1]-1 >= 0):
        if (valores_entrada[0]+2 <= 7 and valores_entrada[1]-1 <= 7):
            print(f"{valores_entrada[0]+2} {valores_entrada[1]-1}")
    if (valores_entrada[0]+2 >= 0 and valores_entrada[1]+1 >= 0):
        if (valores_entrada[0]+2 <= 7 and valores_entrada[1]+1 <= 7):
            print(f"{valores_entrada[0]+2} {valores_entrada[1]+1}")


def entrada():
    for i in range(1):
        for elemento in input().strip().split():
            valores_entrada.append(int(elemento))


entrada()
saida()
