# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que, dado o tempo de preparo do miojo, e os tempos das duas ampulhetas
# (ambos maiores que o tempo do miojo), determina o tempo mínimo necessário para o miojo ficar pronto.

valores_entrada = []


def saida():
    tempo_total = 0
    ampulheta1 = valores_entrada[1]
    ampulheta2 = valores_entrada[2]
    diferenca = 0

    while(diferenca < valores_entrada[0]):
        if(ampulheta1 > ampulheta2):
            diferenca = ampulheta1 - ampulheta2
            tempo_total += ampulheta2
            ampulheta1 = diferenca
            ampulheta2 = valores_entrada[2]
        else:
            diferenca = ampulheta2 - ampulheta1
            tempo_total += ampulheta1
            ampulheta2 = diferenca
            ampulheta1 = valores_entrada[1]
        if (diferenca == valores_entrada[0]):
            tempo_total += valores_entrada[0]
            return print(tempo_total)


def entrada():
    for i in range(3):
        valores_entrada.append(int(input()))


entrada()
saida()
