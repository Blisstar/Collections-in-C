#include "pila.h"
#include <stdlib.h>

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
pila_t* pila_crear(void){
    pila_t* pila=malloc(sizeof(pila_t));
    if (pila==NULL){
        return NULL;
    }
    pila->datos=malloc(sizeof(void*)*3);
    if (pila->datos==NULL){
        free(pila);
        return NULL;
    }
    pila->cantidad=0;
    pila->capacidad=3;
    return pila;
}

void pila_destruir(pila_t* pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t* pila){
    if (pila->cantidad==0){
        return true;
    }
    return false;    
}

bool pila_redimensionar(pila_t* pila,size_t nuevo_tam){
    void* datos_aux=realloc(pila->datos,nuevo_tam*sizeof(void*));
    if (nuevo_tam<pila->cantidad || datos_aux==NULL){
        return false;
    }
    pila->datos=datos_aux;
    pila->capacidad=nuevo_tam;
    return true;
}

bool pila_apilar(pila_t* pila,void* valor){
    if (pila->cantidad==pila->capacidad){
        bool es_fallo=pila_redimensionar(pila,pila->capacidad*2);
        if (!es_fallo){
            return false;
        }
    }
    pila->datos[pila->cantidad]=valor;
    pila->cantidad+=1;
    return true;
}

void* pila_ver_tope(const pila_t* pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)){
        return NULL;
    }
    if (pila->capacidad>=pila->cantidad*4){
        pila_redimensionar(pila,pila->cantidad*2);
    }
    void* dato=pila->datos[pila->cantidad-1];
    pila->cantidad-=1;
    return dato;
}