import matplotlib.pyplot as plt

def plotar():
    f = open("file.txt", "r")
    dados = f.read()
    tempo = []
    presas = []
    predadores = []
    stringTemp = ""
    contador = 0
    for numero in range(len(dados)):
        if(dados[numero] == ","):
            if(contador % 3 == 0):
                presas.append(float(stringTemp))
            elif(contador % 3 == 1):
                predadores.append(float(stringTemp))
            elif(contador % 3 == 2):
                tempo.append(float(stringTemp))
            stringTemp = ""
            contador += 1
        else:
            stringTemp += str(dados[numero])
    presas.pop()
    predadores.pop()
    tempo.pop()
    plt.plot(tempo, presas, '-')
    plt.plot(tempo, predadores, '-')
    plt.show()
plotar()
