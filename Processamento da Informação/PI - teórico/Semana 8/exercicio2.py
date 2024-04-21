# Anthony Deleon de Almeida Costa
# Exercicio: Combine as duas cadeias de caracteres da entrada como mostrado no exemplo abaixo e exiba a cadeia resultante.

valores_entrada = str(input())
valores_entrada = valores_entrada.split(" ")
a = valores_entrada[0]
b = valores_entrada[1]
novo = ""

if len(a) > len(b):
    for i in range(len(a)):
        try:
            novo += a[i]+b[i]
        except IndexError:
            novo += a[i]
else:
    for i in range(len(b)):
        try:
            novo += a[i]+b[i]
        except IndexError:
            novo += b[i]
print(novo)
