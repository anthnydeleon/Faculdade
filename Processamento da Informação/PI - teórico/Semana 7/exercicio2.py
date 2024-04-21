# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve imprimir as posições de todos os vizinhos repetidos em um vetor (veja os exemplos de entrada e saída).

valores_entrada = []


def saida():
    for i in range(len(valores_entrada)-1):
        if valores_entrada[i] == valores_entrada[i+1]:
            print(f"Pos {i} e {i+1}")


def entrada():
    n = int(input())
    for i in range(n):
        valores_entrada.append(int(input()))


entrada()
saida()
