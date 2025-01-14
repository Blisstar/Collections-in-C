#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include "strutil.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *substr(const char *str, size_t n) {
	char* substr = malloc((n+1) * sizeof(char));
	if (!substr) return NULL;

	size_t largo = strlen(str);

	int i = 0;
	for (; i < largo; ++i) { 
		if (i >= n) break;
		substr[i] = str[i];
	}

	substr[i] = '\0';
	return substr;
}

char** split(const char* str, char sep) {
	size_t largo = strlen(str);

	size_t largo_arreglo = 1; // 1 para el NULL que se agrega al final del arreglo

	// Cuántas veces aparece el separador
	for (int i = 0; i < largo; i++) {
		if (str[i] == sep) largo_arreglo++;
	}

	char arreglo[largo_arreglo][largo];

	char** arreglo_dinamico = calloc(largo_arreglo+1, sizeof(char*));

	size_t subcadena = 0; // El índice de la subcadena del arreglo 

	size_t posicion_subcadena = 0; 

	for (int j = 0; j < largo; j++) {
		char* substr = arreglo[subcadena];

		substr[posicion_subcadena] = str[j];

		if (str[j] != sep) {
			if (j == largo-1) substr[posicion_subcadena+1] = '\0';
			posicion_subcadena++;
		}
		else {
			substr[posicion_subcadena] = '\0'; // Termino la cadena
			subcadena++; // Paso a otra subcadena
			if (j == largo - 1) {
				substr = arreglo[subcadena];
				substr[0] = '\0';
				continue;
			}
			posicion_subcadena = 0;
		}
	}

	for (int k = 0; k < largo_arreglo; k++) {
		if (largo == 0) {
			arreglo[0][0] = '\0';
			arreglo_dinamico[0] = strdup(arreglo[0]);
			break;
		}
		arreglo_dinamico[k] = strdup(arreglo[k]);
	}

	arreglo_dinamico[largo_arreglo] = NULL;

	return arreglo_dinamico;
}

size_t largo(char** strv) {
	size_t largo_arreglo = 0;
	for (int j = 0; strv[j] != NULL; j++) {
		largo_arreglo++;
	}

	return largo_arreglo;
}


char *join(char **strv, char sep) {

	size_t largo_arreglo = 0;

	size_t cant_sep = 0;

	for (int j = 0; strv[j] != NULL; j++) {
		largo_arreglo += strlen(strv[j]);
		cant_sep++;
	}

	char* cadena = malloc(sizeof(char) * (largo_arreglo+1+cant_sep));
	cadena[0] = '\0';	

	char separador[1] = {sep};

	char* puntero_a_final = cadena;

	for (int i = 0; strv[i] != NULL; i++) {
		puntero_a_final = stpcpy((void*)puntero_a_final, strv[i]);

		if (strv[i+1] != NULL) {
			puntero_a_final = stpcpy((void*)puntero_a_final, separador);
		}
	}

	return cadena;
}

void free_strv(char *strv[]) {

	for (int i = 0; strv[i] != NULL; i++ ) {		
		free(strv[i]);
	}

	free(strv);
}
