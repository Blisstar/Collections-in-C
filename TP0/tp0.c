#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap (int *x, int *y) {
    int dato=*x;
    *x=*y;
    *y=dato;
}

int maximo(int vector[], int n) {
    if (n>0){
        int pos_del_max=0;
        for (int i = 1; i < n; i++){
            if (vector[pos_del_max]<vector[i]){
                pos_del_max=i;
            }
        }
        return pos_del_max;
    }
    return -1;
}

int comparar(int vector1[], int n1, int vector2[], int n2) {
    int long_min=n1;
    if (n2<n1){
        long_min=n2;
    }
    for (int i = 0; i < long_min; i++){
        if (vector1[i]<vector2[i]){
            return -1;
        }
        if (vector1[i]>vector2[i]){
            return 1;
        }
    }
    if (n1<n2){
        return -1;
    }
    if (n1>n2){
        return 1;
    }
    return 0;
}

void seleccion(int vector[], int n) {
    for (int i = n; i > 0; i--){
        int pos_max=maximo(vector,i);
        swap(&vector[pos_max],&vector[i-1]);
    }
}
