ruta_tweets='/home/joel/Documentos/Ejercicios de Python/TP N°2:Generador de Tweets/tweets.csv'
ruta_fav='/home/joel/Documentos/Ejercicios de Python/TP N°2:Generador de Tweets/favoritos.csv'
import sys
from csv import reader
import random
def main():
	'''Determina con que comandos se ejecuta las funciones generar, favoritos y trending en la cosola.
	Todos estas funciones forman un programa para generar tweets pseudoaleatoriamente.'''
	if len(sys.argv)==1:
		print('No ingresó ningún parámetro.')
		return
	if sys.argv[1].lower()=='generar':	
		if len(sys.argv)==2:
			generar_main()
		if len(sys.argv)>=3:
			generar_main(sys.argv[2:])
	if sys.argv[1].lower()=='trending':
		if len(sys.argv)>3:
			print('Ingresó parámetros de más.')
			return
		if len(sys.argv)<3:
			print('No ingresó los suficientes parámetros.')
			return
		if not(sys.argv[2].isdigit()):
			print('Por favor ingrese un número entero.')
			return
		trending_main(int(sys.argv[2]))
	if sys.argv[1].lower()=='favoritos':
		if len(sys.argv)>3:
			print('Ingresó parámetros de más.')
			return
		if len(sys.argv)==3 and not(sys.argv[2].isdigit()):
			print('Por favor ingrese un número entero.')
			return
		if len(sys.argv)==3:
			favoritos_main(int(sys.argv[2]))
		else:
			favoritos_main()

def generar_main(usuarios=['Todos']):
	'''Imprime un tweet generado pseudo-aleatoriamente utilizando como fuente 
	únicamente los tweets de un archivo con la lista de usuarios que recibe.
	De no recibir ningun usuario, se deberá generar un tweet usando como fuente
	todos los tweets disponibles.
	De recibir un usuario del cual no se tiene información el programa mostrará
	un mensaje de error por consola especificando dicho error.
	Luego de mostrar el tweet, se debe dar la opción de agregar este tweet a favoritos,
	guardándolo en un archivo en caso de que el usuario responda afirmativamente.'''
	usuarios_tweets=dar_usuarios_tweets(usuarios)
	palabras_recurrentes=dar_palabras_recurrentes(usuarios_tweets)
	actual_palabra=generar_primer_palabra(palabras_recurrentes)
	palabras_tweet_pseudoaleatorio=[actual_palabra,]
	while actual_palabra:
		actual_palabra=agregar_palabra_aleatoria(palabras_recurrentes,actual_palabra,palabras_tweet_pseudoaleatorio)
	tweet_pseudoaleatorio=' '.join(palabras_tweet_pseudoaleatorio)
	usuarios=', '.join(usuarios)
	mostrar_tweet_pseudoaleatorio(usuarios,tweet_pseudoaleatorio)
	agregar_tweet_a_favoritos(usuarios,tweet_pseudoaleatorio)
	
def dar_usuarios_tweets(usuarios):
	'''Va recorriendo por el archivo tweets.csv linea por linea para ir armando una
	lista con todos los tweets de los usuarios que esten en la lista que recibe. 
	Finalmente, devuelve esa lista.'''
	with open(ruta_tweets) as arch_tweets:
		usuarios_tweets=[]
		lineas_tweets=reader(arch_tweets,delimiter='\t')
		if usuarios==['Todos']:
			for linea in lineas_tweets:
				usuarios_tweets.append(linea[1])
			return usuarios_tweets	
		for linea in lineas_tweets:
			if linea[0] in usuarios:
				usuarios_tweets.append(linea[1])
		if usuarios_tweets==[]:
			raise NoExisteUsuario('Algún usuario es inexistente en el archivo tweets.csv.')
		return usuarios_tweets

def dar_palabras_recurrentes(usuarios_tweets):
	'''Recibe una lista de tweets, la recorre formando un diccionario conteniendo como claves 
	cada palabra que aparece en los tweets y como valor otro diccionario que tiene como claves 
	cada palabra que aparece despues de la palabra que esta como clave en el diccionario "padre"
	y como valor la cantidad de veces que aparece. Finalmente, devuelve ese diccionario.'''
	palabras_recurrentes={}
	for tweet in usuarios_tweets:
		palabras=tweet.split(' ')
		contador=0
		for palabra in palabras:
			palabra=palabra.lower()
			if contador==1:
				if palabra in palabras_recurrentes[palabra_ant]:	
					palabras_recurrentes[palabra_ant][palabra]+=1
				else:
					palabras_recurrentes[palabra_ant][palabra]=1
				contador=0
			if not(palabra in palabras_recurrentes):
				palabras_recurrentes[palabra]={}
			contador=1
			palabra_ant=palabra
	return palabras_recurrentes

def generar_primer_palabra(palabras_recurrentes):
	'''Recibe un diccionario con todas las palabras de los tweets 
	como clave y como valor otro diccionario con las palabras que le 
	siguen y un numero indicando su recurrencia.
	Devuelve cualquier palabra que este en el diccionario "padre".'''
	palabras_de_tweets=list(palabras_recurrentes.keys())
	primer_palabra_tweet_pseudo=random.choice(palabras_de_tweets)
	primer_palabra_tweet_pseudo=primer_palabra_tweet_pseudo[0].upper()+primer_palabra_tweet_pseudo[1:]
	return primer_palabra_tweet_pseudo
	
def agregar_palabra_aleatoria(palabras_recurrentes,actual_palabra,palabras_tweet_pseudoaleatorio):
	'''Recibiendo el diccionario de las palabras recurrentes, la ultima palabra de la
	lista de palabras que van a formar parte del tweet pseudo-aleatorio y la misma lista en si. 
	Agrega en esa lista cualquiera de las palabras que le siguen a la ultima palabra agregada 
	en la lista. La que tenga mas recurrencia va a ser la que tenga más probabilidades
	de ser elegida para el tweet pseudo-aleatorio. Finalmente, devuelve la palabra elegida.''' 
	palabras_siguientes=palabras_recurrentes[actual_palabra.lower()]
	candidatas=[]
	for palabra in palabras_siguientes:
		recurrencia=palabras_siguientes[palabra]
		candidatas+=[palabra,]*recurrencia
	if candidatas==[]:
		return ''
	palabra_aleatoria=random.choice(candidatas)
	if actual_palabra[-1]=='.' or actual_palabra[-1]==';':
		palabra_aleatoria=palabra_aleatoria[0].upper()+palabra_aleatoria[1:]
	actual_palabra=palabra_aleatoria
	palabras_tweet_pseudoaleatorio.append(actual_palabra)
	return actual_palabra

def mostrar_tweet_pseudoaleatorio(usuarios,tweet_pseudoaleatorio):
	'''Recibe los usuarios que se usaron de fuente y el tweet pseudo-aleatorio 
	ambos en forma de cadenas de texto y los imprime uno debajo del otro.'''
	print(f'Tweet generado por: {usuarios}.\n')
	print(f'{tweet_pseudoaleatorio}\n')

def agregar_tweet_a_favoritos(tweet_pseudoaleatorio,usuarios):
	'''Recibe los usuarios que se usaron de fuente y el tweet pseudo-aleatorio 
	ambos en forma de cadenas de texto.
	Pregunta al usuario si quiere agregar el tweet pseudo-aleatorio a favoritos.
	En caso de que responda afirmativamente, escribe o agrega en un archivo el tweet
	pseudo-aleotorio en una linea.'''
	valores_positivos=['y','se','s','yes','yeah','si','positivo','+']
	valores_negativos=['n','no','nel','negativo','-']
	while True:
		si_o_no=input('¿Desea agregar este tweet a favoritos?[s/n]: ')
		si_o_no=si_o_no.lower()
		if si_o_no in valores_negativos:
			return
		if si_o_no in valores_positivos:
			break
		print('Por favor ingrese un valor positivo o negativo sin espacios.')
	tweet_favorito=f'{usuarios}.\t{tweet_pseudoaleatorio}\n'
	with open(ruta_fav,'a') as arch_fav:
		arch_fav.write(tweet_favorito)
		
class NoExisteUsuario(Exception):
	pass

def trending_main(cant_hashtags):
	'''Recibe un número e imprime tos temas más comunes (palabras precedidas por un hashtag #) 
	de los que se habla en los tweets almacenados de un archivo.'''
	hashtags=dar_hashtags()
	trending=sacar_trending(hashtags,cant_hashtags)
	mostar_trending_top(trending)

def dar_hashtags():
	'''Lee los tweets almacenados del archivo y genera un diccionario con 
	los hashtags como llave y como valor un número representando su popularidad, 
	luego de hacer todo eso devuelve ese diccionario.'''
	hashtags={}
	valores_invalidos='.,;:-_[]+*="°{}¿?!¡()/\''
	with open(ruta_tweets) as arch_tweets:
		tweets_hashtag=reader(arch_tweets,delimiter='#')
		for hashtag_linea in tweets_hashtag:
			contador=0
			for i in hashtag_linea:
				if contador==1:	
					hashtag=i.split(' ')
					hashtag=hashtag[0]
					indice=0
					for x in hashtag:
						if x in valores_invalidos:
							hashtag=hashtag[:indice]
							break
						indice+=1
					if hashtag in hashtags:
						hashtags[hashtag]+=1
					else:
						hashtags[hashtag]=1
				contador=1
		return hashtags

def sacar_trending(hashtags,cant_hashtags):
	'''Recibe un número representando una cantidad de hashtags requeridas y el diccionario con todos 
	los hashtags, los ordena de mayor a menor número de popularidad y va agregando a una lista uno por uno 
	hasta satisfacer la cantidad de hashtags que se pide. Devuelve la lista que fue formando.'''
	trending=[]
	valores=list(hashtags.values())
	valores_ordenados=sorted(valores)
	valores_ordenados=valores_ordenados[::-1]
	for i in valores_ordenados:
		if len(trending)==int(cant_hashtags):
			break
		for hashtag in hashtags:
			hashtag_con_popularidad=(hashtag,i)
			if hashtags[hashtag]==i and not(hashtag_con_popularidad in trending):
				trending.append(hashtag_con_popularidad)
				break
	return trending

def mostar_trending_top(trending):
	'''Recibiendo la lista trending, va imprimiendo uno debajo del otro cada
	hashtag de la lista.'''
	indice=1
	for i in trending:
		print(f'{indice}. #{i[0]}: {i[1]}')
		indice+=1

def favoritos_main(cant_favs=-1):
	'''Imprime todos los tweets favoritos almacenados en un archivo. 
	La cantidad de favoritos que recibe es opcional,
	pero de ser ingresada, solo mostrará por pantalla dicha cantidad en orden
	cronológico descendente (de más recientes a menos recientes).'''
	tweets_favs=dar_tweets_favs()
	mostar_tweets_favs(tweets_favs,cant_favs)

def dar_tweets_favs():
	'''Lee el archivo favoritos.csv y va insertando en una lista las lineas en el orden 
	que quede los tweets más recientes a menos recientes.Finalmente, devuelve la lista.'''
	tweets_favs=[]
	try:	
		with open(ruta_fav) as arch_fav:
			for linea in arch_fav:
				tweet_fav=linea
				tweets_favs.insert(0,tweet_fav)
	except:
		print('No agrego nada a favoritos.')
	return tweets_favs

def mostar_tweets_favs(tweets_favs,cant_favs):
	'''Recibe la lista de que contiene los tweets favoritos y un número 
	representando la cantidad de tweets pedidos.
	Se encarga de imprimir cada tweet que contiene la lista, hasta satisfacer 
	la cantidad solicitada.'''
	contador=0
	for i in tweets_favs:
		if contador==int(cant_favs):
			break
		print(i)
		contador+=1

main()
