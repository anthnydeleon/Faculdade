# MÉDIA DA VELOCIDADE MÉDIA E INCERTEZA

velomed= [20, 21, 23, 21]
incvelo= [2, 1, 1, 1]

MEDvelo = 0

for i in range(len(velomed)):
    MEDvelo = (velomed[i]/4)+MEDvelo
print("\nMédia das velocidades:", MEDvelo)

inc1 = 0

for i in range(len(incvelo)):
    inc1 = (incvelo[i]/4)+inc1

inc2= MEDvelo-inc1
inc3= MEDvelo+inc1

MEDinc= round((inc3-inc2)/2, 3)

print("\nIncerteza das médias da velocidade:", MEDinc)
