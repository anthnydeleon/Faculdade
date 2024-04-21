# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que permita ler duas strings A e B e calcula quantas vezes B ocorre em A.


A = input()
B = input()
C = len(B)
qnt = 0

for i in range(len(A)-(C-1)):
    if (A[i:i+C]) == B:
        qnt += 1

print(qnt)
