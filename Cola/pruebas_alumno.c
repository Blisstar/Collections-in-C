#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define TAM1 10
#define TAM2 10000


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_cola_vacia(cola_t* cola){
    bool comprobacion=cola_esta_vacia(cola);
    print_test("Cola vacia",comprobacion);
    print_test("Viendo que el primer elemento de la cola sea NULL",cola_ver_primero(cola)==NULL);
}

void crear_datos_dinamicos(void*** datos,size_t tam){
    for (int i = 0; i < tam; i++){
        datos[i]=malloc(sizeof(int));
    }
}

void destruir_cola(cola_t* cola, bool interruptor){
    if (interruptor){
        cola_destruir(cola,NULL);
    }else{
        cola_destruir(cola,free);
    }
    print_test("Cola desencolada y destruida ",true);
}

void prueba_encolar(cola_t* cola, void*** datos_dina,int datos_esta[], size_t tam){
    bool comprobacion;
    for (int i = 0; i < tam; i++){
        if (datos_dina==NULL){
            datos_esta[i]=i;
            comprobacion=cola_encolar(cola,&datos_esta[i]);
        }else{
            comprobacion=cola_encolar(cola,datos_dina[i]);
        }
        if (!comprobacion){
            print_test("Fallo encolar",comprobacion);
        }
        if (i==(int)tam/2){
            print_test("Comprobando primitiva esta_vacia mientras encolo",!cola_esta_vacia(cola));
        }
    }
    if (datos_dina==NULL){
        print_test("Encolado de elementos en la cola", cola_ver_primero(cola)==&datos_esta[0] && !cola_esta_vacia(cola));
    }else{
        print_test("Encolado de elementos en la cola", cola_ver_primero(cola)==datos_dina[0] && !cola_esta_vacia(cola));
    }
    
}

void prueba_desencolar(cola_t* cola, void*** datos_dina,int datos_esta[], size_t tam){
    void* dato;
    for (int i = 0; i < tam; i++){
        dato=cola_desencolar(cola);
        if (datos_esta==NULL){ 
            free(dato);
        }
    }
    dato=cola_desencolar(cola);
    print_test("Desencolado completo de la cola", cola_esta_vacia(cola) && dato==NULL);
}

void prueba_de_NULL(cola_t* cola){
    cola_encolar(cola,NULL);
    print_test("Encolado de NULL",cola_ver_primero(cola)==NULL && !cola_esta_vacia(cola));
    void* dato=cola_desencolar(cola);
    print_test("Desencolado de NULL",dato==NULL);
}

void PRUEBAS_CON_MEMORIA_ESTATICA(void){
    printf("\nPRUEBAS CON MEMORIA ESTÁTICA\n");
    //COLA VACIA
    //
    cola_t* cola_prueba1=cola_crear();
    print_test("Cola creada",cola_prueba1!=NULL);
    prueba_cola_vacia(cola_prueba1);
    destruir_cola(cola_prueba1,true);
    //COLA DE 10 ELEMENTOS SIN DESENCOLAR AL FINAL
    //
    cola_t* cola_prueba2=cola_crear();
    print_test("2da cola creada", cola_prueba2!=NULL);
    prueba_cola_vacia(cola_prueba2);
    int datos[TAM2];
    prueba_encolar(cola_prueba2,NULL,datos,TAM1);
    destruir_cola(cola_prueba2,true);
    //COLA DE 10 ELEMENTOS DESENCOLANDO AL FINAL
    //
    cola_t* cola_prueba3=cola_crear();
    print_test("3ra cola creada",cola_prueba3!=NULL);
    prueba_de_NULL(cola_prueba3);
    prueba_encolar(cola_prueba3, NULL,datos, TAM1);
    prueba_desencolar(cola_prueba3, NULL,datos, TAM1);
    destruir_cola(cola_prueba3,true);
    //COLA DE 10000 ELEMENTOS SIN DESENCOLAR AL FINAL
    //
    cola_t* cola_prueba4=cola_crear();
    print_test("4ta cola creada", cola_prueba4!=NULL);
    prueba_encolar(cola_prueba4,NULL,datos,TAM2);
    destruir_cola(cola_prueba4,true);
    //COLA DE 10000 ELEMENTOS DESENCOLANDO AL FINAL
    //
    cola_t* cola_prueba5=cola_crear();
    print_test("5ta cola creada",cola_prueba5!=NULL);
    prueba_encolar(cola_prueba5, NULL,datos, TAM2);
    prueba_desencolar(cola_prueba5, NULL,datos, TAM2);
    destruir_cola(cola_prueba5,true);
}

void PRUEBAS_CON_MEMORIA_DINAMICA(void){
    printf("\nPRUEBAS CON MEMORIA DINÁMICA\n");
    //COLA VACIA
    //
    cola_t* cola_prueba6=cola_crear();
    print_test("6ta cola creada",cola_prueba6!=NULL);
    prueba_cola_vacia(cola_prueba6);
    destruir_cola(cola_prueba6,false);
    //COLA DE 10 ELEMENTOS SIN DESENCOLAR AL FINAL
    //
    cola_t* cola_prueba7=cola_crear();
    print_test("7ma cola creada", cola_prueba7!=NULL);
    prueba_cola_vacia(cola_prueba7);
    void** datos[TAM2];
    crear_datos_dinamicos(datos,TAM1);
    prueba_encolar(cola_prueba7,datos,NULL,TAM1);
    destruir_cola(cola_prueba7,false);
    //COLA DE 10 ELEMENTOS DESENCOLANDO AL FINAL
    //
    cola_t* cola_prueba_8=cola_crear();
    print_test("8va cola creada",cola_prueba_8!=NULL);
    prueba_de_NULL(cola_prueba_8);
    crear_datos_dinamicos(datos,TAM1);
    prueba_encolar(cola_prueba_8, datos,NULL, TAM1);
    prueba_desencolar(cola_prueba_8, datos,NULL, TAM1);
    destruir_cola(cola_prueba_8,false);
    //COLA DE 10000 ELEMENTOS SIN DESENCOLAR AL FINAL
    //
    cola_t* cola_prueba9=cola_crear();
    print_test("9na cola creada", cola_prueba9!=NULL);
    crear_datos_dinamicos(datos,TAM2);
    prueba_encolar(cola_prueba9,datos,NULL,TAM2);
    destruir_cola(cola_prueba9,false);
    //COLA DE 10000 ELEMENTOS DESENCOLANDO AL FINAL
    //
    cola_t* cola_prueba10=cola_crear();
    print_test("10ma cola creada",cola_prueba10!=NULL);
    crear_datos_dinamicos(datos,TAM2);
    prueba_encolar(cola_prueba10, datos, NULL, TAM2);
    prueba_desencolar(cola_prueba10, datos, NULL, TAM2);
    destruir_cola(cola_prueba10,false);
}

void pruebas_cola_alumno(void){
    PRUEBAS_CON_MEMORIA_ESTATICA();
    PRUEBAS_CON_MEMORIA_DINAMICA();
}
