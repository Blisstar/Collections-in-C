#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

struct nodo;
typedef struct nodo nodo_t;

struct nodo{
    void* dato;
    nodo_t* prox;
};

struct cola {
    nodo_t* prim;
    nodo_t* ulti;
    size_t cant;
};

nodo_t* nodo_crear(void* dato){
    nodo_t* nodo=malloc(sizeof(nodo_t));
    if (nodo==NULL){
        return NULL;
    }
    nodo->dato=dato;
    nodo->prox=NULL;
    return nodo;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t* cola_crear(void){
    cola_t* cola=malloc(sizeof(cola_t));
    if (cola==NULL){
        return NULL;
    }
    cola->prim=NULL;
    cola->ulti=NULL;
    cola->cant=0;
    return cola;
}

void cola_destruir(cola_t* cola, void destruir_dato(void*)){
    void* dato;
    while (!cola_esta_vacia(cola)){
        dato=cola_desencolar(cola);
        if (destruir_dato!=NULL){
            destruir_dato(dato);
        }
    }
    free(cola);
}

size_t cola_cantidad(const cola_t *cola){
    return cola->cant;
}

bool cola_esta_vacia(const cola_t* cola){
    if (cola->cant==0 && cola->prim==NULL && cola->ulti==NULL){
        return true;
    }
    return false;    
}

bool cola_encolar(cola_t* cola,void* valor){
    nodo_t* nodo=nodo_crear(valor);
    if (nodo==NULL){
        return false;
    }
    if (cola_esta_vacia(cola)){
        cola->prim=nodo;
    }else{
        cola->ulti->prox=nodo;
    }
    cola->ulti=nodo;
    cola->cant+=1;
    return true;
}

void* cola_ver_primero(const cola_t *cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }
    return cola->prim->dato;
}

void* cola_desencolar(cola_t* cola){
    if (cola_esta_vacia(cola)){
        return NULL;
    }
    void* dato=cola->prim->dato;
    nodo_t* nodo=cola->prim;
    cola->prim=nodo->prox;
    if (cola->cant==1){
        cola->ulti=NULL;
    }
    cola->cant-=1;
    free(nodo);
    return dato;
}

//Dada una cola y un número k, devuelva los primeros k elementos 
//de la cola, en el mismo orden en el que habrían salido
//de la cola. En caso que la cola tenga menos de k elementos, 
//rellenar con NULL.
//La cola debe quedar en el mismo estado que al invocarse la primitiva.
void **cola_multiprimeros(const cola_t* cola, size_t k){
    nodo_t *act = cola->prim;
    void **datos_k = malloc(sizeof(void*)*k);
    if (!datos_k) return NULL;
    void* dato = NULL;
    for (size_t i = 0; i < k; i++){
        if (act) dato = act->dato;
        else dato = NULL;
        datos_k[i] = dato;
        if (act) act = act->prox;
    }
    return datos_k;
}

/* *****************************************************************
 *                    FUNCIONES DE LA COLA
 * *****************************************************************/

void **cola_multiprimeros(cola_t* cola, size_t k){
    void **datos_k = malloc(sizeof(void*)*k);
    if (!datos_k) return NULL;
    cola_t *cola_aux = cola_crear();
    if (!cola_aux){
        free(datos_k);
        return NULL;
    }
    void* dato = NULL;
    for (size_t i = 0; i < k; i++){
        dato = cola_desencolar(cola);
        cola_encolar(cola_aux, dato);
        datos_k[i] = dato;
    }
    while (!cola_esta_vacia(cola)){
        dato = cola_desencolar(cola);
        cola_encolar(cola_aux, dato);
    while (!cola_esta_vacia(cola_aux)){
        dato = cola_desencolar(cola_aux);
        cola_encolar(cola, dato);
    }
    cola_destruir(cola_aux);
    return datos_k;
}