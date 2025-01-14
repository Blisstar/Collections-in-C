#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define FACTOR_DE_CARGA 2
#define CAPACIDAD_INICIAL 31
#define REDIMENSION_ARRIBA 2
#define REDIMENSION_ABAJO 2


size_t DJBHash(const char* str, size_t len)
{
   size_t hash = 5381;
   size_t i    = 0;

   for(i = 0; i < len; str++, i++)
   {   
      hash = ((hash << 5) + hash) + (size_t)(*str);
   }   

   return hash;
}

struct hash {
	hash_destruir_dato_t destruir_dato;
    lista_t** tabla;
    size_t cantidad;
    size_t capacidad;
};

typedef struct hash_campo {
    char* clave;
    void* valor;  
}hash_campo_t;

struct hash_iter {
	size_t pos;
	const hash_t* hash;
	hash_campo_t* actual;
	lista_iter_t* lista_iter;
	size_t recorridos;
};


bool duplicar_tabla(hash_t* hash, lista_t** nueva_tabla, size_t nueva_capacidad);
bool redimensionar(hash_t* hash, size_t redimension); 
hash_campo_t* buscar_campo_en_lista(lista_t* lista, const char* clave);

/* ******************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t)); 
	if (!hash) return NULL;
	hash->tabla = calloc(CAPACIDAD_INICIAL, sizeof(lista_t*));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}
	hash->destruir_dato = destruir_dato;
	hash->cantidad = 0;
	hash->capacidad = CAPACIDAD_INICIAL;
	return hash;
}

hash_campo_t* hash_crear_campo(const char* clave, void* dato, size_t pos) {
	hash_campo_t* campo = malloc(sizeof(hash_campo_t));
	if (!campo) return NULL;

	campo->clave = strdup(clave);
	if (!campo->clave) {
		free(campo);
		return NULL;
	}
	campo->valor = dato;
	return campo;
}

bool tabla_guardar(hash_t* hash, lista_t** tabla, size_t capacidad, hash_destruir_dato_t destruir_dato, const char* clave, void* dato) {	
	/* Guarda (clave, valor) en la tabla recibida, reemplazando el valor si la clave ya existía */

	size_t pos = DJBHash(clave, (size_t)strlen(clave)) % capacidad;

	// Busco la clave en la el balde correspondiente
	hash_campo_t* campo = buscar_campo_en_lista(tabla[pos], clave);

	// Si ya existe un campo con esa clave reemplazo el valor
	if (campo) {
		if (destruir_dato) destruir_dato(campo->valor);
		campo->valor = dato;
	}
	else {
		hash_campo_t* campo_nuevo = hash_crear_campo(clave, dato, pos);
		if (!campo_nuevo) return false;
		if (!tabla[pos]) {
			tabla[pos] = lista_crear();
			if (!tabla[pos]) return false;
		}
		if (hash) hash->cantidad++; // hash == NULL --> estoy duplicando la tabla y no hace falta aumentar la cantidad
		lista_insertar_ultimo(tabla[pos], campo_nuevo);
	}
	return true;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
	// Redimensiono si es necesario
	if (hash->cantidad / hash->capacidad >= FACTOR_DE_CARGA) {
		if (!redimensionar(hash, hash->capacidad * REDIMENSION_ARRIBA)) return false;
	}
	// Guardo (clave, valor) en la tabla de hash
	if (!tabla_guardar(hash, hash->tabla, hash->capacidad, hash->destruir_dato, clave, dato)) return false;
	return true;
}

void* hash_borrar(hash_t* hash, const char* clave) {
	size_t pos = DJBHash(clave, (size_t)strlen(clave)) % hash->capacidad;
	if (!hash_pertenece(hash, clave)) return NULL;
	void* dato = NULL;

	lista_t* lista = hash->tabla[pos];
	lista_iter_t* iter = lista_iter_crear(lista);

	// Itero la lista hasta encontrar la clave correspondiente
	while (!lista_iter_al_final(iter)) {
		hash_campo_t* actual = lista_iter_ver_actual(iter);
		if (!strcmp(actual->clave, clave)) {
			dato = actual->valor;
			free(actual->clave);
			free(actual);
			hash->cantidad--;
			lista_iter_borrar(iter);
			break;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);

	if (hash->cantidad <= (hash->capacidad / 4) && hash->capacidad > CAPACIDAD_INICIAL) {
		redimensionar(hash, hash->capacidad / REDIMENSION_ABAJO);
	}

	return dato;		
}

void* hash_obtener(const hash_t* hash, const char* clave) {
	size_t pos = DJBHash(clave, (size_t)strlen(clave)) % hash->capacidad;
	if (!hash->tabla[pos]) 
		return NULL;

	// Busco el campo que contenga la clave
	hash_campo_t* campo = buscar_campo_en_lista(hash->tabla[pos], clave);
	if (!campo) return NULL;
	return campo->valor;
}

hash_campo_t* buscar_campo_en_lista(lista_t* lista, const char* clave) {
	/* Recibe la lista en la que se encontraría la clave
	 * Recorre la lista y devuelve el campo de la clave, devolviendo NULL si no se encuentra
	 */

	if (!lista) return NULL;
	hash_campo_t* campo = NULL;
	lista_iter_t* iter = lista_iter_crear(lista);
	if (!iter) return NULL;

	while (!lista_iter_al_final(iter)) {
		hash_campo_t* campo_actual = lista_iter_ver_actual(iter);
		if (!strcmp(campo_actual->clave, clave) ) {
			campo = campo_actual;
			break;
		}
		lista_iter_avanzar(iter);
	}

	lista_iter_destruir(iter);
	return campo;

}

size_t hash_cantidad(const hash_t* hash) {
	return hash->cantidad;
}

void destruir_tabla(hash_t* hash, bool borrar_dato) {
	/* Destruye la tabla
	 * Si !borrar_dato --> estoy redimensionando la tabla y no hay que borrar el puntero al dato
	 */

	lista_t** tabla = hash->tabla;

	// Recorro la tabla destruyendo las listas
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (!tabla[i]) continue;
		lista_t* lista = hash->tabla[i];

		// Recorro la lista destruyendo los campos
		while (!lista_esta_vacia(lista)) {
			hash_campo_t* campo = lista_borrar_primero(lista);
			hash_destruir_dato_t destruir_dato = hash->destruir_dato;
			if (destruir_dato && borrar_dato) destruir_dato(campo->valor);
			free(campo->clave);
			free(campo);
		}
		free(lista);
	}
	free(hash->tabla);
}


void hash_destruir(hash_t* hash) {
	destruir_tabla(hash, true);
	free(hash);
}	

bool hash_pertenece(const hash_t* hash, const char* clave) {
	size_t pos = DJBHash(clave, (size_t)strlen(clave)) % hash->capacidad;

	// Busco el campo de la clave en el balde correspondiente
	hash_campo_t* campo_clave = buscar_campo_en_lista(hash->tabla[pos], clave);
	
	// Si campo_clave es NULL no se pudo encontrar la clave en la tabla
	return (campo_clave) ? true : false;
}


bool redimensionar(hash_t* hash, size_t redimension) {
	size_t nueva_capacidad = redimension;

	lista_t** nueva_tabla = calloc(nueva_capacidad, sizeof(lista_t*));
	if (!nueva_tabla) return false;

	duplicar_tabla(hash, nueva_tabla, nueva_capacidad);
	
	destruir_tabla(hash, false); // false para que no destruya los punteros a los datos

	hash->tabla = nueva_tabla;
	hash->capacidad = nueva_capacidad;

	return true;
}


bool duplicar_tabla(hash_t* hash, lista_t** nueva_tabla, size_t nueva_capacidad) {

	// Recorro la tabla 
	for (int i = 0; i < hash->capacidad; i++) {
		if (!hash->tabla[i] || lista_esta_vacia(hash->tabla[i])) continue;
		lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
		
		// Recorro la lista guardando cada campo en la nueva tabla
		while (!lista_iter_al_final(iter)) {
			hash_campo_t* campo = lista_iter_ver_actual(iter);
			tabla_guardar(NULL, nueva_tabla, nueva_capacidad, NULL, campo->clave, campo->valor);
			lista_iter_avanzar(iter);
		}
		lista_iter_destruir(iter);
	}
	return true;
}

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/


lista_t* buscar_lista_no_vacia(const hash_t* hash, hash_iter_t* iter) {
	/* Recorre la tabla desde la posición dada y devuelve la primera lista no vacía
	 * Devuelve NULL si no se encuentra ninguna
	 */
	lista_t* lista = NULL;

	while(iter->pos < hash->capacidad) {

		lista_t* lista_actual = hash->tabla[iter->pos];
		if (lista_actual) {
			lista = lista_actual;
			break;
		}
		iter->pos++;
	}
	return lista;
}

hash_iter_t* hash_iter_crear(const hash_t* hash) {
	hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
	if (!hash_iter) return NULL;

	hash_iter->hash = hash;

	hash_iter->pos = 0;

	// Busco la primera lista no vacía

	lista_t* lista = buscar_lista_no_vacia(hash, hash_iter);

	hash_iter->actual = (!lista) ? NULL : lista_ver_primero(lista);
	hash_iter->lista_iter = (!lista) ? NULL : lista_iter_crear(lista);
	hash_iter->recorridos = 0;

	return hash_iter;
}

bool hash_iter_avanzar(hash_iter_t* hash_iter) {
	if (hash_iter_al_final(hash_iter)) return false;

	hash_iter->recorridos++;

	if (hash_iter_al_final(hash_iter)) return true;

	lista_iter_t* lista_iter = hash_iter->lista_iter;

	lista_iter_avanzar(lista_iter);

	if (lista_iter_al_final(lista_iter)) {
		hash_iter->pos++;
		lista_t* nueva_lista = buscar_lista_no_vacia(hash_iter->hash, hash_iter);
		lista_iter_destruir(hash_iter->lista_iter);
		hash_iter->lista_iter = lista_iter_crear(nueva_lista);	 
	}

	hash_iter->actual = lista_iter_ver_actual(hash_iter->lista_iter); 


	return true;

}

const char* hash_iter_ver_actual(const hash_iter_t* hash_iter) {
	if (hash_iter->recorridos == hash_iter->hash->cantidad) return NULL;
	return (hash_iter->actual->clave);
}

bool hash_iter_al_final(const hash_iter_t* hash_iter) {
	return hash_iter->recorridos == hash_iter->hash->cantidad;
}

void hash_iter_destruir(hash_iter_t* iter) {
	if (iter->lista_iter) lista_iter_destruir(iter->lista_iter);
	free(iter);
}

