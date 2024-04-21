meddist = [17, 24, 16, 19]
medtemp = [0.73, 1.19, 0.743, 0.897]
incdist = [1, 1, 1, 1]
inctempo = [0.03, 0.01, 0.008, 0.007]

# VELOCIDADE MÉDIA E INCERTEZA

for i in range(len(meddist)):
    velomed = round(meddist[i] / medtemp[i])
    print("Velocidade média:", velomed)


for i in range(len(meddist)):
    incvelo = round(velomed*((incdist[i]/meddist[i])**2+(inctempo[i]/medtemp[i])**2)**0.5,3)

    print("Incerteza velocidade média", incvelo)
