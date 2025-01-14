#include <stdbool.h>
#include <stdio.h>

bool _es_magico(const int enteros[], int inicio, int final){
    if (inicio > final) final++;
    if (final == inicio){
        if (enteros[final] == final) return true;
        return false;
    }
    int medio = (int)((inicio+final)/2);
    if (enteros[medio] == medio) return true;
    bool primera_mitad = _es_magico(enteros, inicio, medio-1);
    bool segunda_mitad = _es_magico(enteros, medio+1, final);
    if (primera_mitad == true || segunda_mitad == true) return true;
    return false;
}

//Recibe un arreglo de n enteros sin repetir y ordenados ascendentemente,
//y determina si es mágico. Un arreglo es mágico si existe algún valor i 
//(entre 0 y n − 1) tal que arr[i] = i.
bool es_magico(const int enteros[], int n){
    return _es_magico(enteros, 0, n-1);
}

int main(void){
    int enteros[7] = {-3, -1, 3, 4, 5, 7, 6};
    bool resultado = es_magico(enteros, 7);
    if (resultado) printf("El arreglo es magico.\n");
    else printf("El arreglo no es magico.\n");
    return 0;
}