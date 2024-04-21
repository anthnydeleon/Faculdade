# Anthony Deleon de Almeida Costa
# Exercicio: Caso V seja de soma convergente, seu programa deve imprimir “SIM”; caso contrário, “NAO”.

a = input().split()
a = list(map(int, a))
print(a)
o = len(a)
b = int(len(a)/2)
print(b)
condicao = ""
if b == 1:
    print("SIM")
else:
    for i in range(b-1):
        if a[i]+a[o-1-i] > a[i+1]+a[o-1-(i+1)]:
            condicao = True
        else:
            condicao = False
            break
    if condicao == True:
        print("SIM")
    else:
        print("NAO")
