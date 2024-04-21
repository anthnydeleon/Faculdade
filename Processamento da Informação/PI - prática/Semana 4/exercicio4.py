# Anthony Deleon de Almeida Costa
# Exercicio: Faça um programa que receba uma lista de números natural e contabilize quantos desses números são primos.
# Lembre-se, um número x é primo se x > 1 e seus únicos divisores são 1 e o próprio x.

primos = []


def saida():
    for i in range(inputs):
        num = int(input("valor: "))
        if num > 1:
            for i in range(2, num):
                if (num % i) == 0:
                    break
            else:
                primos.append(num)
    print(f"dos {inputs} numeros informados {len(primos)} eram primos")


inputs = int(input("Digite a quant: "))
saida()
