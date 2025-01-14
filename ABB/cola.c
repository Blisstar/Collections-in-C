#include "cola.h"
#include <stdlib.h>

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

// ...

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
