# Anthony Deleon de Almeida Costa
# Exercicio: Escreva um programa que lê os 3 valores e responde “compro” caso o valor esteja dentro do que
# está disposto a pagar e “passo” caso o valor esteja além.

def compro_passo(espera_pagar, porcentagem_acima, valor_real):
    disposto_pagar = espera_pagar*(porcentagem_acima/100) + espera_pagar
    if valor_real > disposto_pagar:
        print("Eu passo")
    else:
        print("Eu compro")


espero_pagar = float(input("Preço que espera pagar: "))
porcentagem_acima = float(input("Porcentagem acima do preço disposto a pagar: "))
valor_real = float(input("Preço do produto: "))

compro_passo(espero_pagar, porcentagem_acima, valor_real)
