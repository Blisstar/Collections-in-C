#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;


// Tipo de función para comparar dos elementos
typedef int (*abb_comparar_clave_t) (const char *, const char *);

// Tipo de función para destruir dato
typedef void (*abb_destruir_dato_t) (void *);

/* ******************************************************************
 *                         PRIMITIVAS DEL AB
 * *****************************************************************/

/* Devuelve la cantidad de elementos del AB.
 * Pre: La estrucutra AB fue inicializada.
 */
size_t ab_cantidad(const abb_t *arbol);

/*Recibe un árbol binario que representa un heap 
 *(árbol binario izquierdista, que cumple la propiedad de heap),
 *y devuelve la representación en arreglo del heap.
 */
void** ab_repr_arreglo(const abb_t *arbol);

/* ******************************************************************
 *                         PRIMITIVAS DEL ABB
 * *****************************************************************/

/* Crea el ABB
 * Post: devuelve un ABB vacío
 */ 
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


/* Guarda un elemento en el árbol. Si no se puede guardar devuelve false.
 * Pre: La estructura ABB fue incializada
 * Post: Se guardó el elemento
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra el elemento del árbol y devuelve su valor. Devuelve NULL si 
 * si el elemento no existía.
 * Pre: La estructura ABB fue inicializada.
 * Post: El elemento fue borrado de la estrucutra, si éste estaba guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del ABB, si la clave no se encuentra 
 * devuelve NULL.
 * Pre: La estructura ABB fue inicializada.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si la clave pertenece o no al ABB.
 * Pre: La estructura ABB fue inicializada.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del ABB.
 * Pre: La estrucutra ABB fue inicializada.
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura liberando liberando su memoria y llamando la función
 * destruir para cada par (clave, dato).
 * Pre: la estructura ABB fue inicializada.
 * Post: La estrucutra ABB fue destruida.
 */
void abb_destruir(abb_t *arbol);

/* ******************************************************************
 *                       ITERADOR INTERNO
 * *****************************************************************/

/* Itera el ABB inorder aplicando la función visitar a cada uno de los los elementos del árbol
 * hasta que ésta devuelva false o el iterador se encuentre al final del árbol.
 * Pre: el ABB fue creada.
 */
void abb_in_order(abb_t *arbol, bool visitar(const char*, void* valor, void* extra), void *extra);

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador inorder externo del abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza al siguiente nodo.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Obtiene el valor del nodo actual, devolviendo NULL si éste no existe.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Devuelve true si se encuentra en la posición final del árbol.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador y libera su memoria.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H