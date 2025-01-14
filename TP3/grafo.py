from LE import Cola

class Grafo(object):

	"""Implementación del TDA Grafo."""
	def __init__(self) :
		self.vertices = {}
		self.cantidad = 0

	def insertar_vertice(self, v1):
		"""Recibe un vertice, y se encarga de 
		insertar una vertice dentro del Grafo."""
		
		# Si v1 ya está en el grafo supongo que no hace falta ponerlo denuevo
		if v1 in self.vertices : 
			return

		self.vertices[v1] = dict()
		self.cantidad += 1

	def unir_vertices(self, v1, v2, peso = 0) :
		"""Recibe dos vertices y un peso, une las dos vertices
		pertenecientes al grafo mediante una arista con la caracteristica
		de tener el peso recibido, si alguno de los dos vertices no 
		se encuentra devuelve False."""
		
		if (v1 not in self.vertices) or (v2 not in self.vertices):
			return False

		self.vertices[v1][v2] = peso
		self.vertices[v2][v1] = peso

		return True

	def peso(self, v1, v2) :
		"""Verifica si dos vertices estan en el grafo y si estan
		conectados, de ser asi se va a devolver el peso de su arista."""
		vertices = self.vertices

		if (v1 in vertices):
			if v2 in vertices[v1]:
				return vertices[v1][v2]

		return None

	def adyacentes(self, v1) :
		"""Devuelve los vertices adyacentes al vertice como una lista"""
		return list(self.vertices[v1])

	def vertices(self) :
		"""Devuelve todos los vertices del grafo en una lista"""
		vertices = []
		for v in self.vertices:
			vertices.append(v)
		return vertices

def bfs(grafo, origen) :
	"""Implementacion del recorrido BFS para el grafo
	(recorrido por amplitud)."""
	visitados = set()
	padre = {}
	orden = {}
	q = Cola()
	visitados.append(origen)
	padre[origen] = None
	orden[origen] = 0
	q.encolar(origen)
	while not q.esta_vacia():
		v = q.desencolar()
		for w in grafo.adyacentes(v):
			if not(w in visitados):
				visitados.append(w)
				padre[w] = v
				orden[w] = orden[v]+1
				q.encolar(w)
	return padre, orden

def dfs(grafo) :
	"""Implementacion del recorrido DFS para el grafo
	(recorrido por profundidad)."""
	visitados = set()
	padre = {}
	orden = {}
	for v in grafo.vertices():
		if not(v in visitados):
			padre[v] = None
			orden[v] = 0
			recorrido_dfs(grafo, v, visitados, padre, orden)
	return padre, orden

def recorrido_dfs(grafo, v, visitados, padre, orden) :
	visitados.append(v)
	for w in grafo.adyacentes(v):
		if not(w in visitados):
			padre[w] = v
			orden[w] = orden[v] +1
			recorrido_dfs(grafo, w, visitados, padre, orden)

def grados_entrada_bfs(grafo) :
	grados = {}
	for v in grafo.vertices():
		grados[v] = 0
	for v in grafo.vertices():
		for w in grafo.adyacentes(v):
			grados[w] += 1
	q = Cola()
	for v in grafo.vertices():
		if grados[v] == 0:
			q.encolar(v)
	resul = []
	while not q.esta_vacia():
		v = q.desencolar()
		resul.append[v]
		for w in grafo.adyacentes(v):
			grados[w] -= 1
			if grados[v] == 0:
				q.encolar(w)
	if len(resul) == len(grafo.vertices()): return resul 
	return None

def dijkstra(grafo, origen):
	"""Implementacion del Dijkstra para encontrar
	el camino mínimo para todas las vertices desde un 
	vertice origen."""
	dist = {}
	padre = {}
	for v in grafo.vertices():
		dist[v] = float('inf')
	dist[origen] = 0
	padre[origen] = None
	q = Heap()
	q.encolar((origen, 0))
	while not q.esta_vacia():
		v = q.desencolar()
		for w in grafo.adyacentes(v):
			if dist[v] + grafo.peso_union(v, w) < dist[w]:
				dist[w] = dist[v] + grafo.peso_union(v, w)
				padre[w] = v
				q.encolar((w, dist[w]))
	return padre, dist

def bellman_ford(grafo, origen) :
	"""Implementacion del Bellman-Ford para encontrar
	el camino mínimo para todas las vertices desde un 
	vertice origen."""
	dist = {}
	padre = {}
	for v in grafo.vertices():
		dist[v] = float('inf')
	dist[origen] = 0
	padre[origen] = None
	aristas = obtener_aristas(grafo)
	for i in range(grafo.vertices()):
		for v, w, peso in aristas:
			if dist[v] + peso < dist[w]:
				padre[w] = v
				dist[w] = dist[v] + peso
	for v, w, peso in aristas:
		if dist[v] + peso < dist[w]: return None
	return padre, dist

def mst_prim(grafo):
	"""Implementacion de Prim para armar un arbol 
	de tendido mínimo."""
	vertice = grafo.vertice_aleatorio()
	visitados = set()
	visitados.append(vertice)
	q = Heap()
	arbol = Grafo(grafo.vertices())
	for w in grafo.adyacentes(vertice):
		q.encolar((v, w), grafo.peso_union(v, w))
	while not q.esta_vacia():
		(v, w) = q.desencolar()
		if w in visitados:
			continue
		arbol.unir_vertices(v, w, grafo.peso_union(v, w))
		visitados.append(w)
		for u in grafo.adyacentes(w):
			if not(u in visitados): q.encolar((w, u), grafo.peso_union(w, u))
	return arbol

def camino_minimo_laberinto(laberinto, casilleroi, casillerof) :
	"""Se tiene un laberinto compuesto por casilleros. En dicho 
	laberinto se permite el movimiento en cualquier dirección,
	pero moverse de un casillero a otro puede producir una cantidad de 
	cansancio diferente. Este problema está modelado con un grafo. 
	Permite llegar desde un casillero inicial hasta el casillero final 
	(salida, de la cual conocemos ya su posición), con el menor 
	cansancio posible."""
	cansancio = {}
	padre = {}
	for v in laberinto.vertices():
		cansancio[v] = float('inf')
	cansancio[casilleroi] = 0
	padre[casilleroi] = None
	q = Heap()
	q.encolar((casilleroi, 0))
	while not q.esta_vacia():
		v = q.desencolar()
		for w in laberinto.adyacentes(v):
			if cansancio[v] + laberinto.peso_union(v, w) < cansancio[w]:
				cansancio[w] = cansancio[v] + laberinto.peso_union(v, w)
				padre[w] = v
				q.encolar((w, cansancio[w]))
	camino_minimo = Pila()
	while not casillerof is casilleroi:
		camino_minimo.apilar(casillerof)
		casillerof = padre[casillerof]
	return camino_minimo