#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define TAM1 10
#define TAM2 10000

bool multiplicar_2(void *dato, void *extra){
    int *por_2=dato;
    *por_2*=2;
    return true;
}

bool recorrer_6_eleme(void *dato, void *extra){
    int *cont=extra;
    *cont+=1;
    if (*cont==TAM1/2){
        return false;
    }
    return true;
}

bool recorrer_todos_eleme(void *dato, void *extra){
    int *cont=extra;
    *cont+=1;
    return true;
}

void prueba_lista_vacia(lista_t *lista_vacia){
    printf("\nPruebas de Lista Vacia\n");
    print_test("Comprobando primitiva esta_vacia",lista_esta_vacia(lista_vacia));
    print_test("Comprobando que la primitiva borrar_primero devuelva NULL",lista_borrar_primero(lista_vacia)==NULL);
    print_test("Comprobando que la primitiva ver_primero devuelva NULL",lista_ver_primero(lista_vacia)==NULL);
    print_test("Comprobando que la primitiva ver_ultimo devuelva NULL",lista_ver_ultimo(lista_vacia)==NULL);
    print_test("Comprobando que la primitiva largo devuelva 0", lista_largo(lista_vacia)==0);
    lista_iter_t *iter=lista_iter_crear(lista_vacia);
    print_test("Iterador externo creado",iter!=NULL);
    print_test("Comprobando que la primitiva ver_actual devuelva NULL",lista_iter_ver_actual(iter)==NULL);
    print_test("Comprobando que la primitiva borrar devuelva NULL",lista_iter_borrar(iter)==NULL);
    print_test("Comprobando primitiva avanzar",!lista_iter_avanzar(iter));
    print_test("Comprobando primitiva al_final",lista_iter_al_final(iter));
    lista_iter_destruir(iter);
    print_test("Iterador externo destruido",true);
}

void prueba_iterador_interno(lista_t *lista, int *datos){
    printf("\nPrueba del Iterador Interno\n");
    if (datos){
        lista_iterar(lista,multiplicar_2,NULL);
        for(int i=0; i<TAM1; i++){
            if (datos[i]!=i*2){
                print_test("Fallo el iterador interno",false);
                return;
            }
        }
        print_test("Iterado con el iterador interno sin corte",true);
    }else{
        int total=0;
        lista_iterar(lista,recorrer_todos_eleme,&total);
        print_test("Iterado con el iterador interno sin corte",total==TAM1);
    }
    int extra=0;
    lista_iterar(lista,recorrer_6_eleme,&extra);
    print_test("Iterado con el iterador interno con corte",extra==5);
}

void prueba_iterador_externo(lista_t *lista, char *elem_medio){
    printf("\nPrueba del Iterador Externo\n");
    lista_iter_t *iter=lista_iter_crear(lista);
    print_test("Iterador externo creado",iter);
    bool comprobacion;
    comprobacion=lista_iter_insertar(iter,NULL);
    print_test("Insertado de NULL al principio con el iterador externo",comprobacion && lista_ver_primero(lista)==NULL);
    void* dato=lista_iter_borrar(iter);
    print_test("Extraccion de NULL al principio con el iterador externo",dato==NULL);
    if (elem_medio){
        int cont=0;
        while (!lista_iter_al_final(iter)){
            if (cont==5){
                comprobacion=lista_iter_insertar(iter,elem_medio);
                print_test("Insertado de un elemento en medio de la iteracion", comprobacion);
            }
            cont++;
            comprobacion=lista_iter_avanzar(iter);
            if (!comprobacion) print_test("Fallo avanzar en el iterador", comprobacion);
        }
        comprobacion=lista_iter_insertar(iter,NULL);
        print_test("Insertado de NULL al final con el iterador externo",comprobacion && lista_ver_ultimo(lista)==NULL);
        dato=lista_iter_borrar(iter);
        print_test("Extraccion de NULL al final con el iterador externo",dato==NULL);
    }else{
        comprobacion=lista_insertar_primero(lista,NULL);
        print_test("Insertado de NULL al principio",comprobacion && lista_ver_primero(lista)==NULL);
        dato=lista_borrar_primero(lista);
        print_test("Extraccion de NULL al principio",dato==NULL);
        comprobacion=lista_insertar_ultimo(lista,NULL);
        print_test("Insertado de NULL al final",comprobacion && lista_ver_ultimo(lista)==NULL);
        for (int i=TAM2-1; i>-1; i--){
            dato=lista_iter_borrar(iter);
        }
        dato=lista_iter_borrar(iter);
        print_test("Extraccion de NULL con el iterador externo",dato==NULL);
        print_test("Vaciado de la lista",lista_esta_vacia(lista));
    }
    lista_iter_destruir(iter);
    print_test("Iterador externo destruido",true);
}

void prueba_10_elementos_estaticos(lista_t *elem_10){
    int datos[TAM1];
    bool comprobacion;
    for (int i = 0; i < TAM1; i++){
        datos[i]=i;
        comprobacion=lista_insertar_ultimo(elem_10,&datos[i]);
        if (!comprobacion || lista_ver_ultimo(elem_10)!=&datos[i]){
            print_test("Fallo insertar ultimo",false);
            return;
        }
    }
    print_test("10 elementos insertados a lo ultimo",lista_ver_ultimo(elem_10)==&datos[9] && lista_largo(elem_10)==10);
    prueba_iterador_interno(elem_10,datos);
    char *elem_medio="ola q ace";
    prueba_iterador_externo(elem_10,elem_medio);
    comprobacion=lista_insertar_primero(elem_10,NULL);
    print_test("Insertado de NULL al principio",comprobacion && lista_ver_primero(elem_10)==NULL);
    void *dato;
    dato=lista_borrar_primero(elem_10);
    print_test("Extraccion de NULL al principio",lista_ver_primero(elem_10)==&datos[0] && dato==NULL);
    lista_insertar_ultimo(elem_10,NULL);
    print_test("Insertado de NULL al final",lista_ver_ultimo(elem_10)==NULL);
    for(int i=0; i<5; i++){
        dato=lista_borrar_primero(elem_10);
        if (dato!=&datos[i]){
            print_test("Fallo vaciado de la mitad",false);
        }
    }
    dato=lista_borrar_primero(elem_10);
    print_test("Extraccion del elemento que se inserto en medio de la lista",dato==elem_medio);
}

void prueba_10000_elementos_estaticos(lista_t *elem_10000){
    int datos[TAM2];
    bool comprobacion;
    for (int i = 0; i < TAM2; i++){
        datos[i]=i;
        comprobacion=lista_insertar_primero(elem_10000,&datos[i]);
        if (!comprobacion || lista_ver_primero(elem_10000)!=&datos[i]){
            print_test("Fallo insertar ultimo",false);
            return;
        }
    }
    print_test("10000 elementos insertados en la primera posición",lista_ver_primero(elem_10000)==&datos[TAM2-1] && lista_largo(elem_10000)==10000);
    prueba_iterador_interno(elem_10000,datos);
    prueba_iterador_externo(elem_10000,NULL);
}

void prueba_lista_vacia_dinamica(lista_t *lista_vacia_d){
    prueba_lista_vacia(lista_vacia_d);
    lista_destruir(lista_vacia_d,free);
    print_test("Lista destruida",true);
}

void prueba_10_elementos_dinamicos(lista_t *elem_10){
    void** datos[TAM1];
    bool comprobacion;
    for (int i = 0; i < TAM1; i++){
        datos[i]=malloc(sizeof(void*));
        comprobacion=lista_insertar_ultimo(elem_10,datos[i]);
        if (!comprobacion || lista_ver_ultimo(elem_10)!=datos[i]){
            print_test("Fallo insertar ultimo",false);
            return;
        }
    }
    print_test("10 elementos insertados a lo ultimo",lista_ver_ultimo(elem_10)==datos[9]);
    prueba_iterador_interno(elem_10, NULL);
    char *elem_medio="bueno ciao";
    prueba_iterador_externo(elem_10,elem_medio);
    comprobacion=lista_insertar_primero(elem_10,NULL);
    print_test("Insertado de NULL al principio",comprobacion && lista_ver_primero(elem_10)==NULL);
    void *dato;
    dato=lista_borrar_primero(elem_10);
    print_test("Extraccion de NULL al principio",lista_ver_primero(elem_10)==datos[0] && dato==NULL);
    comprobacion=lista_insertar_ultimo(elem_10,NULL);
    print_test("Insertado de NULL al final",comprobacion && lista_ver_ultimo(elem_10)==NULL);
    for(int i=0; i<5; i++){
        dato=lista_borrar_primero(elem_10);
        if (dato!=datos[i]){
            print_test("Fallo vaciado de la mitad",false);
        }
        free(dato);
    }
    dato=lista_borrar_primero(elem_10);
    print_test("Extraccion del elemento que se inserto en medio de la lista",dato==elem_medio);
}

void pruebas_lista_alumno(void){
    printf("\nPRUEBAS CON MEMORIA ESTATICA\n");
    lista_t *lista_vacia=lista_crear();
    print_test("\n~1er Lista creada",lista_vacia!=NULL);
    prueba_lista_vacia(lista_vacia);
    lista_destruir(lista_vacia,NULL);
    print_test("Lista destruida",true);
    lista_t *elem_10=lista_crear();
    print_test("\n~2da Lista creada",elem_10!=NULL);
    prueba_10_elementos_estaticos(elem_10);
    lista_destruir(elem_10,NULL);
    print_test("Lista vaciada y destruida",true);
    lista_t *elem_10000=lista_crear();
    print_test("\n~3ra Lista creada",elem_10000);
    prueba_10000_elementos_estaticos(elem_10000);
    lista_destruir(elem_10000,NULL);
    print_test("Lista destruida",true);
    printf("\nPRUEBAS CON MEMORIA DINAMICA\n");
    lista_t *lista_vacia_d=lista_crear();
    print_test("\n~4ta Lista creada",lista_vacia_d!=NULL);
    prueba_lista_vacia_dinamica(lista_vacia_d);
    lista_t *elem_10_d=lista_crear();
    print_test("\n~5ta Lista creada",elem_10_d!=NULL);
    prueba_10_elementos_dinamicos(elem_10_d);
    lista_destruir(elem_10_d,free);
    print_test("Lista vaciada y destruida",true);
}