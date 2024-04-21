# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que recebe como entrada a Nota Final e realize sua conversão para o conceito correspondente.
# Valide a entrada de forma que o programa emita um aviso ao usuário caso a nota esteja fora do intervalo [0;10]

def conceito_final(nota: float):
    if nota > 10:
        nota_final = "Valor invalido! Digite um numero entre 0 e 10."
    if 9 <= nota <= 10:
        nota_final = "A"
    if 7.5 <= nota < 9:
        nota_final = "B"
    if 6 <= nota < 7.5:
        nota_final = "C"
    if 5 <= nota < 6:
        nota_final = "D"
    if 0 <= nota < 5:
        nota_final = "F"
    if nota < 0:
        nota_final = "Valor invalido! Digite um numero entre 0 e 10."
    return print(nota_final)


nota = float(input("Nota do aluno: "))
conceito_final(nota)
