#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include "cola.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/* ******************************************************************
 *                DECLARACIÓN DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct n_abb{
    struct n_abb* izq;
    struct n_abb* der;
    void* dato;
    char* clave;
}n_abb_t;

struct abb {
    n_abb_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

struct abb_iter{
    pila_t* pila;
};


/* ******************************************************************
 *                     PRIMITIVAS DEL NODO ABB
 * *****************************************************************/

n_abb_t* nodo_abb_crear(const char *clave, void *dato){
    n_abb_t* nodo_abb = malloc(sizeof(n_abb_t));    
    if (!nodo_abb) return NULL;

    nodo_abb->clave = strdup(clave);
    if (!clave){
        free(nodo_abb);
        return NULL;
    }

    nodo_abb->dato = dato;
    nodo_abb->izq = NULL;
    nodo_abb->der = NULL;
    return nodo_abb;
}

void nodo_abb_destruir(n_abb_t* nodo, abb_destruir_dato_t destruir_dato){
    if (destruir_dato){
        destruir_dato(nodo->dato);
    }

    free(nodo->clave);
    free(nodo);
}

/* ******************************************************************
 *                      FUNCIONES ADICIONALES
 * *****************************************************************/

n_abb_t** buscar_padre_e_hijo(n_abb_t* hijo, n_abb_t* padre, const char* clave, abb_comparar_clave_t cmp) {
    /* Busca en el árbol el nodo que contenga la clave recibida y lo devuelve
     * Si no se encuentra devuelve NULL.
     */
     
    if (!hijo) return NULL;

    int comparacion = cmp(hijo->clave, clave);

    if (comparacion == 0){
        n_abb_t** papa_e_hijo = malloc(sizeof(n_abb_t*)*2);
        papa_e_hijo[0] = hijo; 
        papa_e_hijo[1] = padre;
        return papa_e_hijo;
    }
    if (comparacion > 0) {
        return buscar_padre_e_hijo(hijo->izq, hijo, clave, cmp);
    }
    else {
        return buscar_padre_e_hijo(hijo->der, hijo, clave, cmp);
    }
}

bool _abb_guardar(abb_t* arbol, n_abb_t* nodo_actual, n_abb_t* nodo_nuevo);
void* _abb_borrar(abb_t* arbol, const char* clave, n_abb_t* hijo, n_abb_t* padre);
void borrar_hoja(abb_t* arbol, const char* clave, n_abb_t* hijo, n_abb_t* padre);
void borrar_nodo_un_hijo(abb_t* arbol, const char* clave, n_abb_t* hijo, n_abb_t* padre);
void borrar_nodo_dos_hijos(abb_t* arbol, n_abb_t* hijo);
n_abb_t* buscar_hijastro(n_abb_t* nodo);

/* ******************************************************************
 *                         PRIMITIVAS DEL AB
 * *****************************************************************/

size_t _ab_cantidad(const n_abb_t *arbol){
    if (!arbol) return 0;
    size_t cant_hijos_izq = _ab_cantidad(arbol->izq);
    size_t cant_hijos_der = _ab_cantidad(arbol->der);
    return cant_hijos_izq + cant_hijos_der + 1;
}

size_t ab_cantidad(const abb_t *arbol){
    return _ab_cantidad(arbol->raiz);
}

void _ab_repr_arreglo(cola_t *cola, void** heap_arreglo, size_t ind){
    if (cola_esta_vacia(cola)) return;
    
    n_abb_t *nodo_ab = cola_desencolar(cola);
    heap_arreglo[ind] = nodo_ab->dato;
    if (nodo_ab->izq) cola_encolar(cola, nodo_ab->izq);

    if (nodo_ab->der) cola_encolar(cola, nodo_ab->der);

    _ab_repr_arreglo(cola, heap_arreglo, ind+1);
}

void** ab_repr_arreglo(const abb_t *arbol){
    size_t cant = ab_cantidad(arbol);
    void** heap_arreglo = malloc(sizeof(void*)*cant);
    if (!heap_arreglo) return NULL;

    cola_t *cola = cola_crear();
    if (!cola){
        free(heap_arreglo);
        return NULL;
    }
    cola_encolar(cola, arbol->raiz);
    _ab_repr_arreglo(cola, heap_arreglo, 0);
    free(cola);
    return heap_arreglo;
}
/* ******************************************************************
 *                       PRIMITIVAS DEL ABB
 * *****************************************************************/

 
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;

    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;

    return abb;
}
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    n_abb_t* nodo_nuevo = nodo_abb_crear(clave, dato);
    if (!nodo_nuevo) return false;

    arbol->cantidad++;

    if (!arbol->raiz) {
        arbol->raiz = nodo_nuevo;
        return true;
    }

    return _abb_guardar(arbol, arbol->raiz, nodo_nuevo);

}

bool _abb_guardar(abb_t* arbol, n_abb_t* nodo_actual, n_abb_t* nodo_nuevo) {
    /* Guarda el nuevo nodo en el árbol
     * Si ya existía un elemento con la misma clave reemplaza su valor
     */

    abb_comparar_clave_t cmp = arbol->cmp;

    int comparacion = cmp(nodo_actual->clave, nodo_nuevo->clave);
    
    if (comparacion == 0) {
        if (arbol->destruir_dato) arbol->destruir_dato(nodo_actual->dato);
        nodo_actual->dato = nodo_nuevo->dato;
        nodo_abb_destruir(nodo_nuevo, NULL);
        arbol->cantidad--; // En abb_guardar se aumentó la cantidad pero no estamos agregando un elemento
    }
    if (comparacion > 0) {
        if (!nodo_actual->izq) {
            nodo_actual->izq = nodo_nuevo;
        }
        else _abb_guardar(arbol, nodo_actual->izq, nodo_nuevo);
    }
    if (comparacion < 0) {
        if (!nodo_actual->der) {
            nodo_actual->der = nodo_nuevo;
        }
        else _abb_guardar(arbol, nodo_actual->der, nodo_nuevo);
    }

    return true;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    n_abb_t** padre_e_hijo = buscar_padre_e_hijo(arbol->raiz, NULL, clave, arbol->cmp);
    void* dato = NULL;
    if (padre_e_hijo) dato = padre_e_hijo[0]->dato;

    free(padre_e_hijo);
    return dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    n_abb_t** padre_e_hijo = buscar_padre_e_hijo(arbol->raiz, NULL, clave, arbol->cmp);
    if (padre_e_hijo){
        free(padre_e_hijo);
        return true;
    }
    return false;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(n_abb_t* nodo, abb_destruir_dato_t destruir_dato) {
    /* Recibe la raíz del ABB y lo recorre postorder destruyendo cada nodo
     */

    if (!nodo) return;

    _abb_destruir(nodo->izq, destruir_dato);
    _abb_destruir(nodo->der, destruir_dato);
    nodo_abb_destruir(nodo, destruir_dato);
}

void abb_destruir(abb_t *arbol) {
    _abb_destruir(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}

bool abb_iterar(n_abb_t* nodo, bool visitar(const char*, void* valor, void* extra), void* extra) {
    /* Itera el árbol inorder y le aplica la función recibida a cada elemento
     */

    if (!nodo) return true;
    
    if (!abb_iterar(nodo->izq, visitar, extra)) return false;
    if (!visitar(nodo->clave, nodo->dato, extra)) return false;
    if (!abb_iterar(nodo->der, visitar, extra)) return false;
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void* valor, void* extra), void *extra) {
    abb_iterar(arbol->raiz, visitar, extra);
}

void* abb_borrar(abb_t* arbol, const char* clave) {
    n_abb_t** nodo = buscar_padre_e_hijo(arbol->raiz, NULL, clave, arbol->cmp);
    if (!nodo) return NULL;

    arbol->cantidad--;

    void* dato = _abb_borrar(arbol, clave, nodo[0], nodo[1]);
    free(nodo);
    return dato;
}

void* _abb_borrar(abb_t* arbol, const char* clave, n_abb_t* hijo, n_abb_t* padre) {
    void* dato = hijo->dato;

    if (!hijo->der && !hijo->izq) {
        borrar_hoja(arbol, clave, hijo, padre);
        nodo_abb_destruir(hijo, NULL);
    }
    else if (!hijo->der || !hijo->izq) {
        borrar_nodo_un_hijo(arbol, clave, hijo, padre);
        nodo_abb_destruir(hijo, NULL);
    }
    else if (hijo->der && hijo->izq){
        borrar_nodo_dos_hijos(arbol, hijo);
    }

    return dato;
}

void borrar_hoja(abb_t* arbol ,const char* clave, n_abb_t* hijo, n_abb_t* padre) {
    if (!padre) {
        arbol->raiz = NULL;          
        return;  
    }

    if (arbol->cmp(padre->clave, clave) > 0) {
        padre->izq = NULL;
    }
    else padre->der = NULL;
}

void borrar_nodo_un_hijo(abb_t* arbol, const char* clave, n_abb_t* hijo, n_abb_t* padre) {
    n_abb_t* hijastro = (hijo->der) ? hijo->der : hijo->izq;

    if (!padre) {
        arbol->raiz = hijastro;
        return;
    }

    if (arbol->cmp(padre->clave, hijastro->clave) > 0) {
        padre->izq = hijastro;
    }
    else padre->der = hijastro;
}

n_abb_t* buscar_padre_del_hijastro(n_abb_t* nodo) { 
    /* Recibe el hijo derecho del nodo a borrar
     * Devuelve su hijo más izquierdo
     */
    
    if (!nodo->izq->izq) return nodo;
    return buscar_padre_del_hijastro(nodo->izq);
}


void borrar_nodo_dos_hijos(abb_t* arbol, n_abb_t* hijo) {
    n_abb_t* padrasto = hijo->der->izq ? buscar_padre_del_hijastro(hijo->der) : hijo;
    n_abb_t* hijastro = hijo == padrasto ? hijo->der : padrasto->izq;

    char* clave_hijastro = strdup(hijastro->clave);
    void* dato = _abb_borrar(arbol, hijastro->clave, hijastro, padrasto);
   
    free(hijo->clave);
    hijo->clave = clave_hijastro;
    hijo->dato = dato;
}

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

void apilar_izquierdos(pila_t* pila, n_abb_t* nodo) {
    /* Apila el nodo recibido y sus hijos izquierdos
     */
     
    if (!nodo) return;

    pila_apilar(pila, nodo);
    
    apilar_izquierdos(pila, nodo->izq);
}

abb_iter_t* abb_iter_in_crear(const abb_t *arbol) {

    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;

    iter->pila = pila_crear();
    if (!iter->pila) {
        free(iter);
        return NULL;
    }
    
    if (arbol->raiz) {
        pila_apilar(iter->pila, arbol->raiz);
        apilar_izquierdos(iter->pila, arbol->raiz->izq);
    }

    return iter;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    n_abb_t *actual = pila_ver_tope(iter->pila);
    if (!actual) return NULL;
    return actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) return false;

    n_abb_t* actual = pila_desapilar(iter->pila);

    if (actual->der) {
        pila_apilar(iter->pila, actual->der);
        apilar_izquierdos(iter->pila, actual->der->izq);
    }

    return true;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}
