#include "testing.h"
#include "heap.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int comparar(const void* clave1, const void* clave2) {
    int val1 = *((int*)clave1);
    int val2 = *((int*)clave2);
    if (val1 == val2) return 0;
    if (val1 > val2 ) return 1;
    return -1;
}

void pruebas_heap_crear_array() {
	printf("\nPRUEBAS HEAP CREAR ARRAY\n");

	int tamanio = 6;

	// Creo un arreglo desordenado
	print_test("Creo un arreglo", true);
	int* numeros = malloc(sizeof(int)*tamanio);
	for (int i = 0; i < tamanio; i++) {
		numeros[i] = i;
	}
	void** arreglo = malloc(sizeof(void*)*tamanio);
	for (int j = 0; j < tamanio; j++) {
		arreglo[j] = &numeros[j];
	}

	// Heapify
	print_test("Heap crear array", true);
	heap_t* heap = heap_crear_arr(arreglo, tamanio, comparar);

	free(arreglo);

	print_test("Heap cantidad es la correcta", heap_cantidad(heap) == tamanio);
	print_test("Desencolar devuelve el maximo", *((int*)heap_desencolar(heap)) == 5);
	print_test("Se actualizo el maximo", *((int*)heap_ver_max(heap)) == 4);
	print_test("Heap cantidad es la correcta", heap_cantidad(heap) == tamanio-1);

	heap_destruir(heap, NULL);
	free(numeros);
	// free(arreglo);
}

void pruebas_heap_vacio() {
	printf("\nPRUEBAS HEAP VACIO\n");

	heap_t* heap = heap_crear(comparar);
	print_test("EL heap se creo", heap);

	print_test("El heap esta vacio", heap_esta_vacio(heap));
	print_test("Heap cantidad es 0", !heap_cantidad(heap));
	print_test("Ver max devuelve NULL", !heap_ver_max(heap));
	print_test("Desencolar devuelve NULL", !heap_desencolar(heap));

	heap_destruir(heap, NULL);
}

void pruebas_varios_elementos() {
	printf("\nPRUEBAS VARIOS ELEMENTOS\n");
	heap_t* heap = heap_crear(comparar);

	int var1 = 10;
	int var2 = 2;
	int var3 = 7;

	print_test("Encolar NULL devuelve false", !heap_encolar(heap, NULL));

	//Encolo los 3 elementos
	print_test("Encolo la var1", heap_encolar(heap, &var1));
	print_test("Heap cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Ver max devuelve el max", heap_ver_max(heap) == &var1);

	print_test("Encolo la var2", heap_encolar(heap, &var2));
	print_test("Heap cantidad es 2", heap_cantidad(heap) == 2);	
	print_test("Ver max devuelve el max", heap_ver_max(heap) == &var1);

	print_test("Encolo la var3", heap_encolar(heap, &var3));
	print_test("Heap cantidad es 3", heap_cantidad(heap) == 3);
	print_test("Ver max devuelve el max", heap_ver_max(heap) == &var1);

	// Desencolo los 3 elementos
	print_test("Desencolar devuelve el max", heap_desencolar(heap) == &var1);
	print_test("Desencolar devuelve el max", heap_desencolar(heap) == &var3);
	print_test("Desencolar devuelve el max", heap_desencolar(heap) == &var2);

	print_test("El heap esta vacio", heap_esta_vacio(heap));
	print_test("Heap cantidad es 0", !heap_cantidad(heap));

	heap_destruir(heap, NULL);
}

void pruebas_de_volumen(size_t largo) {
	printf("\nPRUEBAS DE VOLUMEN\n");

	heap_t* heap = heap_crear(comparar);

    const size_t largo_valor = 20;
    int* valores = malloc(largo * largo_valor);

    // Encolo los elementos 
    bool ok = true;
    for (int j = 0; j < largo; j++) {
    	valores[j] = rand();
    	ok = heap_encolar(heap, &valores[j]);
    	if (!ok) break;
    }

    print_test("Prueba heap almacenar muchos elementos", ok);
    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    // Verifico que se desencolen de mayor a menor
    void* ultimo = NULL;
    for (unsigned i = 0; i < largo; i++) {
    	void* actual = heap_desencolar(heap);
    	if (!ultimo) {
    		ultimo = actual;
    		continue;
    	}
    	if (*((int*)ultimo) < *((int*)actual)) {
    		ok = false;
    		break;
    	}
    }	

    print_test("Los elementos se desencolaron en orden correcto", ok);
    print_test("Heap cantidad es 0", !heap_cantidad(heap));

    heap_destruir(heap, NULL);
    free(valores);

}

void pruebas_destruir() {
	printf("\nPRUEBAS DESTRUIR CON FREE\n");

	int* valor1 = malloc(sizeof(int)); *valor1 = 1;
	int* valor2 = malloc(sizeof(int)); *valor2 = 5;
	int* valor3 = malloc(sizeof(int)); *valor3 = 10;
	int* valor4 = malloc(sizeof(int)); *valor4 = 4;

	heap_t* heap = heap_crear(comparar);

	print_test("Encolo valor1", heap_encolar(heap, valor1));
	print_test("Encolo valor2", heap_encolar(heap, valor2));
	print_test("Encolo valor3", heap_encolar(heap, valor3));
	print_test("Encolo valor4", heap_encolar(heap, valor4));
	print_test("Heap cantidad es 4", heap_cantidad(heap) == 4);

	print_test("Desencolar devuelve el max", heap_desencolar(heap) == valor3);

	print_test("Destruyo el heap con free", true);

	heap_destruir(heap, free);
	free(valor3);
}

void pruebas_heapsort() {
	printf("\nPRUEBAS HEAPSORT\n");


	size_t largo = 200;

	// Creo un arreglo de numeros desordenado
	print_test("Creo un arreglo desordenado", true);

    const size_t largo_valor = 20;
    int* valores = malloc(largo * largo_valor);
    for (int j = 0; j < largo; j++) {
    	valores[j] = rand();
    }

    // Creo un arreglo de punteros a partir del arreglo anterior
	void** arreglo = malloc(sizeof(void*)*largo);
	for (int k = 0; k < largo; k++) {
		arreglo[k] = &valores[k];
	}

	heap_sort(arreglo, largo, comparar);
	print_test("Le aplico heap_sort", true);


	bool ok = true;
	void* ultimo = NULL;
	for (int i = 0; i < largo; i++) {
		if (!ultimo) ultimo = arreglo[i];
		else {
			if (*((int*)ultimo) > *((int*)arreglo[i])) {
				ok = false;
				break;
			}
			ultimo = arreglo[i];
		}
	}

	print_test("El arreglo esta ordenado", ok);

	free(valores);
	free(arreglo);

}

void pruebas_heap_alumno() {
	pruebas_heap_vacio();
	pruebas_varios_elementos();
	pruebas_destruir();
	pruebas_heap_crear_array();
	pruebas_heapsort();
	pruebas_de_volumen(5000);
}