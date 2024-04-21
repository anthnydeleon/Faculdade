# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa em que, dado um número inteiro, sejam apresentados seus três números anteriores
# ou seus três números posteriores.

def tres_ant_post(sequencia: str, x: int) -> int:
    if sequencia == "Posteriores":
        primeiro = x+1
        segundo = primeiro+1
        terceiro = segundo+1
        print(primeiro, ",", segundo, ",", terceiro)
    elif sequencia == "Anteriores":
        primeiro = x+1
        segundo = primeiro+1
        terceiro = segundo+1
        print(primeiro, ",", segundo, ",", terceiro)
    else:
        print("String inválida")


numero = int(input("Digite um número: "))
ant_post = str(input("Deseja números 'Anteriores' ou 'Posteriores': "))
tres_ant_post(ant_post, numero)
