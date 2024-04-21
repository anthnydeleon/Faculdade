# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que recebe como entrada o Conceito Final e imprima a mensagem correspondente.
# Valide a entrada de forma que o programa emita um aviso ao usuário caso o Conceito Final não seja A, B, C, D, F ou O


def conceito_final(nota: str):
    if nota == "A" or nota == "B" or nota == "C" or nota == "D":
        conceito = "APROVADO"
    elif nota == "F":
        conceito = "REPROVADO POR DESEMPENHO"
    elif nota == "O":
        conceito = "REPROVADO POR FREQUENCIA"
    else:
        conceito = "Conceito invalido!"
    return print(conceito)


nota = str(input("Nota do aluno: "))
conceito_final(nota)
