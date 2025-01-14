from heap import Heap
from cola import Cola, Nodo

def comparar(elem1, elem2):
	if elem1[1] == elem2[1]:
		return 0
	if elem1[1] < elem2[1] :
		return -1
	return 1

def camino_minimo(grafo, origen):
	""" Algoritmo de Bellman-Ford de camino mínimo.
		Devuelve una tupla con el camino mínimo y su peso."""

	dist = {}
	padre = {}
	for v in list(grafo.vertices):
		dist[v] = float('inf')

	dist[origen] = 0
	padre[origen] = None

	aristas = obtener_aristas(grafo)
	for i in range(len(list(grafo.vertices))):
		for v, w, peso in aristas:
			if dist[v] + peso < dist[w]:
				padre[w] = v
				dist[w] = dist[v] + peso

	return dist, padre

def ordenar_vertices(grafo, distancia):
	vertices_ordenados = []

	for v in grafo.vertices:
		vertices_ordenados.append((v, distancia[v]))

	vertices_ordenados.sort(key=lambda tup: tup[1], reverse=True)
	vertices_ordenados = [vertice[0] for vertice in vertices_ordenados if distancia[vertice[0]] != float('inf')]

	return vertices_ordenados

def centralidad(grafo):
	# No anda
    cent = {}
    for v in grafo.vertices: 
    	cent[v] = 0
    for v in grafo.vertices:
        # hacia todos los demas vertices
        distancia, padre = camino_minimo(grafo, v)
        cent_aux = {}
        for w in grafo.vertices: 
        	cent_aux[w] = 0
        # Aca filtramos (de ser necesario) los vertices a distancia infinita, 
        # y ordenamos de mayor a menor
        vertices_ordenados = ordenar_vertices(grafo, distancia) 
        for w in vertices_ordenados:
            if padre[w] in cent_aux:
            	cent_aux[padre[w]] += 1 + cent_aux[w]
        # le sumamos 1 a la centralidad de todos los vertices que se encuentren en 
        # el medio del camino
        for w in grafo.vertices:
            if w == v: 
            	continue
            cent[w] += cent_aux[w]
    return cent


def _camino_mas(grafo, origen, destino):
	""" Algoritmo de Bellman-Ford de camino mínimo.
		Devuelve una tupla con el camino mínimo y su peso."""

	dist = {}
	padre = {}
	for v in list(grafo.vertices):
		dist[v] = float('inf')

	dist[origen] = 0
	padre[origen] = None

	aristas = obtener_aristas(grafo)
	for i in range(len(list(grafo.vertices))):
		for v, w, peso in aristas:
			if dist[v] + peso < dist[w]:
				padre[w] = v
				dist[w] = dist[v] + peso
	
	camino = []
	aeropuerto = destino
	camino.append(aeropuerto)
	while True:
		if not padre[aeropuerto]:
			break

		camino.append(padre[aeropuerto])
		aeropuerto = padre[aeropuerto]

	return camino, dist[destino]


def obtener_aristas(grafo):
	aristas = []
	for v in list(grafo.vertices):
		for w in grafo.adyacentes(v):
			aristas.append((v, w, grafo.peso(v, w)))

	return aristas

def vacaciones(ciudades, grafo, origen, largo):

	for aeropuerto in ciudades[origen]:
		camino = [aeropuerto]
		visitados = []
		_vacaciones(grafo, camino, largo, visitados + [aeropuerto], aeropuerto)
		if len(camino) == int(largo) + 1:
			return camino
	return None

def _vacaciones(grafo, camino, largo, visitados, origen):
	if len(camino) == largo:
		if origen in grafo.adyacentes(camino[-1]):
			camino.append(origen)
			return False
		camino.pop(-1)

	for ady in grafo.adyacentes(camino[-1]):
		if ady in visitados:
			continue
		visitados.append(ady)
		camino.append(ady)
		if not _vacaciones(grafo, camino, largo, visitados, origen):
			return


def menos_escalas(grafo, origen, destino):

	padre = {}
	orden = {}
	orden[origen] = 0
	padre[origen] = None

	q = Cola()
	q.encolar(origen)

	while not q.esta_vacia():
		v = q.desencolar()

		for w in grafo.adyacentes(v):
			if w not in orden:
				padre[w] = v
				orden[w] = orden[v] + 1
				q.encolar(w)
	
	aeropuerto = destino
	camino = [aeropuerto]

	while True:
		if not padre[aeropuerto]:
			break

		camino.append(padre[aeropuerto])
		aeropuerto = padre[aeropuerto]

	return camino, orden[destino]

def recorrer_mundo(grafo, origen, ciudades, aeropuertos):
	costo = 0
	padre = {}

	recorridos = 0

	proximo, peso = ady_mas_cercano(grafo, origen, aeropuertos, None)

	camino = [origen, proximo]
	recorridos = set()
	recorridos.add(aeropuertos[origen])

	costo = _recorrer_mundo(grafo, costo, recorridos, aeropuertos, origen, camino, ciudades)

	return camino, costo


def _recorrer_mundo(grafo, costo, recorridos, aeropuertos, ultimo, camino, ciudades):
	if len(recorridos) == len(ciudades):
		return costo

	proximo, peso = ady_mas_cercano(grafo, camino[-1], aeropuertos, camino[-2])
	camino.append(proximo)
	recorridos.add(aeropuertos[proximo])

	return _recorrer_mundo(grafo, costo+peso, recorridos, aeropuertos, proximo, camino, ciudades)



def ady_mas_cercano(grafo, v, aeropuertos, ultimo):
	closest = None
	costo = float('inf')

	for w in grafo.adyacentes(v):
		if not closest:
			closest = w
			costo = grafo.peso(v, w)
		else:
			if grafo.peso(v, w) < costo and w != ultimo:
				closest = w
				costo = grafo.peso(v, w)

	return closest, costo
