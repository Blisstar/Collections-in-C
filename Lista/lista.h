#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

//Crea una lista.
//Post: devuelve una nueva lista vacía. 
lista_t *lista_crear(void);

//Recibe una lista, si esta vacía devuelve true y
//si no es el caso devuelve false.
//Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

//Dada una lista y un dato(void*) inserta en la primera posicion
//de la lista el dato correspondiente moviendo todos los demas
//elementos a la derecha.
//Pre: la lista fue creada.
//Post: se agrega un nuevo elemento al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Dada una lista y un dato(void*) inserta en la ultima posicion
//de la lista el dato correspondiente.
//Pre: la lista fue creada.
//Post: se agrego un elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Elimina de la lista el elemento que esta en la primera 
//posicion de la lista, si esta vacía devuelve NULL.
//Pre: la lista fue creada.
//Post: se quita el primer elemento de la lista.
void *lista_borrar_primero(lista_t *lista);

//Obtiene el primer elemento de la lista, si esta vacía devuelve NULL.
//Pre: la lista fue creada.
//Post: devuelve el primer elemento de la lista. 
void *lista_ver_primero(const lista_t *lista);

//Obtiene el ultimo elemento de la lista, si esta vacía devuelve NULL.
//Pre: la lista fue creada.
//Post: devuelve el ultimo elemento de la lista.
void *lista_ver_ultimo(const lista_t* lista);

//Obtiene la cantidad de elementos de la lista.
//Pre: la lista fue creada.
//Post: devuelve el tamaño de la lista.
size_t lista_largo(const lista_t *lista);

//Vacia y destruye la lista. Si recibe la función destruir_dato itera
//en la lista aplicandole esa función a todos los elementos.
//Pre: la lista fue creada. Si no recibe una función destruir_dato 
//se debe ingresar NULL.
//Post: elimina los elementos de la lista y libera 
//el espacio que ocupaba la lista y sus nodos.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

//Recibe una lista y devuelve el elemento que esté a k posiciones 
//del final (el ante-k-último), recorriendo la lista una sola vez 
//y sin usar estructuras auxiliares.
//Pre: Considerar que k es siempre menor
//al largo de la lista.
//Pre: La lista fue creada.
void *lista_ind_reverso(const lista_t *lista, size_t indice);

/* ******************************************************************
 *                  PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

//Sirve para iterar en la lista con un caso de corte.
//Si la función que se utiliza en cada dato devuelve false
//la iteración se corta. Se recibe un parametro adicional,
//que si no se utiliza debe ser NULL, que define un momento especifico
//para que se corte la iteración.
//Pre: La lista fue creada.
//Post: Puede que algunos datos hayan sido modificados. 
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                  PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea una estructura llamada lista_iter(iterador externo) que sirve para iterar
//en una lista haciendo referencia, al principio, al primer nodo de la lista, al 
//nodo anterior que por ahora es NULL y a la lista en si.
//Pre: la lista fue creada.
//Post: se crea un nuevo iterador externo y lo devuelve.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza una posición en la lista para la estructura lista_iter.
//Devuelve false si ya recorrio todos los elementos y true en caso
//contrario.
//Pre: el iterador externo ya fue creado.
//Post: hace referencia al siguiente nodo que tenia como nodo actual y
//al nodo que antes tenia como actual.
bool lista_iter_avanzar(lista_iter_t *iter);

//Obtiene el dato del nodo actual de la lista si 
//todavia no se recorrio toda.
//Pre: el iterador externo fue creado.
//Post: devuelve el dato que contiene el nodo 
//actual o NULL en caso de haber recorrido toda
//la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Indica si el iterador esta al final de la lista. Cuando el iterador 
//esta al final de la lista, el actual hace referencia a NULL
//y el anterior al ultimo nodo.
//Pre: el iterador externo fue cerado.
//Post: devuelve true si esta al final de la lista y false en caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador externo.
//Pre: el iterador externo fue creado.
//Post: se libera la memoria que ocupaba el iterador externo.
void lista_iter_destruir(lista_iter_t *iter);

//Agrega un elemento en la posicion que se encuentra el iterador. Devuele false
//en caso de error.
//Pre: el iterador externo fue creado.
//Post: inserta un nodo con el dato que recibe y lo pone entre los dos nodos
//que tiene como referencia.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Borra en la lista el elemento de la posicion en la 
//que se encuentra el iterador y lo devuelve.
//Pre: el iterador externo fue creado.
//Post: se libera el espacio que ocupaba el nodo y devuelve el
//dato que contenia, el actual se cambia a su proximo.
void *lista_iter_borrar(lista_iter_t *iter);

/* ******************************************************************
 *                       PRUEBAS DEL ALUMNO
 * *****************************************************************/

void pruebas_lista_alumno(void);