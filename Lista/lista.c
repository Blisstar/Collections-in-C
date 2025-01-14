#include "lista.h"
#include <stdlib.h>

struct nodo;
typedef struct nodo nodo_t;

/* ******************************************************************
 *                DECLARACIÓN DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista{
    nodo_t *prim;
    nodo_t *ulti;
    size_t largo;
};

struct nodo{
    void *dato;
    nodo_t *prox;
};

struct lista_iter{
    lista_t *lista;
    nodo_t *ant;
    nodo_t *act;
};

/* ******************************************************************
 *                       PRIMITIVA DEL NODO
 * *****************************************************************/

nodo_t* nodo_crear(void* dato,nodo_t *prox){
    nodo_t* nodo=malloc(sizeof(nodo_t));
    if (nodo==NULL){
        return NULL;
    }
    nodo->dato=dato;
    nodo->prox=prox;
    return nodo;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void){
    lista_t *lista=malloc(sizeof(lista_t));
    if (lista==NULL){
        return NULL;
    }
    lista->prim=NULL;
    lista->ulti=NULL;
    lista->largo=0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    if (lista->prim==NULL && lista->ulti==NULL && lista->largo==0){
        return true;
    }
    return false;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t *nodo=nodo_crear(dato,lista->prim);
    if (nodo==NULL){
        return false;
    }
    if (lista_esta_vacia(lista)){
        lista->ulti=nodo;
    }
    lista->prim=nodo;
    lista->largo+=1;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t *nodo=nodo_crear(dato,NULL);
    if (nodo==NULL){
        return false;
    }
    if (lista_esta_vacia(lista)){
        lista->prim=nodo;
    }else{
        lista->ulti->prox=nodo;
    }
    lista->ulti=nodo;
    lista->largo+=1;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    if (lista->largo==1){
        lista->ulti=NULL;
    }
    nodo_t *nodo=lista->prim;
    void *dato=nodo->dato;
    lista->prim=lista->prim->prox;
    lista->largo-=1;
    free(nodo);
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
    if (lista_esta_vacia(lista)){
        return NULL;
    }
    return lista->ulti->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t* lista, void destruir_dato(void*)){
    void* dato;
    while (!lista_esta_vacia(lista)){
        dato=lista_borrar_primero(lista);
        if (destruir_dato!=NULL){
            destruir_dato(dato);
        }
    }
    free(lista);
}

void *lista_ind_reverso(const lista_t *lista,size_t indice){
    nodo_t *act = lista->prim;
    if (!act) return NULL;
    nodo_t *nodo_ind = lista->prim;
    while (indice != 0){
        act = act->prox;
        indice--;
    }
    while (act){
        act = act->prox;
        nodo_ind = nodo_ind->prox;
    }
    return nodo_ind->dato;
}

/* ******************************************************************
 *                  PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t *act=lista->prim;
    while (act!=NULL){
        if (!visitar(act->dato,extra)){
            break;
        }
        act=act->prox;
    }
}

/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t *iter=malloc(sizeof(lista_iter_t));
    if (iter==NULL){
        return NULL;
    }
    iter->lista=lista;
    iter->ant=NULL;
    iter->act=lista->prim;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (iter->act==NULL){
        return false;
    }
    iter->ant=iter->act;
    iter->act=iter->act->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if (iter->act==NULL){
        return NULL;
    }
    return iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    if (iter->act==NULL){
        return true;
    }
    return false;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    bool comprobacion=true;
    if (iter->ant==NULL){
        comprobacion=lista_insertar_primero(iter->lista,dato);
        iter->act=iter->lista->prim;
        return comprobacion;
    }
    if (iter->act==NULL){
        comprobacion=lista_insertar_ultimo(iter->lista,dato);
        iter->act=iter->lista->ulti;
        return comprobacion;
    }
    nodo_t *nodo=nodo_crear(dato,iter->act);
    if (nodo==NULL){ 
        return !comprobacion;
    }
    iter->ant->prox=nodo;
    iter->act=nodo;
    iter->lista->largo+=1;
    return comprobacion;
}

void *lista_iter_borrar(lista_iter_t *iter){
    if (lista_esta_vacia(iter->lista) || iter->act==NULL){
        return NULL;
    }
    if (iter->ant==NULL){
        iter->act=iter->act->prox;
        return lista_borrar_primero(iter->lista);
    }
    if (iter->act->prox==NULL){
        iter->lista->ulti=iter->ant;    
    }
    nodo_t *nodo=iter->act;
    void *dato=nodo->dato;
    iter->act=iter->act->prox;
    iter->ant->prox=iter->act;
    iter->lista->largo-=1;
    free(nodo);
    return dato;
}