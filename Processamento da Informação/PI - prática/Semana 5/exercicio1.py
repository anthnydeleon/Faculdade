# Anthony Deleon de Almeida Costa
# Exercicio: Crie um programa que permita calcular o valor mínimo e o valor máximo de uma sequência de números inteiros.


qnt_input = int(input(""))
varA = int(input(""))
varB = varA
if qnt_input == 1:
    min = varA
    max = min
else:
    for i in range(qnt_input-1):
        p = int(input())
        min = (p+varA)/2 - abs(varA-p)/2
        max = (p+varB)/2 + abs(varB-p)/2
        varA = min
        varB = max
print("minimo = ", round(min))
print("maximo = ", round(max))
