# Anthony Deleon de Almeida Costa
# Exercício: Faça um programa para calcular o número máximo de regiões em que um retângulo pode ser dividido
# por n cortes (número inteiro positivo). Neste exercício deve se evitar o uso de laços ou condicionais.
# Use apenas programação sequencial.

def regioes_cortadas(num_de_cortes):
    retorno = round((num_de_cortes*(num_de_cortes+1))/2+1)
    return print("Quantidade de áreas formadas pelos cortes: ", retorno)


cortes = int(input("Insira a quantidade de cortes: "))
regioes_cortadas(cortes)
