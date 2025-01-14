class Nodo:

	def __init__(self, dato = None, prox = None):
		self.dato = dato
		self.prox = None
		
class Cola:

	def __init__(self):
		self.ultimo = None
		self.frente = None

	def encolar(self, dato):
		Nuevo=Nodo(dato)

		if not self.frente:
			self.frente = Nuevo
			self.ultimo = Nuevo

		else:
			self.ultimo.prox = Nuevo
			self.ultimo = Nuevo

	def desencolar(self):
		if self.frente:
			frente = self.frente
			self.frente = self.frente.prox
			return frente.dato

		return None

	def esta_vacia(self):
		return self.frente == None

	def ver_frente(self):
		return self.frente.dato