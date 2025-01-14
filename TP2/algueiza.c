#define _POSIX_C_SOURCE 200809L

#include "strutil.h"
#include "heap.h"
#include "abb.h"
#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct estructuras {
	abb_t* vuelos;
	hash_t* info_vuelos;
	abb_t* vuelos_con_priori;
}estructuras_t;

// Estructura para la función de imprimir los vuelos en ver_tablero
typedef struct tda {
	size_t contador;
	size_t cantidad;
	char* desde;
	char* hasta;
}extra_t;

bool es_entero(char* cadena) {
	/* Devuelve true si la cadena recibida es un entero. False en caso contrario.
	 */

	int pos = 0;

	if (cadena[0] == '-') pos++;
	if (!cadena[pos]) return false;

	for (int i = pos; cadena[i] != '\0'; i++) {
		if (!isdigit(cadena[i])) {
			return false;
		}
	}
	return true;
}

int str_comparacion(void* priori_de_vuelo1, void* priori_de_vuelo2){
	int prioridad1=atoi(priori_de_vuelo1);
	int prioridad2=atoi(priori_de_vuelo2);
	if (prioridad1 > prioridad2) return 1;
	
	else if (prioridad2 > prioridad1) return -1;

	return 0;
}

void agregar_archivo(char** input, estructuras_t* aux, lista_t* informacion_vuelos) {
    FILE* archivo;

    archivo = fopen(input[1], "r");
	if (!archivo) {
		printf("No se pudo abrir\n");
		fprintf(stderr, "Error en comando %s\n", input[0]);
		return;	
	}

    char *line = NULL;
    size_t len = 0;

   	while ((getline(&line, &len, archivo)) != -1) {
   		line[strcspn(line, "\n")] = 0;
 
  		char** datos_vuelo = split(line, ',');
  		if (!datos_vuelo) {
  			fprintf(stderr, "Error en comando %s\n", input[0]);
  			return;
		}

  		lista_insertar_primero(informacion_vuelos, datos_vuelo); // Guardo la info para no perder la referencia

 		hash_guardar(aux->info_vuelos, datos_vuelo[0], datos_vuelo); // Codigo de vuelo - Info del vuelo

		abb_guardar(aux->vuelos_con_priori, datos_vuelo[5], datos_vuelo[0]);
 		/*¿Qué estructura usar para guardar los vuelos prioritarios? Teniendo en cuenta la complejidad de 
 		de tiempo al momento de guardarlo y la complejidad para borrarlo si es que se borra el vuelo.*/ 

 		abb_guardar(aux->vuelos, datos_vuelo[6], datos_vuelo[0]); // Fecha despegue - Codigo de vuelo
   	}

   	free(line);
   	fclose(archivo);

   	fprintf(stdout, "OK\n");

}

void mostrar_info_vuelo(char** input, estructuras_t* aux) {
	char* codigo_vuelo = input[1];

	char** info_vuelo = hash_obtener(aux->info_vuelos, codigo_vuelo);
	if (!info_vuelo) {
		fprintf(stderr, "Error en comando info_vuelo\n");
		return;
	}

	char* informacion = join(info_vuelo, ' ');

	fprintf(stdout, "%s\n", informacion);
	fprintf(stdout, "OK\n");

	free(informacion);
}

void prioridad_vuelos(size_t k, estructuras_t* aux) {
	// ????
	fprintf(stdout, "OK\n");

}



/* Recibe la fecha del vuelo y su código. Los imprime si se encuentra
 * en el rango de fechas pedido.
 */
bool imprimir_asc(const char* clave, void* dato, void* _extra) {
	extra_t* extra = ((extra_t*)_extra);

    size_t contador = extra->contador;

    // Ya se imprimió la cantidad de vuelos pedidos
    if (contador >= extra->cantidad) return false;

    // Ya se imprimió la última fecha a imprimir
    if (strcmp(clave, extra->hasta) > 0) return false;

    // Todavía no está en el rango de vuelos pedidos
    if (strcmp(clave, extra->desde) < 0) return true;

    ++extra->contador;
    fprintf(stdout, "%s - %s\n", clave, (char*)dato);
    return true;
}

bool imprimir_desc(const char* clave, void* dato, void* _extra) {
	extra_t* extra = ((extra_t*)_extra);

    size_t contador = extra->contador;

    // Ya se imprimió la cantidad de vuelos pedidos
    if (contador >= extra->cantidad) return false;
    
    if (strcmp(clave, extra->desde) < 0) return false;

    if (strcmp(clave, extra->hasta) > 0) return true;

    ++extra->contador;
    fprintf(stdout, "%s - %s\n", clave, (char*)dato);
    return true;  
} 
   


void mostrar_tablero(size_t k, char* orden, char* desde, char* hasta, estructuras_t* aux) {
	if (k <= 0 || strcmp(desde, hasta) > 0) {
		fprintf(stderr, "Error en comando ver_tablero\n");
		return;
	}

	extra_t extra = {0, k, desde, hasta};

	if (!strcmp(orden, "asc")) {
		abb_in_order(aux->vuelos, imprimir_asc, &extra);
		fprintf(stdout, "OK\n");
	}

	else if (!strcmp(orden, "desc")) {
		abb_iterar_descendiente(aux->vuelos, imprimir_desc, &extra);
		fprintf(stdout, "OK\n");
	}

	else fprintf(stderr, "Error en comando ver_tablero\n");

}

/* Recorre el ABB de vuelos y guarda y devuelve en una lista los vuelos
 * cuyas fechas se encuentren en el rango recibido.
 */
lista_t* vuelos_a_borrar(abb_t* vuelos, char* desde, char* hasta) {
	abb_iter_t* iter = abb_iter_in_crear(vuelos);
	lista_t* vuelos_a_borrar = lista_crear(); 

	while (!abb_iter_in_al_final(iter) && strcmp(abb_iter_in_ver_actual(iter), hasta) <= 0) {

		const char* fecha = abb_iter_in_ver_actual(iter);

		if (strcmp(fecha, desde) < 0) {
			abb_iter_in_avanzar(iter);
			continue; // Todavía no está en el rango de fechas a borrar
		}

		lista_insertar_ultimo(vuelos_a_borrar, strdup(fecha));
		abb_iter_in_avanzar(iter);
	}	

	abb_iter_in_destruir(iter);	
	return vuelos_a_borrar;
}

void borrar_vuelos(estructuras_t* aux, char* desde, char* hasta) {
	if (strcmp(desde, hasta) > 0) {
		fprintf(stderr, "Error en comando borrar\n");
		return;
	}

	lista_t* vuelos = vuelos_a_borrar(aux->vuelos, desde, hasta);

	lista_iter_t* iterador = lista_iter_crear(vuelos);

	// Los borro del hash y del ABB
	while (!lista_iter_al_final(iterador)) {
		hash_borrar(aux->info_vuelos, abb_borrar(aux->vuelos, lista_iter_ver_actual(iterador)));
		lista_iter_avanzar(iterador);
	}

	lista_iter_destruir(iterador);
	lista_destruir(vuelos, free);

	fprintf(stdout, "OK\n");
}

void procesar_comando(char** input, estructuras_t* aux, lista_t* informacion_vuelos) {
	char* comando = input[0];
	bool ok = true;

	if (!comando) {
		ok = false;
	}

	else if (!strcmp(comando, "agregar_archivo")) {
		if (largo(input) == 1) {
			ok = false;
		}
		else agregar_archivo(input, aux, informacion_vuelos);		
	}
	else if (!strcmp(comando, "info_vuelo")) {
		if (largo(input) == 1) {
			ok = false;
		}
		else mostrar_info_vuelo(input, aux);
	}
	else if (!strcmp(comando, "prioridad_vuelos")) {
		if ((largo(input) == 1) | (!es_entero(input[1]))) {
			ok = false;
		}
		else prioridad_vuelos(atoi(input[1]), aux);
	}
	else if (!strcmp(comando, "ver_tablero")) {
		if (largo(input) < 5 || !es_entero(input[1])) {
			ok = false;
		}
		else mostrar_tablero(atoi(input[1]), input[2], input[3], input[4], aux);
	}
	else if (!strcmp(comando, "borrar")) {
		if (largo(input) < 3) {
			ok = false;
		}
		else borrar_vuelos(aux, input[1], input[2]);
	}

	if (!ok) fprintf(stderr, "Error en comando %s\n", comando);

}

void destruir_estructura(estructuras_t* aux) {
	if (aux->info_vuelos) hash_destruir(aux->info_vuelos);
	if (aux->vuelos) abb_destruir(aux->vuelos);
	free(aux);
}		

/* Crea todas las estructuras necesarias para la ejecución del programa y las
 * devuelve en una estructura auxiliar. Devuelve NULL en caso de error.
 */
estructuras_t* crear_tdas(void) {
	estructuras_t* aux = malloc(sizeof(estructuras_t));
	if (!aux) return NULL;

	aux->vuelos = abb_crear(strcmp, NULL);
	if (!aux->vuelos) {
		destruir_estructura(aux);
		return NULL;
	}
	aux->info_vuelos = hash_crear(NULL);
	if (!aux->info_vuelos) {
		destruir_estructura(aux);
		return NULL;
	}
	aux->vuelos_con_priori=abb_crear(str_comparacion,NULL);
	if (!aux->vuelos_con_priori){
		destruir_estructura(aux);
		return NULL;
	}
	// ¿Usar un heap para las prioridades?

	return aux;
}

int main(int argc, char* argv[]) {
	FILE* archivo = stdin;

	estructuras_t* aux = crear_tdas();
	if (!aux) return -1;

	lista_t* informacion_vuelos = lista_crear(); // Copio la info de los vuelos para no perder la referencia

	char* line = NULL;
	size_t len = 0;
	char** input;
	while ((getline(&line, &len, archivo)) != -1) {
		line[strcspn(line, "\n")] = 0;

		input = split(line, ' ');
		procesar_comando(input, aux, informacion_vuelos);

		free_strv(input);
	}

	free(line);
	destruir_estructura(aux);
	_lista_destruir(informacion_vuelos, free_strv);

}
