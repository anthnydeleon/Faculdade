# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa que leia o nome de um vendedor, o seu salário fixo e o total de
# vendas efetuadas por ele no mês (em dinheiro). Sabendo que este vendedor ganha 5% de comissão
# sobre suas vendas efetuadas, informar o total a receber no final do mês, com duas casas decimais.


def salario(vendas, salario_funcionario):
    comissao = vendas*0.05
    retorno = comissao + salario_funcionario
    return retorno


nome = str(input("Nome: "))
salario_inicial = float(input("Salario: "))
vendas_mensal = float(input("Vendas: "))

print(f"{nome} deve receber R$ %.2f" % salario(vendas_mensal, salario_inicial))
