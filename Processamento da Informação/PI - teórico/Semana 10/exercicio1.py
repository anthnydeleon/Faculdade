# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve imprimir na tela o resultado usando duas casas decimais. Veja nos exemplos abaixo
# que na saída há o texto “Resultado = ” (com um espaço no final) precedendo o valor do resultado propriamente.


valores_entrada = []


def saida():
    soma = 0
    for i in range(((len(valores_entrada))//2)+1):
        for j in range(i, len(valores_entrada[0])-i):
            soma += valores_entrada[i][j]
    print(f"Resultado ={soma: ,.2f}")


def entrada():
    n = int(input())
    for i in range(n):
        linha = list()
        for elemento in input().strip().split():
            linha.append(float(elemento))
        valores_entrada.append(linha)


entrada()
saida()
