#include "pila.h"
#include <stdlib.h>
#include <stdbool.h>
#define CAPACIDAD_INICIAL 20
#define REDIMENSION_ARRIBA pila->capacidad * 2
#define REDIMENSION_ABAJO  pila->capacidad / 2

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
bool pila_redimensionar(pila_t* pila, bool redimension);

pila_t* pila_crear(void) {
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;
	pila->datos = malloc(CAPACIDAD_INICIAL* sizeof(void*));
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return (!pila->cantidad);
}

bool pila_apilar(pila_t *pila, void* valor) {
	if (pila->cantidad == pila->capacidad) {
		if (!pila_redimensionar(pila, 1)) return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true;
}

void* pila_ver_tope(const pila_t *pila) {
	return (!pila->cantidad) ? NULL : pila->datos[pila->cantidad-1];

}

void* pila_desapilar(pila_t *pila) {
	if (!pila->cantidad) return NULL;
	void* tope = pila->datos[pila->cantidad-1];
	if (pila->cantidad <= (pila->capacidad / 4) && pila->capacidad > CAPACIDAD_INICIAL) {
		pila_redimensionar(pila, 0);
	} 
	pila->cantidad -= 1;
	return tope;
}

bool pila_redimensionar(pila_t* pila, bool redimension) {    
	size_t nueva_capacidad;                              
	void** datos_nuevos;
	if (redimension) {
		nueva_capacidad = REDIMENSION_ARRIBA;
	}
	else {
		nueva_capacidad = REDIMENSION_ABAJO;
	}
	datos_nuevos = realloc(pila->datos, nueva_capacidad * sizeof(void*));
	if (datos_nuevos == NULL) {
		return false;
	}
	pila->capacidad = nueva_capacidad;
	pila->datos = datos_nuevos;
	return true;	 		
}