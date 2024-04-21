# Anthony Deleon de Almeida Costa
# Exercicio: O seu programa receberá um número inteiro não negativo N que denota a quantidade de números que seu
# programa receberá para computar o valor total da soma. Na sequência seu programa receberá N números reais.

valor = []

N = int(input("Quantidade de números: "))

for i in range(N):
    valor.append(float(input("Valores: ")))

print("Total: %.2f" % sum(valor))
