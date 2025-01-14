#include <stdlib.h>

int *elem_dist_k(int *arreglo, size_t n, int k){
    int elem_k[] = NULL;
    size_t ind = 0;
    for (size_t i = 0; i < n; i++){
        for (size_t j = i; j < n; j++){
            if ((arreglo[i] == arreglo[j] + k) || (arreglo[i] == arreglo[j] - k)){
                elem_k[ind] = arreglo[j];
                elem_k[ind] = arreglo[i];
            }
        }
    }
    return elem_k;
}