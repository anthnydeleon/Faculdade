# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que receba uma sequência de números e que liste todos os pares de posições da sequência nos quais um mesmo número aparece.


valores_entrada = []


def saida():
    a = False
    for i in range(len(valores_entrada)):
        for j in range(i, len(valores_entrada)):
            if i != j:
                if (valores_entrada[i] == valores_entrada[j]):
                    print(f"{valores_entrada[i]}: ({i+1}, {j+1})")
                    a = True
    if a == False:
        print("NAO HA NUMEROS REPETIDOS")


def entrada():
    n = int(input())
    for i in range(n):
        valores_entrada.append(int(input()))


entrada()
saida()
