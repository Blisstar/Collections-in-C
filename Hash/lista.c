#include "lista.h"
#include <stdlib.h>

struct lista {
	size_t largo;
	nodo_t* primero;
	nodo_t* ultimo;
};

struct lista_iter {
	lista_t* lista;
	nodo_t* anterior;
	nodo_t* actual;
};

nodo_t* crear_nodo(void* dato) {
	nodo_t* elemento = malloc(sizeof(nodo_t));
	if (!elemento) return NULL;
	elemento->dato = dato;
	elemento->prox = NULL;
	return elemento;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/


lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (!lista) return NULL;
	lista->largo = 0;
	lista->primero = NULL;
	lista->ultimo = NULL;
	return lista;
}


bool lista_esta_vacia(const lista_t* lista) {
	return (!lista->primero);
}

bool lista_insertar_primero(lista_t* lista, void* dato) {
	nodo_t* elemento = crear_nodo(dato);
	if (!elemento) return false;
	lista->largo++;
	if (!lista->primero) {
		lista->ultimo = elemento;
	}
	else {
		nodo_t* primero = lista->primero;
		elemento->prox = primero;
	}
	lista->primero = elemento;
	return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato) {
	if (!lista->primero) return lista_insertar_primero(lista, dato);
	nodo_t* elemento = crear_nodo(dato);
	if (!elemento) return false;
	lista->largo++;
	lista->ultimo->prox = elemento;
	lista->ultimo = elemento;
	return true;
}

void* lista_borrar_primero(lista_t* lista) {
	nodo_t* primero = lista->primero;
	if (!primero) return NULL;
	lista->largo--;
	void* dato = primero->dato;
	lista->primero = primero->prox;
	if (!lista->primero) lista->ultimo = NULL;
	free(primero);
	return dato;
}

void* lista_ver_primero(const lista_t* lista) {
	return (!lista->primero) ? NULL : lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t* lista) {
	return (!lista->ultimo) ? NULL : lista->ultimo->dato;
}

size_t lista_largo(const lista_t* lista) {
	return lista->largo;
}

void lista_destruir(lista_t* lista, void destruir_dato(void*)) {
	while (!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato != NULL) destruir_dato(dato);
	}
	free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t* actual = lista->primero;
	while (actual) {
		void* dato = actual->dato;
		if (!visitar(dato, extra)) return;
		actual = actual->prox;
	}
}


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

lista_iter_t* lista_iter_crear(lista_t* lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter) return NULL;
	iter->lista = lista;
	iter->anterior = NULL;
	iter->actual = lista->primero;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter) {
	if (!iter->actual) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter) {
	if (!iter->actual) return NULL;
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter) {
	return (!iter->actual);
}

void lista_iter_destruir(lista_iter_t* iter) {
	free(iter);
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato) {
	if (!iter->anterior) {
		if (!lista_insertar_primero(iter->lista, dato)) return false;
		iter->actual = iter->lista->primero;
	}
	else if (!iter->actual) {
		if (!lista_insertar_ultimo(iter->lista, dato)) return false;
		iter->actual = iter->lista->ultimo;
	}
	else {
		nodo_t* elemento = crear_nodo(dato);
		if (!elemento) return false;
		iter->lista->largo++;
		elemento->prox = iter->actual;
		iter->anterior->prox = elemento;
		iter->actual = elemento;
	}
	return true;	
}

void* lista_iter_borrar(lista_iter_t* iter) {
	if (!iter->actual) return NULL;
	if (!iter->anterior) {
		iter->actual = iter->actual->prox; 
		return lista_borrar_primero(iter->lista);
	}
	iter->lista->largo--;
	nodo_t* act = iter->actual;
	if (!act->prox) iter->lista->ultimo = iter->anterior;
	iter->anterior->prox = iter->actual->prox;
	void* dato = act->dato;
	iter->actual = iter->actual->prox;
	free(act);
	return dato;
}
