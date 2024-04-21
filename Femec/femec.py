# VALORES DISTANCIA ---------- ALTERAR VALORES
L1 = [14.8, 16.7, 18.5]
L2 = [22.7, 24.4, 26]
L3 = [14.3, 16.4, 17.7]
L4 = [17.4, 19.4, 20.9]

# VALORES TEMPO --------- ALTERAR VALORES
T1 = [0.688, 0.696, 0.794]
T2 = [1.203, 1.201, 1.165]
T3 = [0.737, 0.733, 0.759]
T4 = [0.892, 0.888, 0.912]

# VALORES MÉDIOS DAS DISTANCIAS

L1med = 0
L2med = 0
L3med = 0
L4med = 0

for i in range(len(L1)):
    L1med = round((L1[i] / 3) + L1med, 4)
for i in range(len(L2)):
    L2med = round((L2[i] / 3) + L2med, 4)
for i in range(len(L3)):
    L3med = round((L3[i] / 3) + L3med, 4)
for i in range(len(L4)):
    L4med = round((L4[i] / 3) + L4med, 4)

print("\nMédia das distâncias:\nL1=", L1med, "\nL2=", L2med, "\nL3=", L3med, "\nL4=", L4med)

# VALORES MÉDIOS DOS TEMPOS

T1med = 0
T2med = 0
T3med = 0
T4med = 0

for i in range(len(T1)):
    T1med = round((T1[i] / 3) + T1med, 4)
for i in range(len(T2)):
    T2med = round((T2[i] / 3) + T2med, 4)
for i in range(len(T3)):
    T3med = round((T3[i] / 3) + T3med, 4)
for i in range(len(T4)):
    T4med = round((T4[i] / 3) + T4med, 4)

print("\nMédia dos tempos:\nt1=", T1med, "\nt2=", T2med, "\nt3=", T3med, "\nt4=", T4med)

# INCERTEZAS DE DISTANCIA
IncDist = 0.05

L1DP = 0
L2DP = 0
L3DP = 0
L4DP = 0

# desvio padrão L1
for i in range(len(L1)):
    L1DP = (((L1[i] - L1med) ** 2) + L1DP)
L1DP = (L1DP / 6) ** 0.5

# desvio padrão L2
for i in range(len(L2)):
    L2DP = (((L2[i] - L2med) ** 2) + L2DP)
L2DP = (L2DP / 6) ** 0.5

# desvio padrão L3
for i in range(len(L3)):
    L3DP = (((L3[i] - L3med) ** 2) + L3DP)
L3DP = (L3DP / 6) ** 0.5

# desvio padrão L4
for i in range(len(L4)):
    L4DP = (((L4[i] - L4med) ** 2) + L4DP)
L4DP = (L4DP / 6) ** 0.5

# INCERTEZA DE L1, L2, L3, L4
L1incerteza = round((IncDist ** 2 + L1DP ** 1) ** 0.5, 4)

L2incerteza = round((IncDist ** 2 + L2DP ** 1) ** 0.5, 4)

L3incerteza = round((IncDist ** 2 + L3DP ** 1) ** 0.5, 4)

L4incerteza = round((IncDist ** 2 + L4DP ** 1) ** 0.5, 4)

print("\nIncertezas das distancias:\nL1:", L1incerteza, "\nL2:", L2incerteza, "\nL3:", L3incerteza, "\nL4:",
      L4incerteza)

# INCERTEZAS DE TEMPO
IncTempo = 0.001

T1DP = 0
T2DP = 0
T3DP = 0
T4DP = 0

# desvio padrão T1
for i in range(len(T1)):
    T1DP = (((T1[i] - T1med) ** 2) + T1DP)
T1DP = (T1DP / 6) ** 0.5

# desvio padrão T2
for i in range(len(T2)):
    T2DP = (((T2[i] - T2med) ** 2) + T2DP)
T2DP = (T2DP / 6) ** 0.5

# desvio padrão T3
for i in range(len(T3)):
    T3DP = (((T3[i] - T3med) ** 2) + T3DP)
T3DP = (T3DP / 6) ** 0.5

# desvio padrão T4
for i in range(len(T4)):
    T4DP = (((T4[i] - T4med) ** 2) + T4DP)
T4DP = (T4DP / 6) ** 0.5

# INCERTEZA DE LT, T2, T3, T4
T1incerteza = round((IncTempo ** 2 + T1DP ** 2) ** 0.5, 4)

T2incerteza = round((IncTempo ** 2 + T2DP ** 2) ** 0.5, 4)

T3incerteza = round((IncTempo ** 2 + T3DP ** 2) ** 0.5, 4)

T4incerteza = round((IncTempo ** 2 + T4DP ** 2) ** 0.5, 4)

print("\nIncertezas dos tempos:\nt1:", T1incerteza, "\nt2:", T2incerteza, "\nt3:", T3incerteza, "\nt4:", T4incerteza)
