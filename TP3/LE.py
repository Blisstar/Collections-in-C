class ListaEnlazada:
	"""Modela una lista enlazada."""
	def __init__(self):
		"""Crea una lista enlazada vacía."""
		# referencia al primer nodo (None si la lista está vacía)
		self.prim = None
		# cantidad de elementos de la lista
		self.len = 0
	def pop(self, i=None):
		"""Elimina el nodo de la posición i, y devuelve el dato contenido.
		Si i está fuera de rango, se levanta la excepción IndexError.
		Si no se recibe la posición, devuelve el último elemento."""
		if i is None:
			i = self.len - 1
		if i < 0 or i >= self.len:
			raise IndexError("Índice fuera de rango")
		if i == 0:
			# Caso particular: saltear la cabecera de la lista
			dato = self.prim.dato
			self.prim = self.prim.prox
		else:
			# Buscar los nodos en las posiciones (i-1) e (i)
			n_ant = self.prim
			n_act = n_ant.prox
			for pos in range(1, i):
				n_ant = n_act
				n_act = n_ant.prox
			# Guardar el dato y descartar el nodo
			dato = n_act.dato
			n_ant.prox = n_act.prox
		self.len -= 1
		return dato
	def remove(self, x):
		"""Borra la primera aparición del valor x en la lista.
		Si x no está en la lista, levanta ValueError"""
		if self.len == 0:
			raise ValueError("Lista vacía")
		if self.prim.dato == x:
			# Caso particular: saltear la cabecera de la lista
			self.prim = self.prim.prox
		else:
			# Buscar el nodo anterior al que contiene a x (n_ant)
			n_ant = self.prim
			n_act = n_ant.prox
			while n_act is not None and n_act.dato != x:
				n_ant = n_act
				n_act = n_ant.prox
			if n_act == None:
				raise ValueError("El valor no está en la lista.")
			# Descartar el nodo
			n_ant.prox = n_act.prox
		self.len -= 1	
	def insert(self, i, x):
		"""Inserta el elemento x en la posición i.
		Si la posición es inválida, levanta IndexError"""
		if i < 0 or i > self.len:
			raise IndexError("Posición inválida")
		nuevo = _Nodo(x)
		if i == 0:
			# Caso particular: insertar al principio
			nuevo.prox = self.prim
			self.prim = nuevo
		else:
			# Buscar el nodo anterior a la posición deseada
			n_ant = self.prim
			for pos in range(1, i):
				n_ant = n_ant.prox
			# Intercalar el nuevo nodo
			nuevo.prox = n_ant.prox
			n_ant.prox = nuevo
		self.len += 1
	def append(self,dato_nuevo):
		"""Agrega un elemento al final de la lista."""
		if self.len==0:
			self.prim=_Nodo(dato_nuevo)
		else:	
			n_ant=self.prim
			for pos in range(1,self.len):
				n_ant=n_ant.prox
			n_ant.prox=_Nodo(dato_nuevo)
		self.len+=1
	def __str__(self):
		"""Representacion de la ListaEnlazada como cdena de texto"""
		res=''
		if self.len==0:
			return 'None'
		else:
			n_ant=self.prim
			str_dato=n_ant.dato
			res+=f'{str_dato}'
			for pos in range(1,self.len):
				n_ant=n_ant.prox
				str_dato=n_ant.dato
				res+=f',{str_dato}'
		return res
	def invertir(self):
		"""Invierte la ListaEnlazada"""
		if self.len==0:
			print('Lista vacia.')
		pila1=Pila()
		for pos in range(self.len):
			n_act=self.prim
			dato=n_act.dato
			pila1.apilar(dato)
			self.prim=self.prim.prox
		self.prim=_Nodo(pila1.desapilar())	
		n_act=self.prim
		while not(pila1.esta_vacia()):
			n_act.prox=_Nodo(pila1.desapilar())
			n_act=n_act.prox
	def suma_acumulativa(self):
		'''Para una lista simplemente enlazada de números (que solo mantiene una referencia al
		primer nodo). Devuelve una nueva lista (del mismo largo) tal que el nodo i de la nueva
		lista contenga la suma acumulativa de los elementos de la lista origina hasta el nodo i.'''
		listae2=ListaEnlazada()
		n_act=self.prim
		res_suma=n_act.dato
		listae2.prim=_Nodo(res_suma)
		n_act2=listae2.prim
		for pos in range(1,self.len):
			n_act=n_act.prox
			res_suma+=n_act.dato
			n_act2.prox=_Nodo(res_suma)
			n_act2=n_act2.prox
		listae2.len=self.len
		return listae2
	def extend(self,listae):
		'''Recibe una ListaEnlazada y agrega a la lista actual 
		los elementos que se encuentran en la lista recibida.'''
		act=self.prim
		sig=act.prox
		while sig:
			act=act.prox
			sig=act.prox
		act.prox=listae.prim
		self.len+=listae.len
	def remover_todos(self,elemento):
		'''Recibe un elemento y remueve de la lista todas las apariciones del mismo, devolviendo 
		la cantidad de elementos removidos. La lista debe ser recorrida una sola vez.'''
		if self.prim==None:
			raise ValueError('Lista vacia')
		ant=0
		act=self.prim
		sig=act.prox
		cont=0
		if act.dato==elemento:
			cont=1
			self.prim=sig
		while sig:
			ant=act
			act=act.prox
			sig=act.prox
			if act.dato==elemento:
				cont+=1
				ant.prox=sig
		self.len-=cont
		return cont
	def duplicar(self,elemento):
		'''Recibe un elemento y duplica todas las apariciones del mismo.'''
		if self.prim==None:
			raise ValueError('La lista esta vacia')
		act=self.prim
		sig=act.prox
		cont=0
		while sig:
			if act.dato==elemento:
				act.prox=_Nodo(elemento,sig)
				act=act.prox
				cont+=1
			act=act.prox
			sig=act.prox
		if act.dato==elemento:
			act.prox=_Nodo(elemento,sig)
			cont+=1
		self.len+=cont
	def filter(self,f):
		'''Recibe una función y devuelve una nueva lista enlazada con los
		elementos para los cuales la aplicación de la funcion devuelve True.'''
		res=ListaEnlazada()
		act=self.prim
		interruptor=False
		inicio=None
		act1=None
		cont=0
		while act:
			if f(act.dato):
				if interruptor:
					act1.prox=_Nodo(act.dato)
					act1=act1.prox
					cont+=1
				else:
					inicio=_Nodo(act.dato)
					act1=inicio
					interruptor=True
					cont=1
			act=act.prox
		res.prim=inicio
		res.len=cont
		return res

class ListaCircular:
	'''Una lista circular es una lista cuyo último nodo esta ligado
	al primero, de modo que es posible recorrerla infinitamente.'''
	def __init__(self):
		self.prim=None
	def insert(self,posicion,dato):
		"""Inserta un elemento en la posición que recibe.
		Si la posición es inválida, levanta IndexError"""
		if self.prim==None:
			if posicion==0:
				self.prim=_Nodo(dato)
				self.prim.prox=self.prim
				return
			else:
				raise IndexError('Posición inválida')
		if posicion==0:
			act=self.prim
			sig=act.prox
			while sig!=self.prim:
				act=act.prox
				sig=act.prox
			self.prim=_Nodo(dato,sig)
			act.prox=self.prim
			return
		ant=self.prim
		act=ant.prox
		posicion-=1
		while posicion!=0:
			if act==self.prim:
				raise IndexError('Posición inválida')
			ant=act
			act=ant.prox
			posicion-=1
		ant.prox=_Nodo(dato,act)
	def append(self,dato):
		"""Agrega un elemento al final de la lista."""
		if self.prim==None:
			self.prim=_Nodo(dato)
			self.prim.prox=self.prim
			return
		ant=self.prim
		act=ant.prox
		while act!=self.prim:
			ant=act
			act=ant.prox
		ant.prox=_Nodo(dato,act)
	def remove(self,dato):
		"""Borra la primera aparición del elemento recibido en la lista.
		Si el elemento no está en la lista, levanta ValueError"""
		if self.prim==None:
			raise ValueError('Lista vacia')
		if dato==self.prim.dato:
			self.prim=self.prim.prox
		ant=0
		act=self.prim
		sig=act.prox
		while sig!=self.prim:
			if act.dato==dato:
				ant.prox=sig
				return
			ant=act
			act=ant.prox
			sig=act.prox
		if act.dato==dato:
			ant.prox=sig
		else:
			raise ValueError('El elemento no está en la lista')
	def pop(self,i):
		"""Elimina el nodo de la posición i, y devuelve el dato contenido.
		Si i está fuera de rango, se levanta la excepción IndexError.
		Si no se recibe la posición, devuelve el último elemento."""
		if self.prim==None:
			raise ValueError('Lista vacia')
		if i==0:
			ant=self.prim
			act=ant.prox
			dato=self.prim.dato
			if act==self.prim:
				self.prim=None
				return dato
			while act!=self.prim:
				ant=act
				act=ant.prox
			self.prim=act.prox
			ant.prox=self.prim
			return dato
		ant=self.prim
		act=ant.prox
		sig=act.prox
		i-=1
		while i!=0:
			if act==self.prim:
				raise IndexError('Posición inválida')
			ant=act
			act=ant.prox
			sig=act.prox
			i-=1
		dato=act.dato
		ant.prox=sig
		return dato

class ListaDobleEnlazada:
	'''Una lista doblemente enlazada es una lista en la cual cada nodo tiene una referencia
	al anterior además de al próximo de modo que es posible recorrerla en ambas direcciones.'''
	def __init__(self):
		self.prim=None
	def insert(self,posicion,dato):
		"""Inserta un elemento en la posición que recibe.
		Si la posición es inválida, levanta IndexError"""
		if self.prim==None:
			if posicion==0:
				self.prim=_NodoDoble(dato)
				return
			raise IndexError('Posición inválida')
		if posicion==0:
			nuevo=_NodoDoble(dato,None,self.prim)
			self.prim.prev=nuevo
			self.prim=nuevo
			return
		ant=self.prim
		act=ant.prox
		posicion-=1
		while posicion!=0:
			if act==None:
				raise IndexError('Posición inválida')
			ant=act
			act=ant.prox
			posicion-=1
		if act:
			nuevo=_NodoDoble(dato,ant,act)
			act.prev=nuevo
			ant.prox=nuevo
		else:
			ant.prox=_NodoDoble(dato,ant,act)

class _Nodo:
	def __init__(self, dato=None, prox=None):
		self.dato = dato
		self.prox = prox
	def __str__(self):
		return str(self.dato)

class _NodoDoble:
	def __init__(self,dato=None,prev=None,prox=None):
		self.prev=prev
		self.dato=dato
		self.prox=prox
	def __str__(self):
		return str(self.dato)

class Pila:
	"""Representa una pila con operaciones de apilar, desapilar y
	verificar si está vacía."""
	def __init__(self):
		"""Crea una pila vacía."""
		self.items = []
	def esta_vacia(self):
		"""Devuelve True si la lista está vacía, False si no."""
		return len(self.items) == 0
	def apilar(self, x):
		"""Apila el elemento x."""
		self.items.append(x)
	def desapilar(self):
		"""Devuelve el elemento tope y lo elimina de la pila.
		Si la pila está vacía levanta una excepción."""
		if self.esta_vacia():
			raise IndexError("La pila está vacía")
		return self.items.pop()

class Cola:
	"""Representa a una cola, con operaciones de encolar y
	desencolar. El primero en ser encolado es también el primero
	en ser desencolado."""
	def __init__(self):
		"""Crea una cola vacía."""
		self.items = []
	def encolar(self, x):
		"""Encola el elemento x."""
		self.items.append(x)
	def desencolar(self):
		"""Elimina el primer elemento de la cola y devuelve su
		valor. Si la cola está vacía, levanta ValueError."""
		if self.esta_vacia():
			raise ValueError("La cola está vacía")
		return self.items.pop(0)
	def esta_vacia(self):
		"""Devuelve True si la cola esta vacía, False si no."""
		return len(self.items) == 0

class Heap:
	def __init__(self):
		self.arreglo = []
		self.cantidad = 0
	def encolar(self, elem):
		if not elem: return False
		

def ver_lista(nodo):
	"""Recorre todos los nodos a través de sus enlaces,
	mostrando sus contenidos."""
	while nodo is not None:
		print(nodo)
		nodo = nodo.prox