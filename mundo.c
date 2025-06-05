#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

//retorna um inteiro aleatorio entre min e max
int aleat( int min, int max) {
    return min + rand() % (max - min + 1);
}

//calcula distancia entre os pontos 
int calcula_distancia(struct local a, struct local b) {
    double cd;

    cd = pow((b.x - a.x), 2) + pow((b.y -a.y), 2);

    return (int)sqrt(cd);
}



struct mundo cria_mundo () { //preciso alocar memoria aqui??
    

}