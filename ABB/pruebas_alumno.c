#include "abb.h"
#include "testing.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

bool imprimir(const char* clave, void* dato, void* extra) {
    int* contador = extra;
    if (*contador >= 10) return false;
    ++(*contador);
    printf("La clave es: %s\n", clave);
    return true;
}

int comparar(const char* clave1, const char* clave2) {
    int val1 = atoi(clave1);
    int val2 = atoi(clave2);
    if (val1 == val2) return 0;
    if (val1 > val2 ) return 1;
    return -1;
}


void pruebas_abb_vacio() {
	printf("\nPRUEBAS ABB VACIO\n");

    abb_t* abb = abb_crear(comparar, NULL);

    print_test("Se creo el abb", true);

    print_test("El abb esta vacio", !abb_cantidad(abb));
    print_test("Pertenece devuelve NULL", !abb_pertenece(abb, "hola"));
    print_test("Obtener clave devuelve NULL", !abb_obtener(abb, "hola"));
    print_test("Borrar devuelve NULL", !abb_borrar(abb, "hola"));

    abb_destruir(abb);
}

void pruebas_guardar() {
	printf("\nPRUEBAS GUARDAR\n");

	abb_t* abb = abb_crear(comparar, NULL);

	print_test("Se creo el abb", true);

	print_test("La cantidad de elementos es 0", !abb_cantidad(abb));

	const char* clave1 = "3";
	int valor1;

	print_test("Se guardo la clave1", abb_guardar(abb, clave1, &valor1));
	print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("La clave1 pertenece", abb_pertenece(abb, clave1));
	print_test("Obtener clave1 es valor1", abb_obtener(abb, clave1) == &valor1);

	const char* clave2 = "6";
	int valor2;

	print_test("Se guardo la clave2", abb_guardar(abb, clave2, &valor2));
	print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);
	print_test("La clave2 pertenece", abb_pertenece(abb, clave2));
	print_test("Obtener clave2 es valor2", abb_obtener(abb, clave2) == &valor2);

	const char* clave3 = "-1";
	int valor3;
	print_test("Se guardo la clave3", abb_guardar(abb, clave3, &valor3));
	print_test("La cantidad de elementos es 3", abb_cantidad(abb) == 3);
	print_test("La clave3 pertenece", abb_pertenece(abb, clave3));
	print_test("Obtener clave3 es valor3", abb_obtener(abb, clave3) == &valor3);

	abb_destruir(abb);
}

void pruebas_clave_vacia() {
	printf("\nPRUEBAS CLAVE VACIA\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave1 = "";
	int valor1;

	print_test("Se guardo la clave vacia", abb_guardar(abb, clave1, &valor1));
	print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("La clave pertenece", abb_pertenece(abb, clave1));
	print_test("Obtener clave vacia devuelve el valor", abb_obtener(abb, clave1));
	print_test("Se puede borrar la clave vacia", abb_borrar(abb, clave1));

	abb_destruir(abb);

}

void pruebas_valor_null() {
	printf("\nPRUEBAS VALOR NULL\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave = "1";
	void* valor = NULL;

	print_test("Se guardo el valor NULL", abb_guardar(abb, clave, valor));
	print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("La clave pertenece", abb_pertenece(abb, clave));
	print_test("Obtener clave devuelve NULL", abb_pertenece(abb, clave));
	print_test("Se puede borrar el elemento", !abb_borrar(abb, clave));
	print_test("La cantidad de elementos es 0", abb_cantidad(abb) == 0);

	abb_destruir(abb);
}

void pruebas_reemplazar() {
	printf("\nPRUEBAS REEMPLAZAR VALOR\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave1 = "1";

	int valor1;
	int valor2;

	print_test("Guardar clave1", abb_guardar(abb, clave1, &valor1));
	print_test("Obtener clave1 es valor1", abb_obtener(abb, clave1) == &valor1);
	print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);

	print_test("Guardar clave1 con otro valor", abb_guardar(abb, clave1, &valor2));
	print_test("Obtener clave1 es valor2", abb_obtener(abb, clave1) == &valor2);
	print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);

	abb_destruir(abb);
}

void pruebas_reemplazar_con_destruir() {
	printf("\nPRUEBAS REEMPLAZAR CON DESTRUIR\n");
	abb_t* abb = abb_crear(comparar, free);

	const char* clave1 = "1";
	const char* clave2 = "2";

	void* valor1a = malloc(sizeof(int));
	void* valor1b = malloc(sizeof(int));
	void* valor2a = malloc(sizeof(int));
	void* valor2b = malloc(sizeof(int));

	// Inserto las dos claves
	print_test("Guardar clave1", abb_guardar(abb, clave1, valor1a));
	print_test("Obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Guardar clave2", abb_guardar(abb, clave2, valor2a));
	print_test("Obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

	// Reemplazo los valores
	print_test("Guardar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
	print_test("Obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Guardar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
	print_test("Obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

	abb_destruir(abb);
}

void pruebas_borrar_con_dos_hijos() {
	printf("\nPRUEBAS BORRAR NODO CON 2 HIJOS\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave1 = "0";
	const char* clave2 = "1";
	const char* clave3 = "-1";
	const char* clave4 = "2";

	int valor1;
	int valor2;
	int valor3;
	int valor4;

	// Inserto 3 claves
	print_test("Guardar clave1", abb_guardar(abb, clave1, &valor1));
	print_test("Guardar clave2", abb_guardar(abb, clave2, &valor2));
	print_test("Guardar clave3", abb_guardar(abb, clave3, &valor3));
	print_test("La cantidad de elementos es 3", abb_cantidad(abb) == 3);

	// Borro el nodo con dos hijos
	print_test("Borrrar clave1 (raíz con dos hijos)", abb_borrar(abb, clave1) == &valor1);
	print_test("Clave1 no pertenece", !abb_pertenece(abb, clave1));
	print_test("Obtener clave1 es NULL", !abb_obtener(abb, clave1));
	print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

	// Verifico que los otros elementos no hayan cambiado
	print_test("Obtener clave2 es valor2", abb_obtener(abb, clave2) == &valor2);
	print_test("Obtener clave3 es valor3", abb_obtener(abb, clave3) == &valor3);

	// Pruebo insertar un elemento luego de borrar
	print_test("Guardar clave4", abb_guardar(abb, clave4, &valor4));
	print_test("Obtener clave4 es valor4", abb_obtener(abb, clave4) == &valor4);
	print_test("La cantidad de elementos es 3", abb_cantidad(abb) == 3);

	abb_destruir(abb);
}

void pruebas_borrar_hoja() {
    printf("\nPRUEBAS BORRAR HOJA\n");
    abb_t* abb = abb_crear(comparar, NULL);

    const char* clave1 = "0";
    const char* clave2 = "1";
    const char* clave3 = "2";

    int valor1;
    int valor2;
    int valor3;

    // Inserto una clave
    print_test("Guardar clave1", abb_guardar(abb, clave1, &valor1));
    print_test("Guardar clave2", abb_guardar(abb, clave2, &valor2));
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    // Borro una hoja
    print_test("Borrar clave2 (hoja)", abb_borrar(abb, clave2) == &valor2);
    print_test("Clave2 no pertenece", !abb_pertenece(abb, clave2));
    print_test("Obtener clave2 es NULL", !abb_obtener(abb, clave2));
    print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);

    // Verifico que no se haya cambiado la otra clave
    print_test("Clave1 pertenece", abb_pertenece(abb, clave1));
    print_test("Obtener clave1 es valor1", abb_obtener(abb, clave1) == &valor1);

    // Pruebo insertar un elemento luego de borrar
    print_test("Guardar clave3", abb_guardar(abb, clave3, &valor3));
    print_test("Obtener clave3 es valor3", abb_obtener(abb, clave3) == &valor3);
    print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}
void pruebas_borrar_con_un_hijo() {
	printf("\nPRUEBAS BORRAR NODO CON 1 HIJO\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave1 = "0";
	const char* clave2 = "1";
	const char* clave3 = "2";

	int valor1;
	int valor2;
	int valor3;

	// Inserto dos claves
	print_test("Guardar clave1", abb_guardar(abb, clave1, &valor1));
	print_test("Guardar clave2", abb_guardar(abb, clave2, &valor2));
	print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

	// Borro el nodo con un hijo
	print_test("Borrar clave1 (raíz con un hijo)", abb_borrar(abb, clave1) == &valor1);
	print_test("Clave1 no pertenece", !abb_pertenece(abb, clave1));
	print_test("Obtener clave1 es NULL", !abb_obtener(abb, clave1));
	print_test("La cantidad de elementos es 1", abb_cantidad(abb) == 1);

	// Pruebo insertar luego de borrar
	print_test("Guardar clave3", abb_guardar(abb, clave3, &valor3));
	print_test("Obtener clave3 es valor3", abb_obtener(abb, clave3) == &valor3);
	print_test("La cantidad de elementos es 2", abb_cantidad(abb) == 2);

	abb_destruir(abb);
}

void pruebas_iterador_externo() {
	printf("\nPRUEBAS ITERADOR EXTERNO\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave1 = "0";
	const char* clave2 = "-1";
	const char* clave3 = "1";

	int valor1;
	int valor2;
	int valor3;

	print_test("Guardar clave1", abb_guardar(abb, clave1, &valor1));
	print_test("Guardar clave2", abb_guardar(abb, clave2, &valor2));
	print_test("Guardar clave3", abb_guardar(abb, clave3, &valor3));

	abb_iter_t* iter = abb_iter_in_crear(abb);

	// Primera clave
	print_test("Esta al final es false", !abb_iter_in_al_final(iter));
	print_test("Ver actual es la clave correcta", comparar(abb_iter_in_ver_actual(iter), clave2) == 0);
	print_test("Avanzar es true", abb_iter_in_avanzar(iter));

	// Segunda clave
	print_test("Esta al final es false", !abb_iter_in_al_final(iter));
	print_test("Ver actual es la clave correcta", comparar(abb_iter_in_ver_actual(iter), clave1) == 0);
	print_test("Avanzar es true", abb_iter_in_avanzar(iter));

	// Tercera clave
	print_test("Esta al final es false", !abb_iter_in_al_final(iter));
	print_test("Ver actual es la clave correcta", comparar(abb_iter_in_ver_actual(iter), clave3) == 0);
	print_test("Avanzar es true", abb_iter_in_avanzar(iter));

	// Al final
	print_test("Esta al final es true", abb_iter_in_al_final(iter));
	print_test("Ver actual es NULL", !abb_iter_in_ver_actual(iter));
	print_test("Avanzar es false", !abb_iter_in_avanzar(iter));

	abb_iter_in_destruir(iter);
	abb_destruir(abb);
}

void pruebas_iterador_interno() {
	printf("\nPRUEBAS ITERADOR INTENRO\n");
	abb_t* abb = abb_crear(comparar, NULL);

	const char* clave1 = "0";
	const char* clave2 = "-1";
	const char* clave3 = "1";

	int valor1;
	int valor2;
	int valor3;

	print_test("Guardar clave1", abb_guardar(abb, clave1, &valor1));
	print_test("Guardar clave2", abb_guardar(abb, clave2, &valor2));
	print_test("Guardar clave3", abb_guardar(abb, clave3, &valor3));

	int contador = 0;
	abb_in_order(abb, imprimir, &contador);
	print_test("Itero el abb imprimiendo las claves", true);

	abb_destruir(abb);
}

void pruebas_volumen(size_t largo) {
	printf("\nPRUEBAS DE VOLUMEN\n");

	clock_t begin = clock();

	abb_t* abb = abb_crear(comparar, free);

    const size_t largo_clave = 100;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];

    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
    	sprintf(claves[i], "%08d", rand());
    	valores[i] = i;
    	ok = abb_guardar(abb, claves[i], &valores[i]);
    	if (!ok) break;
    }

    print_test("Prueba abb almacenar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb, claves[i]) == &valores[i];
        if (!ok) break;
    }

    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que borre y devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb, claves[i]) == &valores[i];
        if (!ok) break;
    }

    print_test("Prueba abb borrar muchos elementos", ok);
   	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Tiempo elapsado: %f\n", time_spent);

    abb_destruir(abb);    	
    free(claves);


}

void pruebas_abb_alumno() {
	pruebas_abb_vacio();
	pruebas_guardar();
	pruebas_reemplazar();
	pruebas_reemplazar_con_destruir();
	pruebas_clave_vacia();
	pruebas_valor_null();
	pruebas_borrar_hoja();
	pruebas_borrar_con_un_hijo();
	pruebas_borrar_con_dos_hijos();
	pruebas_iterador_externo();
	pruebas_iterador_interno();
	pruebas_volumen(1000);
}