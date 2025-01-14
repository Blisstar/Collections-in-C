#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "strutil.h"

char *substr(const char *str, size_t n){
    size_t len=strlen(str);
    if (n>len) n=len;
    char *resul=malloc(sizeof(char)*(n+1));
    if (!resul) return NULL;
    for (size_t i = 0; i < n; i++){
        resul[i]=*(&str[i]);
    }
    resul[n]='\0';
    return resul;
}

/*Recibe una cadena y un separador
 *Cuenta la cantidad de cadenas delimitadas por el separador
 *y devuelve tal cantidad
 */
size_t dar_cant_cadenas(const char *str, char sep){
    size_t len=2;
    size_t ind1=0;
    while (str[ind1]!='\0'){
        if (str[ind1]==sep) len++;
        ind1++;
    }
    return len;
}

char **split(const char *str, char sep){
    size_t len=dar_cant_cadenas(str, sep);
    char **resul=malloc(sizeof(char*)*(len));
    if (!resul) return NULL;
    resul[len-1]=NULL;
    len=0;
    size_t ind=0;
    int cont=0;
    char *aux;
    while (true){
        if (str[len]==sep || str[len]=='\0'){
            aux=substr(str+(len-cont),cont);
            if (!aux) return NULL;
            resul[ind]=aux;
            ind++;
            cont=-1;
            if (str[len]=='\0') break;
        }
        cont++;
        len++;
    }
    return resul;
}

/*Recibe un arreglo de cadenas con un NULL al final y un separador
 *Va sumando la longitud de cada cadena más la cantidad 
 *divisiones que tiene el arreglo, esto ultimo mientras
 *el separador no sea \0.
 *Finalmente, devuelve la suma total.
 */
size_t dar_sumatoria_len_cadenas(char **strv, char sep){
    char *act=strv[0];
    size_t len=0;
    size_t ind=0;
    while (act){
        len+=strlen(act);
        if (sep!='\0') len++;
        ind++;
        act=strv[ind];
    }
    if (len==0) len++;
    return len;
}

char *join(char **strv, char sep){
    size_t len=dar_sumatoria_len_cadenas(strv, sep);
    char *resul=malloc(sizeof(char)*(len+1));
    if (!resul) return NULL;
    char *act=strv[0];
    char *caracter=&act[0];
    size_t ind=0;
    size_t cont=0;
    len=0;
    while (act){
        while (*caracter!='\0'){   //Si utilizo strcpy o strcat me lanza error de memoria, intente hacer todas las funciones
            resul[len]=*caracter;  //utilizando primero funciones de string.h y hasta en el substr me andaba mal
            len++;                 //no veia donde fallaba asi que finalmente opte por ir iterando en las cadenas yo mismo
            cont++;                //El error consiste en que cuando utilizaba el strcpy,strcat o sus derivados    
            caracter=&act[cont];   //se perdia la referencia de la memoria pedida y a la hora de liberar fallaba o
        }                          //o creo que ni llegaba a liberar ya que se cortaba ahi.
        cont=0;
        ind++;
        act=strv[ind];
        caracter=&act[cont];
        if (sep!='\0'){
            resul[len]=sep;
            len++;
        }
    }
    if (sep=='\0') len++;
    if (len>0) len--;
    resul[len]='\0';
    return resul;
}

void free_strv(char *strv[]){
    int ind=0;
    char *act=strv[ind];
    while (act){
        free(act);
        ind++;
        act=strv[ind];
    }
    free(act);
    free(strv);
}