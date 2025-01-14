#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"

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
    n_abb_t* actual;
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

n_abb_t* buscar_nodo(n_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp) {
    /* Busca en el árbol el nodo que contenga la clave recibida y lo devuelve
     * Si no se encuentra devuelve NULL.
     */
     
    if (!nodo) return NULL;

    int comparacion = cmp(nodo->clave, clave);

    if (comparacion == 0) return nodo;
    if (comparacion > 0) {
        return buscar_nodo(nodo->izq, clave, cmp);
    }
    else {
        return buscar_nodo(nodo->der, clave, cmp);
    }
}

bool _abb_guardar(abb_t* arbol, n_abb_t* nodo_actual, n_abb_t* nodo_nuevo);
void* _abb_borrar(abb_t* arbol, const char* clave, n_abb_t* nodo);
n_abb_t* buscar_padre_nodo(n_abb_t* nodo_actual, const char* clave, abb_comparar_clave_t cmp);
void borrar_hoja(abb_t* arbol, const char* clave);
void borrar_nodo_un_hijo(abb_t* arbol, const char* clave, n_abb_t* nodo_a_borrar);
void borrar_nodo_dos_hijos(abb_t* arbol, n_abb_t* nodo_a_borrar);
n_abb_t* buscar_reemplazante(n_abb_t* nodo);


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
    n_abb_t* nodo = buscar_nodo(arbol->raiz, clave, arbol->cmp);
    if (!nodo) return NULL;

    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    if (buscar_nodo(arbol->raiz, clave, arbol->cmp)){
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

bool abb_descendiente(n_abb_t* nodo, bool visitar(const char*, void* valor, void* extra), void* extra) {
    if (!nodo) return true;

    if (!abb_descendiente(nodo->der, visitar, extra)) return false;
    if (!visitar(nodo->clave, nodo->dato, extra)) return false;
    if (!abb_descendiente(nodo->izq, visitar, extra)) return false;
    return true;
}

void abb_iterar_descendiente(abb_t* arbol, bool visitar(const char*, void* valor, void* extra), void* extra) {
    abb_descendiente(arbol->raiz, visitar, extra);
}

n_abb_t* buscar_padre_nodo(n_abb_t* nodo_actual, const char* clave, abb_comparar_clave_t cmp) {
    /* Devuelve el padre del nodo recibido
     */

    if (cmp(clave, nodo_actual->clave) > 0) {
        if (cmp(nodo_actual->der->clave, clave) == 0) {
            return nodo_actual;
        }
        return buscar_padre_nodo(nodo_actual->der, clave, cmp);
    }
    else {
        if (cmp(nodo_actual->izq->clave, clave) == 0){
            return nodo_actual;
        }
        return buscar_padre_nodo(nodo_actual->izq, clave, cmp);
    }

}

void* abb_borrar(abb_t* arbol, const char* clave) {
    n_abb_t* nodo = buscar_nodo(arbol->raiz, clave, arbol->cmp);
    if (!nodo) return NULL;

    arbol->cantidad--;

    return _abb_borrar(arbol, clave, nodo);
}

void* _abb_borrar(abb_t* arbol, const char* clave, n_abb_t* nodo) {
    void* dato = nodo->dato;

    if (!nodo->der && !nodo->izq) {
        borrar_hoja(arbol, clave);
        nodo_abb_destruir(nodo, NULL);
    }
    else if (!nodo->der || !nodo->izq) {
        borrar_nodo_un_hijo(arbol, clave, nodo);
        nodo_abb_destruir(nodo, NULL);
    }
    else if (nodo->der && nodo->izq){
        borrar_nodo_dos_hijos(arbol, nodo);
    }

    return dato;
}

void borrar_hoja(abb_t* arbol, const char* clave) {
    if (arbol->cmp(arbol->raiz->clave, clave) == 0) {
        arbol->raiz = NULL;          
        return;  
    }

    n_abb_t* padre_de_hoja = buscar_padre_nodo(arbol->raiz, clave, arbol->cmp);

    if (arbol->cmp(padre_de_hoja->clave, clave) > 0) {
        padre_de_hoja->izq = NULL;
    }
    else padre_de_hoja->der = NULL;
}

void borrar_nodo_un_hijo(abb_t* arbol, const char* clave, n_abb_t* nodo_a_borrar) {
    n_abb_t* reemplazo = (nodo_a_borrar->der) ? nodo_a_borrar->der : nodo_a_borrar->izq;

    if (arbol->cmp(arbol->raiz->clave, clave) == 0) {
        arbol->raiz = reemplazo;
        return;
    }

    n_abb_t* padre_de_nodo = buscar_padre_nodo(arbol->raiz, clave, arbol->cmp);
    if (arbol->cmp(padre_de_nodo->clave, nodo_a_borrar->clave) > 0) {
        padre_de_nodo->izq = reemplazo;
    }
    else padre_de_nodo->der = reemplazo;
}

n_abb_t* buscar_reemplazante(n_abb_t* nodo) { 
    /* Recibe el hijo derecho del nodo a borrar
     * Devuelve su hijo más izquierdo
     */
    
    if (!nodo->izq) return nodo;
    return buscar_reemplazante(nodo->izq);
}


void borrar_nodo_dos_hijos(abb_t* arbol, n_abb_t* nodo_a_borrar) {
    n_abb_t* reemplazante = buscar_reemplazante(nodo_a_borrar->der);

    char* clave_reemplazante = strdup(reemplazante->clave);
    void* dato = _abb_borrar(arbol, reemplazante->clave, reemplazante);
   
    free(nodo_a_borrar->clave);
    nodo_a_borrar->clave = clave_reemplazante;
    nodo_a_borrar->dato = dato;
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

    iter->actual = pila_ver_tope(iter->pila);

    return iter;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (!iter->actual) return NULL;
    return iter->actual->clave;
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
    
    iter->actual = pila_ver_tope(iter->pila);

    return true;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}
