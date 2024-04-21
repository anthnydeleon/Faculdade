# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia três números inteiros e ordene-os em ordem crescente. No final,
# mostre os valores em ordem crescente.


numeros = []
for i in range(3):
    numeros.append(int(input(f"Digite o {i+1}° número: ")))

numeros.sort()
print(numeros)
