# Anthony Deleon de Almeida Costa
# Exercício: A entrada está composta por duas cadeia de caracteres, a primeira cadeia de caracteres corresponde ao
# tipo sanguíneo do paciente e a segunda cadeia de caracteres corresponde ao tipo sanguíneo do doador.
# Os valores de entrada são uma das seguintes cadeias: “A”, “B”, “O” e “AB”.

def transfusao_de_sangue(sangue):
    if sangue[0] == "A" and sangue[1] == "A" or sangue[0] == "A" and sangue[1] == "O":
        print("transfusao compativel")
    elif sangue[0] == "B" and sangue[1] == "B" or sangue[0] == "B" and sangue[1] == "O":
        print("transfusao compativel")
    elif sangue[0] == "AB" and sangue[1] == "A" or sangue[0] == "AB" and sangue[1] == "B" or sangue[0] == "AB" and sangue[1] == "O" or sangue[0] == "AB" and sangue[1] == "AB":
        print("transfusao compativel")
    elif sangue[0] == "O" and sangue[1] == "O":
        print("transfusao compativel")
    else:
        print("transfusao incompativel")


sangue = []
for i in range(2):
    sangue.append(str(input(f"Digite o {i+1}° tipo sanguíneo: ")))

transfusao_de_sangue(sangue)
