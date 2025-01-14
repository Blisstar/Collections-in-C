#define _GNU_SOURCE
#include "strutil.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Recibe dos enteros, uno como
 *base de una potencia y otro 
 *como el exponente, y devuelve
 *la potencia resultante.
 *Funcion extraida del drive de la Catedra.
 */
int pot(int base, int exponente) {
	if (exponente < 1) return 1;
	if (exponente % 2 == 0) {
		int a = pot(base, exponente/2);
		return a * a;
	} else {
		int a = pot(base, (exponente-1)/2);
		return base * a * a;
	}
}

/*Dado dos números recibidos como base
 *y argumento de un logaritmo, se devuelve 
 *su logaritmo entero.
 */ 
int loga(double argumento, int base){
    int caracteristica=0;
    while(argumento >= base){
        argumento /= base;
        caracteristica++;
    }
    return caracteristica;
}

/*Recibe una pila, 2 numeros en formato char
 *y un char que determinara que operación se tendra
 *que realizar entre los dos números, el resultado final se apilara
 *en la pila como char. Si ocurre algún error se apila NULL.
 *Ademas recibe otra pila para apilar lo que se pidio de memoria.
 *Pre: la pila fue creada.
 */ 
void res_de_2operaciones(pila_t *pila, pila_t *pila_dinamica, const char *valor2, const char *valor1, const char *operacion){
    if (!valor1 || !valor2 || !operacion) {
        pila_apilar(pila, NULL);
        return;
    }
    int n1=atoi(valor1);
    int n2=atoi(valor2);
    char *resul=malloc(sizeof(char)*12);
    pila_apilar(pila_dinamica, resul);
    if (strcmp(operacion, "+") == 0) sprintf(resul, "%d", n1+n2);

    else if(strcmp(operacion, "-") == 0) sprintf(resul, "%d", n1-n2);

    else if(strcmp(operacion, "*") == 0) sprintf(resul, "%d", n1*n2);

    else if(strcmp(operacion, "/") == 0 && n2 != 0) sprintf(resul, "%d", (int)(n1 / n2));

    else if(strcmp(operacion, "^") == 0 && n2 >= 0) sprintf(resul, "%d", pot(n1, n2));

    else if(strcmp(operacion, "log") == 0 && n2 > 1) sprintf(resul, "%d", loga((double)n1, n2));

    else{
        pila_apilar(pila,NULL);
        return;
    }

    pila_apilar(pila, resul);
}

/*Dada un número en formato char y una pila
 *Calcula la raíz cuadrada del numero y apila
 *el resultado en la pila en formato char.
 *Si ocurre algún error se apila NULL.
 *Ademas recibe otra pila para apilar lo que se pidio de memoria.
 */
void sqroot(pila_t *pila, pila_t *pila_dinamica, const char *base){
    if (!base){
        pila_apilar(pila, NULL);
        return;
    }
    int radicando=atoi(base);
    if (radicando < 0){
        pila_apilar(pila, NULL);
        return;
    }
    double i=0;
    double x1,x2;
    while( (i*i) <= radicando ) i+=0.1;
    x1=i;
    for(int j=0;j<10;j++){
        x2=radicando;
        x2/=x1;
        x2+=x1;
        x2/=2;
        x1=x2;
    }
    char *resul=malloc(sizeof(char)*12);
    sprintf(resul, "%d", (int)x2);
    pila_apilar(pila, resul);
    pila_apilar(pila_dinamica, resul);
}

/*Recibiendo una pila y tres números en formato char
 *determina el ternario de los tres valores y lo apila.
 *Si ocurre algún error se apila NULL.
 */
void sacar_ternario(pila_t *pila, char *valor3, char *valor2, const char *valor1){
    if (!valor1 || !valor2 || !valor3){
        pila_apilar(pila, NULL);
        return;
    }
    if (strcmp(valor1, "0") == 0) pila_apilar(pila, valor3);
    else pila_apilar(pila, valor2);
}

/*Recibe un arreglo de chars e indica
 *si la cadena es un número entero o no.
 *Devolviendo true o false dependiendo del caso.
 */
bool es_digito(const char* n){
    size_t i=0;
    char act=n[i];
    if (act=='\0') return false;

    if (act=='-'){
        i++;
        act=n[i];
        if (act=='\0') return false;
    }
    while (act != '\0'){
        if (isdigit(act) == 0) return false;
        i++;
        act=n[i];
    }
    return true;
}

/*Dada un arreglo de cadenas y una pila
 *se apila las cadenas que se consideren números.
 */
void apilar_numeros(pila_t *pila, char** numeros, size_t *ind){
    while (numeros[*ind]){
        if (es_digito(numeros[*ind])) pila_apilar(pila, numeros[*ind]);
        else break;
        *ind= *ind + 1;
    }
}

/*Dada una cadena se pide memoria para una copia
 *de la misma, si hay un "/n" al final se descarta.
 */
char *sin_enter(char *linea, ssize_t len_linea){
    if (linea[len_linea-1]=='\n') return substr(linea, len_linea-1);
    return substr(linea, len_linea);
}

/*Implementacion de una calculadora mediante lectura de archivos
 */
int calculadora(void){
    FILE *archivo=stdin; 
    pila_t *pila=pila_crear();
    pila_t *pila_dinamica=pila_crear();
    if (!archivo || !pila || !pila_dinamica){
        if (pila) pila_destruir(pila);
        if (pila_dinamica) pila_destruir(pila_dinamica);
        printf("Surgió un error.\n");
        return -1;
    }
    char *linea=NULL;
    char *linea_aux="";
    size_t tam=0;
    size_t ind_operaciones=0;
    ssize_t len=getline(&linea, &tam, archivo);
    char **numeros_operaciones;
    char *act="";
    char *resul="";
    while (len >= 0){
        linea_aux=sin_enter(linea, len);
        numeros_operaciones=split(linea_aux, ' ');
        apilar_numeros(pila, numeros_operaciones, &ind_operaciones);
        act=numeros_operaciones[ind_operaciones];
        while (act){
            if (strcmp(act, "") == 0){
                ind_operaciones++;
                act=numeros_operaciones[ind_operaciones];
                continue;
            }
            if (es_digito(act)) {
                apilar_numeros(pila, numeros_operaciones, &ind_operaciones);
                act=numeros_operaciones[ind_operaciones];
                continue;
            }
            if (strcmp(act, "sqrt") == 0) sqroot(pila, pila_dinamica, pila_desapilar(pila));

            else if(strcmp(act, "?") == 0) sacar_ternario(pila, pila_desapilar(pila),pila_desapilar(pila),pila_desapilar(pila));

            else res_de_2operaciones(pila, pila_dinamica, pila_desapilar(pila), pila_desapilar(pila), act);

            if (!pila_ver_tope(pila)) break;
            ind_operaciones++;
            act=numeros_operaciones[ind_operaciones];
        }
        resul=pila_desapilar(pila);
        if (resul && pila_esta_vacia(pila)) printf("%s\n", resul);
        
        else fprintf(stdout, "ERROR\n");

        while (!pila_esta_vacia(pila)) pila_desapilar(pila);
        
        while (!pila_esta_vacia(pila_dinamica)) free(pila_desapilar(pila_dinamica));
        
        free(linea_aux);
        free_strv(numeros_operaciones);
        ind_operaciones=0;
        len = getline(&linea, &tam, archivo);
    }
    free(linea);
    pila_destruir(pila);
    pila_destruir(pila_dinamica);
    fclose(archivo);
    return 0;
}

int main(void){
    return calculadora();
}
