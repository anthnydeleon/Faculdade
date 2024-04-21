# Anthony Deleon de Almeida Costa
# Exercício: A saída do seu programa deve apresentar os conjuntos de Interseção e União, ambos ordenados na sua
# forma lexicográfica. A formatação deve seguir os exemplos ilustrados na seguinte tabela.


valores_entrada = []


def saida():
    intersecao = []
    uniao = valores_entrada[0]
    intersecao_print = ""
    uniao_print = ""
    for i in range(len(valores_entrada[1])):
        if valores_entrada[1][i] in valores_entrada[0]:
            intersecao.append(valores_entrada[1][i])
        else:
            uniao.append(valores_entrada[1][i])
    intersecao = sorted(intersecao)
    uniao = sorted(uniao)
    for i in range(len(intersecao)):
        intersecao_print += intersecao[i] + " "
    for i in range(len(uniao)):
        uniao_print += uniao[i] + " "
    print(f"Intersecao: {intersecao_print}")
    print(f"Uniao: {uniao_print}")
    print(f"Similaridade = {len(intersecao)/len(uniao): ,.2f}")


def entrada():
    for i in range(2):
        valores_entrada.append(str(input()).split(" "))


entrada()
saida()
