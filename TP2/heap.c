#include "heap.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CAP_INICIAL 10

/* ******************************************************************
 *                DECLARACIÓN DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct heap {
	void** arreglo;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};

/* ******************************************************************
 *                      FUNCIONES ADICIONALES
 * *****************************************************************/

/*Cambia el tamaño del heap
 *Pre: el heap fue creado
 *Post: el heap cambió de tamaño a nuevo_tam y devuelve true
 *o el heap queda intacto y devuelve false si no se pudo cambiar el tamaño
 *a nuevo_tam
 *Proveído de un TP de Pila hecho por un integrante del grupo.
 */
bool heap_redimensionar(heap_t* heap,size_t nuevo_tam){
    void* datos_aux = realloc(heap->arreglo,nuevo_tam*sizeof(void*));
    if (nuevo_tam < heap->cantidad || datos_aux == NULL){
        return false;
    }

    heap->arreglo = datos_aux;
    heap->capacidad = nuevo_tam;
    return true;
}

/* Intercambia dos punteros genéricos.
 */
void swap(void **pa, void **pb) {
    void *pc; 

    pc  = *pa;
    *pa = *pb;
    *pb = pc;
}

/* Mientas que el elemento sea mayor a su padre, se van swapeando
 * hasta alcanzar un heap de máximos.
 */
void upheap(void** arreglo, size_t pos, cmp_func_t cmp) {
	if (pos == 0) return;

	size_t padre = (pos-1) / 2;

	if (cmp(arreglo[padre], arreglo[pos]) < 0) {
		swap(&arreglo[padre], &arreglo[pos]);
		upheap(arreglo, padre, cmp);
	}
}

/* Mientas que el elemento sea menor que su hijo menor, se van swapeando
 * hasta alcanzar un heap de máximos.
 */
void downheap(void** A, size_t tam, size_t pos, cmp_func_t cmp) {
	if (pos >= tam) return;

	size_t min = pos; // PADRE
	size_t izq = 2 * pos + 1;
	size_t der = 2 * pos + 2;

	if (izq < tam && cmp(A[izq], A[min]) > 0) {
		min = izq;
	}
	if (der < tam && cmp(A[der], A[min]) > 0) {
		min = der;
	}
	if (min != pos) {
		swap(&A[pos], &A[min]);
		downheap(A,   tam,   min,   cmp);
	}
}

/* Recibe un arreglo y lo devuelve con forma de heap.
 */
void heapify(void* arreglo[], size_t tam, cmp_func_t cmp) {
	for (int i = (int)tam/2; i >= 0; i--) {
		downheap(arreglo, tam, i, cmp);
	}
}

/* ******************************************************************
 *                       PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	heap->arreglo = malloc(sizeof(void*) * CAP_INICIAL);
	if (!heap->arreglo) {
		free(heap);
		return NULL;
	}

	heap->cantidad = 0;
	heap->capacidad = CAP_INICIAL;
	heap->cmp = cmp;

	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	void** arreglo_heap = malloc(sizeof(void*)*n);

	for (size_t i = 0; i < n; i++){
		arreglo_heap[i] = arreglo[i];
	}

	heapify(arreglo_heap, n, cmp);

	heap->arreglo = arreglo_heap;
	heap->cantidad = n;
	heap->capacidad = n;
	heap->cmp = cmp;
	
	return heap;
}


void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
	if (destruir_elemento) {
		for (int i = 0; i < heap->cantidad; i++) {
			destruir_elemento(heap->arreglo[i]);
		}
	}

	free(heap->arreglo);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
	return (!heap->cantidad);
}

bool heap_encolar(heap_t *heap, void *elem){
	if (!elem) return false;

	if (heap->cantidad == heap->capacidad){
		bool comprobacion = heap_redimensionar(heap, heap->capacidad * 2);
		if (!comprobacion) return false;

	}

	heap->arreglo[heap->cantidad] = elem;
	upheap(heap->arreglo, heap->cantidad, heap->cmp);
	heap->cantidad++;

	return true;
}

void *heap_ver_max(const heap_t *heap) {
	if (!heap->cantidad) return NULL;

	return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap) {
	if (!heap->cantidad) return NULL;

	void* elemento = heap->arreglo[0];

	heap->cantidad--;

	swap(&heap->arreglo[0], &heap->arreglo[heap->cantidad]);
	downheap(heap->arreglo, heap->cantidad, 0, heap->cmp);

	if (heap->cantidad <= (heap->capacidad / 4) && heap->capacidad > CAP_INICIAL) {
		heap_redimensionar(heap, heap->capacidad / 2);
	}

	return elemento;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	heapify(elementos, cant, cmp);

	int cantidad = (int)cant;

	for (int i = cantidad-1; i >= 0; i--) {
		swap(&elementos[0], &elementos[i]);
		downheap(elementos, i, 0, cmp);
	}
}

