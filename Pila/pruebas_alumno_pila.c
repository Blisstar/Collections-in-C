#include "pila.h"
#include "testing.h"
#include <stddef.h>
#define TAM1 10
#define TAM2 10000

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_pila_vacia(void){
    pila_t* pila_prueba1=pila_crear();
    print_test("Pila creada",pila_prueba1!=NULL);
    bool comprobacion=pila_esta_vacia(pila_prueba1);
    print_test("Pila vacia",comprobacion);
    print_test("Viendo que el tope de la pila sea NULL",pila_ver_tope(pila_prueba1)==NULL);
    pila_destruir(pila_prueba1);
    print_test("Pila destruida",true);
}

void prueba_apilar_desapilar_10(pila_t* pila){
    int datos[TAM1];
    bool comprobacion;
    for (int i = 0; i < TAM1; i++){
        datos[i]=i;
        comprobacion=pila_apilar(pila,&datos[i]);
        if (!comprobacion){
            print_test("Fallo apilar",comprobacion);
        }
        if (i==4){
            print_test("Viendo tope de una pila de 5 elementos",pila_ver_tope(pila)==&datos[i]);
            print_test("Comprobando primitiva esta_vacia mientras apilo",!pila_esta_vacia(pila));
        }
    }
    print_test("Apilado 10 elementos en la 2da pila", pila_ver_tope(pila)==&datos[9]);  
    void* dato;
    for (int i = 9; i > -1; i--){
        dato=pila_desapilar(pila);
        if (dato!=&datos[i]){
            print_test("Fallo desapilar",false);
        }
    }
    dato=pila_desapilar(pila);
    print_test("Desapilado completo de 2da pila", pila_esta_vacia(pila) && dato==NULL);
}

void prueba_apilar_desapilar_10000(pila_t* pila){
    int datos[TAM2];
    bool comprobacion;
    for (int i = 0; i < TAM2; i++){
        datos[i]=i;
        comprobacion=pila_apilar(pila,&datos[i]);
        if (!comprobacion){
            print_test("Fallo apilar",comprobacion);
        }
        if (i==4999){
            print_test("Viendo tope de una pila de 5000 elementos",pila_ver_tope(pila)==&datos[i]);
            print_test("Comprobando primitiva esta_vacia mientras apilo",!pila_esta_vacia(pila));
        }
    }
    print_test("Apilado 10000 elementos en la 2da pila", pila_ver_tope(pila)==&datos[9999]);
    void* dato;
    for (int i = 9999; i > -1; i--){
        dato=pila_desapilar(pila);
        if (dato!=&datos[i]){
            print_test("Fallo desapilar",false);
        }
    }
    dato=pila_desapilar(pila);
    print_test("Desapilado completo de 3ra pila", pila_esta_vacia(pila) && dato==NULL && pila_ver_tope(pila)==NULL);
}

void prueba_null(pila_t* pila){
    pila_apilar(pila,NULL);
    print_test("Apilado de NULL",pila_ver_tope(pila)==NULL && !pila_esta_vacia(pila));
    void* dato=pila_desapilar(pila);
    print_test("Desapilado de NULL",dato==NULL);
}


void pruebas_pila_alumno() {
    pila_t* ejemplo = NULL;
    print_test("Puntero inicializado a NULL", ejemplo == NULL);
    prueba_pila_vacia();
    pila_t* pila_prueba2=pila_crear();
    print_test("2da pila creada", pila_prueba2!=NULL);
    prueba_null(pila_prueba2);
    prueba_apilar_desapilar_10(pila_prueba2);
    pila_destruir(pila_prueba2);
    print_test("2da pila destruida",true);
    pila_t* pila_prueba3=pila_crear();
    print_test("3ra pila creada",pila_prueba3!=NULL);
    prueba_apilar_desapilar_10000(pila_prueba3);
    prueba_null(pila_prueba3);
    pila_destruir(pila_prueba3);
    print_test("3ra pila destruida",true);
}
