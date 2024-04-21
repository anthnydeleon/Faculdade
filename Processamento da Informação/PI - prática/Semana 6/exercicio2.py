# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa Python que leia uma string e remova todas as suas vogais. Por exemplo, se a entrada
# for “macaco” o e seu programa deve imprimir “mcc”. Considere que o usuário digitará apenas palavras (strings) sem acento.


valores_entrada = str(input()).lower()
valores_entrada = valores_entrada.replace("a", "")
valores_entrada = valores_entrada.replace("e", "")
valores_entrada = valores_entrada.replace("i", "")
valores_entrada = valores_entrada.replace("o", "")
valores_entrada = valores_entrada.replace("u", "")

print(valores_entrada)
