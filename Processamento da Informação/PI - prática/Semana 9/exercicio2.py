# Anthony Deleon de Almeida Costa
# Exercicio: Seu programa deve imprimir na tela o resultado usando duas casas decimais. Veja nos exemplos abaixo
# que na saída há o texto “Borda = ” precedendo o valor da soma propriamente..


valores_entrada = []


def saida():
    soma = sum(valores_entrada[0]) + \
        sum(valores_entrada[len(valores_entrada)-1])
    for i in range(1, len(valores_entrada)-1):
        soma += valores_entrada[i][0] + \
            valores_entrada[i][len(valores_entrada[i])-1]
    print(f"Borda ={soma: ,.2f}")


def entrada():
    n = input().split()
    for i in range(int(n[0])):
        linha = list()
        for elemento in input().strip().split():
            linha.append(float(elemento))
        valores_entrada.append(linha)


entrada()
saida()
