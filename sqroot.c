//Dado un número, encuentra por División y Conquista 
//la parte entera de su raíz cuadrada.
int raiz_cuadrada(int base){
    if (base == 0) return 0;
    if (base < 4) return 1;
    int base2 = (int)(base / 4);
    return raiz_cuadrada(base2) * 2 + 1;
}