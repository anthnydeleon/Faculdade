# Anthony Deleon de Almeida Costa
# Exercicio: A primeira linha contém um número inteiro N (1 < N ≤ 100) representando a quantidade de medições
# de velocidade do motor em um determinado teste. Cada uma das próximas N linhas consiste de um único inteiro M
# (0 ≤ M ≤ 10000) representando o número de RPM (rotações por minuto) daquela medida.

valores_entrada = []


def saida():
    for i in range(len(valores_entrada)):
        if (valores_entrada[i] < valores_entrada[i-1]) and (i > 0):
            print(i+1)
            break
        elif i == len(valores_entrada)-1:
            print("0")


def entrada():
    j = int(input("Quant de i: "))
    for i in range(j):
        valores_entrada.append(int(input("Números: ")))


entrada()
saida()
