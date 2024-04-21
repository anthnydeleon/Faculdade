# Anthony Deleon de Almeida Costa
# Exercicio: Impressão do valores dos números reais repetidos (um número em cada linha).


valores_entrada = []
for i in range(2):
    valores_entrada.append(str(input()))
a = valores_entrada[0].split(" ")
b = valores_entrada[1].split(" ")
print(valores_entrada)


def saida():
    novo = 0
    if len(a) == len(b):
        for i in range(len(a)):
            novo += (float(b[i])-float(a[i]))**2
        print(f"{novo**0.5: ,.2f}")
    else:
        print("ERRO")


saida()
