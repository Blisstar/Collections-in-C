#include <stdlib.h>
#include <stdio.h>

int _encontrar_faltante(const int arreglo_a[], const int arreglo_b[], int inicio, int fin){
    if (inicio > fin) fin++;
    if (inicio == fin) return arreglo_a[inicio];
    int medio = (int)((inicio + fin) / 2);
    if (arreglo_a[medio] != arreglo_b[medio]){
        if (arreglo_a[medio-1] == arreglo_b[medio-1]) return arreglo_a[medio];
        return _encontrar_faltante(arreglo_a, arreglo_b, inicio, medio-1);
    }
    return _encontrar_faltante(arreglo_a, arreglo_b, medio+1, fin);
}

/*Dados dos arreglos ordenados y sin repetidos A y B, 
 *donde B es igual a A pero con un elemento menos, 
 *permite encontrar el elemento faltante de A en B. 
 *Ejemplo: A = {2, 4, 6, 8, 9, 10, 12}, B = {2, 4, 6, 8, 10, 12}. 
 *La salida del algoritmo debe ser el valor 9.
 */
int encontrar_faltante(const int arreglo_a[], const int arreglo_b[], int tam_a){
    if (arreglo_a[tam_a-1] != arreglo_b[tam_a-2]) return arreglo_a[tam_a-1];
    return _encontrar_faltante(arreglo_a, arreglo_b, 0, tam_a-2);
}
int main(){
    int arreglo_a[7] = {2, 4, 6, 8, 9, 10, 12};
    int arreglo_b[6] = {2, 4, 6, 8, 10, 12};
    int faltante = encontrar_faltante(arreglo_a, arreglo_b, 7);
    printf("el faltante es el %d \n", faltante);
    return 0;
}
