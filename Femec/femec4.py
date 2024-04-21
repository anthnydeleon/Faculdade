meddist = [17, 24, 16, 19]
medtemp = [0.73, 1.19, 0.743, 0.897]
incdist = [1, 1, 1, 1]
inctempo = [0.03, 0.01, 0.008, 0.007]

# POSIÇÃO DO CARRINHO x TEMPO
# POSIÇÃO CARRINHO
Ponto1 = 0
Ponto2 = round(meddist[0] + Ponto1, 3)
Ponto3 = round(meddist[1] + Ponto2, 3)
Ponto4 = round(meddist[2] + Ponto3, 3)
Ponto5 = round(meddist[3] + Ponto4, 3)

y = [Ponto2, Ponto3, Ponto4, Ponto5]

print("\nPosição do carrinho:\n2:", Ponto2, "\n3:", Ponto3, "\n4:", Ponto4, "\n5:", Ponto5)

# ERRO DA POSIÇÃO
ErroPos1 = 0
ErroPos2 = (incdist[0] ** 2 + ErroPos1 ** 2) ** 0.5
ErroPos3 = (incdist[1] ** 2 + ErroPos2 ** 2) ** 0.5
ErroPos4 = (incdist[2] ** 2 + ErroPos3 ** 2) ** 0.5
ErroPos5 = (incdist[3] ** 2 + ErroPos4 ** 2) ** 0.5

ErroPos2 = (round(ErroPos2))
ErroPos3 = (round(ErroPos3))
ErroPos4 = (round(ErroPos4))
ErroPos5 = (round(ErroPos5))

ey = [ErroPos2, ErroPos3, ErroPos4, ErroPos5]

print("\nErro de posição:\n2:", ErroPos2, "\n3:", ErroPos3, "\n4:", ErroPos4, "\n5:", ErroPos5)

# TEMPO CARRINHO
Tempo1 = 0
Tempo2 = round(medtemp[0] + Tempo1, 3)
Tempo3 = round(medtemp[1] + Tempo2, 3)
Tempo4 = round(medtemp[2] + Tempo3, 3)
Tempo5 = round(medtemp[3] + Tempo4, 3)

x = [Tempo2, Tempo3, Tempo4, Tempo5]

print("\nTempo do carrinho:\n2:", Tempo2, "\n3:", Tempo3, "\n4:", Tempo4, "\n5:", Tempo5)

# ERRO DO TEMPO
ErroTemp1 = 0
ErroTemp2 = (inctempo[0] ** 2 + ErroTemp1 ** 2) ** 0.5
ErroTemp3 = (inctempo[1] ** 2 + ErroTemp2 ** 2) ** 0.5
ErroTemp4 = (inctempo[2] ** 2 + ErroTemp3 ** 2) ** 0.5
ErroTemp5 = (inctempo[3] ** 2 + ErroTemp4 ** 2) ** 0.5

ErroTemp2 = round(ErroTemp2, 3)
ErroTemp3 = round(ErroTemp3, 3)
ErroTemp4 = round(ErroTemp4, 3)
ErroTemp5 = round(ErroTemp5, 3)

print("\nErro de tempo:\n2:", ErroTemp2, "\n3:", ErroTemp3, "\n4:", ErroTemp4, "\n5:", ErroTemp5)

