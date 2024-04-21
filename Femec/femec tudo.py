                        #VALORES DISTANCIA ----------ALTERAR VALORES
L1= [14.6, 11, 12.8]
L2= [27.4, 23.9, 25.5]
L3= [16, 12.5, 14.3]
L4= [19, 15.5, 17.5]

                         #VALORES TEMPO ---------ALTERAR VALORES
T1= [0.986, 0.938, 1.16]
T2= [1.946, 1.877, 2.336]
T3= [0.875, 0.858, 1.007]
T4= [1.050, 1.013, 1.168]

                        #VALORES MÉDIOS DAS DISTANCIAS

L1med=0
L2med=0
L3med=0
L4med=0

for i in range(len(L1)):
    L1med=round((L1[i]/3)+L1med,0)
for i in range(len(L2)):
    L2med=round((L2[i]/3)+L2med,0)
for i in range(len(L3)):
    L3med=round((L3[i]/3)+L3med,0)
for i in range(len(L4)):
    L4med=round((L4[i]/3)+L4med,0)

print("\nMédia das distâncias:\nL1=",L1med, "\nL2=", L2med, "\nL3=", L3med, "\nL4=", L4med)


                      #VALORES MÉDIOS DOS TEMPOS

T1med=0
T2med=0
T3med=0
T4med=0

for i in range(len(T1)):
    T1med=round((T1[i]/3)+T1med,2)
for i in range(len(T2)):
    T2med=round((T2[i]/3)+T2med,1)
for i in range(len(T3)):
    T3med=round((T3[i]/3)+T3med,3)
for i in range(len(T4)):
    T4med=round((T4[i]/3)+T4med,2)

print("\nMédia dos tempos:\nt1=",T1med, "\nt2=", T2med, "\nt3=", T3med, "\nt4=", T4med)

                        #INCERTEZAS DE DISTANCIA
IncDist= 0.05

L1DP= 0
L2DP= 0
L3DP= 0
L4DP= 0

#desvio padrão L1
for i in range(len(L1)):
    L1DP=(((L1[i]-L1med)**2)+L1DP)
L1DP= (L1DP/6)**0.5

#desvio padrão L2
for i in range(len(L2)):
    L2DP=(((L2[i]-L2med)**2)+L2DP)
L2DP= (L2DP/6)**0.5

#desvio padrão L3
for i in range(len(L3)):
    L3DP=(((L3[i]-L3med)**2)+L3DP)
L3DP= (L3DP/6)**0.5

#desvio padrão L4
for i in range(len(L4)):
    L4DP=(((L4[i]-L4med)**2)+L4DP)
L4DP= (L4DP/6)**0.5

#INCERTEZA DE L1, L2, L3, L4
L1incerteza=round(((IncDist)**2+(L1DP)**1)**0.5,1)

L2incerteza=round(((IncDist)**2+(L2DP)**1)**0.5,1)

L3incerteza=round(((IncDist)**2+(L3DP)**1)**0.5,1)

L4incerteza=round(((IncDist)**2+(L4DP)**1)**0.5,1)

print("\nIncertezas das distancias:\nL1:", L1incerteza, "\nL2:", L2incerteza, "\nL3:", L3incerteza, "\nL4:", L4incerteza)

                        #INCERTEZAS DE TEMPO
IncTempo= 0.001

T1DP= 0
T2DP= 0
T3DP= 0
T4DP= 0

#desvio padrão T1
for i in range(len(T1)):
    T1DP=(((T1[i]-T1med)**2)+T1DP)
T1DP= (T1DP/6)**0.5

#desvio padrão T2
for i in range(len(T2)):
    T2DP=(((T2[i]-T2med)**2)+T2DP)
T2DP= (T2DP/6)**0.5

#desvio padrão T3
for i in range(len(T3)):
    T3DP=(((T3[i]-T3med)**2)+T3DP)
T3DP= (T3DP/6)**0.5

#desvio padrão T4
for i in range(len(T4)):
    T4DP=(((T4[i]-T4med)**2)+T4DP)
T4DP= (T4DP/6)**0.5

#INCERTEZA DE LT, T2, T3, T4
T1incerteza=round(((IncTempo)**2+(T1DP)**2)**0.5,2)

T2incerteza=round(((IncTempo)**2+(T2DP)**2)**0.5,1)

T3incerteza=round(((IncTempo)**2+(T3DP)**2)**0.5,2)

T4incerteza=round(((IncTempo)**2+(T4DP)**2)**0.5,2)

print("\nIncertezas dos tempos:\nt1:",T1incerteza,"\nt2:",T2incerteza,"\nt3:",T3incerteza,"\nt4:",T4incerteza)

                        #VELOCIDADE MÉDIA E INCERTEZA

VeloMed1=round(L1med/T1med)
VeloMed2=round(L2med/T2med)
VeloMed3=round(L3med/T3med)
VeloMed4=round(L4med/T4med)



print("\nVelocidade média:\nv1:",VeloMed1,"\nv2:",VeloMed2,"\nv3:",VeloMed3,"\nv4:",VeloMed4)
print("\nIncertezas da velocidade:\nL1:", round(L1incerteza), "\nL2:", round(L2incerteza), "\nL3:", round(L3incerteza), "\nL4:", round(L4incerteza))

                        #POSIÇÃO DO CARRINHO x TEMPO
#POSIÇÃO CARRINHO
y = [13, 39, 53, 70]

print("\nPosição do carrinho:\n2:",y)

#ERRO DA POSIÇÃO
ey = [1, 1, 2, 2]

print("\nErro de posição:\n2:", ey)

#TEMPO CARRINHO
x = [1.03, 3.1, 4.0, 5.1]


print("\nTempo do carrinho:\n2:", x)

#ERRO DO TEMPO
ErroTemp1=0
ErroTemp2=(T1incerteza**2+ErroTemp1**2)**0.5
ErroTemp3=(T2incerteza**2+ErroTemp2**2)**0.5
ErroTemp4=(T3incerteza**2+ErroTemp3**2)**0.5
ErroTemp5=(T4incerteza**2+ErroTemp4**2)**0.5

ErroTemp2=round(ErroTemp2,2)
ErroTemp3=round(ErroTemp3,2)
ErroTemp4=round(ErroTemp4,2)
ErroTemp5=round(ErroTemp5,2)

print("\nErro de tempo:\n2:",ErroTemp2,"\n3:",ErroTemp3,"\n4:",ErroTemp4,"\n5:",ErroTemp5)

                        #MÉDIA DAS VELOCIDADES E INCERTEZAS

MediaVelo=(VeloMed1+VeloMed2+VeloMed3+VeloMed4)/4
print("\nMédia das velocidades médias:", MediaVelo)


                        #VALOR DE X E Y E SUAS INCERTEZAS
#VALOR DE N
N= 4
#SOMA DE X/ERRO^2
sx= 0
#SOMA DE Y/ERRO^2
sy= 0
#SOMA 1/ERRO^2
s1= 0
#SOMA XY/ERRO^2
sxy= 0
#SOMA DE X^2/ERRO^2
sxx= 0


##SOMATÓRIOS
for i in range(len(y)):
    sy=(y[i]/ey[i]**2)+sy
    sx=(x[i]/ey[i]**2)+sx
    s1=(1/ey[i]**2)+s1
    sxy=((x[i]*y[i])/ey[i]**2)+sxy
    sxx=(x[i]**2/ey[i]**2)+sxx

#FORMÚLA PARA ACHAR O A
A= (sy * sx - s1 * sxy ) / ( sx ** 2 - sxx * s1)

#FORMÚLA PARA ACHAR O B
B= (sy-A*sx)/s1

#X^2
XX=(sxx/s1)

#X
X=(sx/s1)

#SIGMA
SIGMA= (N/s1)

#INCERTEZAS

    #INCERTEZA A
erroA= (1/N**0.5)*(SIGMA/(XX-X**2))**0.5

    #INCERTEZA B
erroB= (1/N**0.5)*((SIGMA*XX)/(XX-X**2))**0.5


#PRINT VALORES
print("\n",y)
print("\n",x)
print("\n",ey)


print("\nValor de V e X e suas incertezas: \nValor de v =", round(A,2),"\nErro de v =", round(erroA,2),"\nValor de x =", round(B,2),"\nErro de x =", round(erroB,2))

print(sy)
print(sx)
print(s1)
print(sxy)
print(sxx)