#define _GNU_SOURCE
#include "strutil.h"
#include "cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Dado una cadena con un número positivo 
 *se devuele la transformacion al formato size_t.
 */ 
size_t size_t_convertir(const char *n){
    size_t nuevo_n=(size_t)(*n - '0');
    return nuevo_n;
}

/*Lee un archivo y con los parametros que recibe
 *va a implementar el clasico "grep" de linux.
 */
void imprimir_lineas(const char *parametro, size_t n, const char *ruta_arch){
    FILE *archivo;
    if (ruta_arch){
        archivo=fopen(ruta_arch, "r");
    }else{
        archivo=stdin; 
    }
    if (!archivo){
        fprintf(stderr, "No se pudo leer el archivo indicado\n");
        return;
    }
    char *linea=NULL;
    char *linea_aux="";
    size_t tam=0;
    ssize_t len=getline(&linea, &tam, archivo);
    cola_t *cola=cola_crear();
    while (len >= 0){
        linea_aux=substr(linea, len);
        cola_encolar(cola, linea_aux);
        if (cola_cantidad(cola) > (n+1)){
            free(cola_desencolar(cola));
        }
        if (strstr(linea_aux, parametro)){
            while (!cola_esta_vacia(cola)){
                char *linea_a_imprimir=cola_desencolar(cola);
                printf("%s", linea_a_imprimir);
                free(linea_a_imprimir);
            }
        }
        len=getline(&linea, &tam, archivo);
    }
    free(linea);
    cola_destruir(cola,free);
    fclose(archivo);
}

/*Indica si una cadena es un número positivo.
 */
bool es_digito(const char* n){
    size_t i=0;
    char act=n[i];
    while (act != '\0'){
        if (isdigit(act) == 0) return false;
        i++;
        act=n[i];
    }
    return true;
}

int main(int argc, char const *argv[]){
    if (argc < 3 || argc > 4){ 
        fprintf(stderr, "Cantidad de parametros erronea\n");
        return -1;
    }
    if (!es_digito(argv[2])){
        fprintf(stderr, "Tipo de parametro incorrecto\n");
        return -1;
    }
    size_t n=size_t_convertir(argv[2]);
    if (argc < 4) imprimir_lineas(argv[1], n, NULL);
    
    if (argc == 4) imprimir_lineas(argv[1], n, argv[3]);

    return 0;
}