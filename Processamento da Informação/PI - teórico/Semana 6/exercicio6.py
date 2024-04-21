# Anthony Deleon de Almeida Costa
# Exercicio: O seu programa deve imprimir Erro na parentizacao, se o uso de parêntese não foi empregado corretamente,
# e Parentizacao correta, caso contrário.

def saida():
    parentese1 = "("
    parentese2 = ")"
    a = 0
    b = 0
    for i in valores_entrada:
        if a < b:
            print("Erro na parentizacao")
            break
        if i in parentese1:
            a += 1
        if i in parentese2:
            b += 1
    if a != b:
        print("Erro na parentizacao")
    else:
        print("Parentizacao correta")


valores_entrada = str(input())


saida()
