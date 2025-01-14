import csv
from grafo import Grafo
import sys
import random
import biblioteca as bib

def main():
    if len(sys.argv) < 2 :
        print("Parámetros incorrectos.")
        return

    aeropuertos = {}
    ciudades = {}
    precios = Grafo()
    tiempos = Grafo()

    arch_aeropuertos = sys.argv[1]
    vuelos = sys.argv[2] 

    procesar_aeropuertos(sys.argv[1], precios, tiempos, ciudades, aeropuertos)
    procesar_vuelos(sys.argv[2], precios, tiempos)

    for comando in sys.stdin:
        procesar_comando(comando.strip(), precios, tiempos, ciudades, aeropuertos)

def procesar_aeropuertos(arch_aeropuertos, precios, tiempos, ciudades, airports):
    """ Lee el archivo de aeropuertos recibido y agrega los datos a las 
        estructuras auxiliares"""

    with open(arch_aeropuertos, 'r') as aeropuertos:

        for aeropuerto in aeropuertos:
            aeropuerto = aeropuerto.strip().split(",")

            ciudades[aeropuerto[0]] = ciudades.get(aeropuerto[0], []) + [aeropuerto[1]]
            airports[aeropuerto[1]] = aeropuerto[0] # Guardo a que ciudad pertenece el aeropuerto
            precios.insertar_vertice(aeropuerto[1])
            tiempos.insertar_vertice(aeropuerto[1])


def procesar_vuelos(arch_vuelos, precios, tiempos):
    """ Lee el archivo de vuelos recibido y agrega los datos a las 
        estructuras auxiliares"""

    with open(arch_vuelos, 'r') as vuelos:

        for vuelo in vuelos:
            vuelo = vuelo.strip().split(",")

            precios.unir_vertices(vuelo[0], vuelo[1], int(vuelo[3])) # Aero1, Aero2, precio
            tiempos.unir_vertices(vuelo[0], vuelo[1], int(vuelo[2])) # Aero1, Aero2, tiempo

def camino_mas(grafo, origen, destino, ciudades):
    """ Busca y devuelve el camino óptimo entre un origen y 
        un destino recibido por el usuario."""

    posibles_caminos = []

    for aero_origen in ciudades[origen]: # origen
        for aero_destino in ciudades[destino]: # destino
            camino, peso = bib._camino_mas(grafo, aero_origen, aero_destino)
            posibles_caminos.append((camino, peso))

    camino_optimo = posibles_caminos[0] # camino, peso
    for camino in posibles_caminos:
        if camino[1] < camino_optimo[1]:
            camino_optimo = camino
   
    camino = camino_optimo[0]
    camino.reverse()
    return camino 

def camino_escalas(ciudades, grafo, origen, destino):

    posibles_caminos = []
    for aero_origen in ciudades[origen]:
        for aero_destino in ciudades[destino]:
            camino, escalas = bib.menos_escalas(grafo, aero_origen, aero_destino)
            posibles_caminos.append((camino, escalas))

    camino_optimo = posibles_caminos[0] # camino, peso
    for camino in posibles_caminos:
        if camino[1] < camino_optimo[1]:
            camino_optimo = camino
   
    camino = camino_optimo[0]
    camino.reverse()
    return camino 


def procesar_comando(comando, precios, tiempos, ciudades, aeropuertos):
    if len(comando) < 1:
        print("Parámetros incorrectos.")
        return

    if "camino_mas" in comando:
        comando = comando.split(",")

        if len(comando) < 3:
            print("Parámetros incorrectos")
            return

        origen = comando[1]
        destino = comando[2]

        if "rapido" in comando[0]:
            camino = camino_mas(tiempos, origen, destino, ciudades)
        if "barato" in comando[0]:
            camino = camino_mas(precios, origen, destino, ciudades)

        print(f"{' -> '.join(camino)}")
    

    if "vacaciones" in comando:
        comando = comando[11:]
        comando = comando.split(",")

        origen, largo = comando
        camino = bib.vacaciones(ciudades, tiempos, origen, int(largo))

        if not camino:
            print("No se encontro recorrido")
        else: 
            print(f"{' -> '.join(camino)}")

    if "camino_escalas" in comando:
        comando = comando[15:]
        comando = comando.split(",")

        origen = comando[0]
        destino = comando[1]

        camino = camino_escalas(ciudades, tiempos, origen, destino)
        print(f"{' -> '.join(camino)}")

    if "centralidad" in comando:
        # Esta funcion no anda
        _comando = comando.split(" ")
        cant = comando[1]

        aeropuertos = []
        cent = bib.centralidad(tiempos)
        cantidad = 0
        for w in sorted(cent, key=cent.get, reverse=True):
            if cantidad == cant:
                break
            aeropuertos.append(w)

        print(f"{' -> '.join(aeropuertos)}")

    if "recorrer_mundo_aprox" in comando:
        # Esta funcion no anda y es medio paja, creo que es mejor intentar hacer otra
        ciudad = comando[len("recorrer_mundo_aprox")+1:]
        print("la ciudad es {}".format(ciudad))

        camino = [float('inf'), []]

        for aero in ciudades[ciudad]:
            camino_posible, tiempo = bib.recorrer_mundo(tiempos, aero, ciudades, aeropuertos)
            if tiempo < camino[0]:
                camino[0] = tiempo
                camino[1] = camino_posible

        print(camino)

    if "itinerario" in comando:
        comando = comando.split(" ")
        ruta_arch = comando[1]

        itinerario, caminos = armar_itinerario(tiempos, precios, ruta_arch, ciudades, aeropuertos)
        print(itinerario)
        for camino in caminos:
            print(f"{' -> '.join(camino)}")

def armar_itinerario(tiempos, precios, ruta_arch, ciudades_dict, aeropuertos):
    """ Recibe las ciudades a visitar y sus restricciones
        Hace el orden topológico de estas ciudades
        Devuelve el orden en el que visitar las ciudades y sus caminos más rápidos
        """
        
    grafo = Grafo()
    with open(ruta_arch, "r") as arch:
        ciudades_a_visitar = arch.readline().strip().split(",")
        for ciudad in ciudades_a_visitar:
            grafo.insertar_vertice(ciudad)

        for linea in arch:
            ciudades = linea.strip().split(",")
            grafo.unir_vertices_dirigido(ciudades[0], ciudades[1])
   
    itinerario = bib.orden_topologico(grafo)

    caminos = []
    for i in range(len(itinerario)-1):
        camino_min = camino_mas(tiempos, itinerario[i], itinerario[i+1], ciudades_dict)
        caminos.append(camino_min)

    return itinerario, caminos


main()
