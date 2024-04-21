# VALOR DE X E Y E SUAS INCERTEZAS
y = [13, 38, 52, 69]  # POSIÇÃO DO CARRINHO
x = [0.62, 2.1, 2.8, 3.6]  # VALOR DO TEMPO
ey = [1, 1, 2, 2]  # ERRO DA POSIÇÃO

# VALOR DE N
N = 4
# SOMA DE X/ERRO^2
sx = 0
# SOMA DE Y/ERRO^2
sy = 0
# SOMA 1/ERRO^2
s1 = 0
# SOMA XY/ERRO^2
sxy = 0
# SOMA DE X^2/ERRO^2
sxx = 0

# SOMATÓRIOS
for i in range(len(y)):
    sy = (y[i] / ey[i] ** 2) + sy
    sx = (x[i] / ey[i] ** 2) + sx
    s1 = (1 / ey[i] ** 2) + s1
    sxy = ((x[i] * y[i]) / ey[i] ** 2) + sxy
    sxx = (x[i] ** 2 / ey[i] ** 2) + sxx

# FORMÚLA PARA ACHAR O A
A = (sy * sx - s1 * sxy) / (sx ** 2 - sxx * s1)

# FORMÚLA PARA ACHAR O B
B = (sy - A * sx) / s1

# X^2
XX = (sxx / s1)

# X
X = (sx / s1)

# SIGMA
SIGMA = (N / s1)

# INCERTEZAS

# INCERTEZA A
erroA = (1 / N ** 0.5) * (SIGMA / (XX - X ** 2)) ** 0.5

# INCERTEZA B
erroB = (1 / N ** 0.5) * ((SIGMA * XX) / (XX - X ** 2)) ** 0.5

# PRINT VALORES
print("\nValor de V e X e suas incertezas: \nValor de v =", round(A, 2), "\nErro de v =", round(erroA, 2),
      "\nValor de x =", round(B, 2), "\nErro de x =", round(erroB, 2))
