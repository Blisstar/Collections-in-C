#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

 
/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
typedef struct nodo {
	void* dato;
	struct nodo* prox;
}nodo_t;

//Lista que contiene punteros genéricos.
typedef struct lista lista_t;

//Permite tener acceso a los elementos de una lista sin saber su implementación.
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista
// Post: devuelve una nueva lista vacía.
lista_t* lista_crear(void);

// Devuelve erdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo al elemento a la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo al elemento a la lista. Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primero elemento de la lista, devolviendo su valor si éste 
// existe, si la lista está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento, la lista 
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primero elemento de la lista y lo 
// devuelve si ésta no está vacía, devuelve NULL de lo contrario.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primero elemento si no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista y lo 
// devuelve si ésta no está vacía, devuelve NULL de lo contrario.
// Pre: la lista fue creada.
// Post: se devolvió el valor del último elemento si no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir por parámetro se le aplica
// a cada elemento de la lista.
// Pre: la lista fue creada. Destruir_dato es una función capaz de destruir los elementos
// de la lista, o NULL en caso de que no se utilice.
// Post: Se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


// Itera la lista aplicando la función visitar a cada uno de los los elementos de la lista
// hasta que ésta devuelva false o el iterador se encuentre al final de la lista.
// Pre: la lista fue creada.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea el iterador.
// Post: devuelve un iterador en la primera posición de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al siguiente nodo.
// Pre: el iterador fue creado.
// Post: se actualiza el actual y el anterior.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del dato actual, devolviendo NULL si éste no existe.
// Pre: el iterador fue creado.
// Post: devuelve el valor del dato actual.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si se encuentra en la posición final de la lista.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un nuevo elemento en la posición actual del iterador.
// Pre: la lista fue creada. El iterador fue creado.
// Post: la lista tiene un nuevo elemento.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento en la posición actual del iterador y devuelve su valor, si éste
// no existe devuelve NULL.
// Pre: la lista fue creada. El iterador fue creado.
// Post: se devuelve el valor del elemento actual. La lista tiene un elemento menos si
// éste existía.
void *lista_iter_borrar(lista_iter_t *iter);


#endif // LISTA_H