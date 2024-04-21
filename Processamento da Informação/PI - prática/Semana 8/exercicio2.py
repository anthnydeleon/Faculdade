# Anthony Deleon de Almeida Costa
# Exercicio: A saída do seu programa deve consistir de uma única linha contendo a frase “MEDIA FINAL:”, seguida da média
# final do aluno, separados por espaço. A média final deve ser apresentada com quatro casas decimais de precisão.


def saida():
    soma = 0
    count = 0
    for i in range((len(valores_entrada))):
        if i > 0 and i < len(valores_entrada)-1:
            soma += valores_entrada[i]
            count += 1
    print(f"MEDIA FINAL: {soma/count:,.4f}")


valores_entrada = str(input()).split(" ")
valores_entrada = sorted(list(map(float, valores_entrada)))


saida()
