#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
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

//cria e inicializa estruturas um novo evento
struct evento cria_evento(int tempo, int tipo, int info1, int info2) {
    struct evento *novo_evento = malloc(sizeof(struct evento));

    //inicializa os campos do evento
    novo_evento->tempo = tempo;
    novo_evento->tipo = tipo;
    novo_evento->info1 = info1;
    novo_evento->info2 = info2;

    return novo_evento;

}

//destroi e libera memoria de um evento
struct evento destroi_evento(struct evento *ev) {
    //caso ev for nulo
    if (ev == NULL)
        return NULL;

    free(ev);
}


struct mundo cria_mundo () { //preciso alocar memoria aqui??
    struct mundo m;

    //inicializa os dados do mundo
    m.n_herois = N_HEROIS;
    m.n_bases = N_BASES;
    m.n_missoes = N_MISSOES;
    m.n_habilidades = N_HABILIDADES;
    m.n_compostos_v = N_COMPOSTOS_V; 
    m.n_tamanho_mundo = N_TAMANHO_MUNDO;
    m.relogio = T_INICIO;

    //cria e inicializa vetor de herois
    for (int i = 0; i < m.n_herois)
        m.herois[i] = cria_heroi(&m, i);

    //cria e inicializa o vetor de bases
    for (int i = 0; i < m.n_bases; i++);
        m.bases[i] = cria_base(&m, i);
    
    //cria e inicializa o vetor de missoes
    for (int i = 0; i < m.n_missoes; i++);
        m.missoes[i] = cria_missao(&m, i);

     //cria habilidades
    m.habilidades = cjt_cria(m.n_habilidades);
    //inicializa
    for (int i = 0; i < m.n_habilidades; i++)
        cjto_insere(m.habilidades, i);

    //cria e inicializa a quantidade de compostos v disponíveis
    //COMO INICIALIZAR OS COMPOSTOS V???


    //cria a lista de eventos futuros usando a fprio_t e cria eventos inciais
    m.lef = fprio_cria;
    ev_iniciais(&m);

    return m;
}

struct heroi cria_heroi (struct mundo *m, int id) {
    heroi.id = id;
    heroi.experiencia = 0;
    heroi.paciencia = aleat(0,100);
    heroi.velocidade = aleat(50,500);
    heroi.habilidades = cjt_cria(N_HABILIDADES);
    //sorteia as habilidades e insere no conjunto de habilidades do heroi
    for(int i = 0; i < aleat(1,3); i++)
        cjto_insere(heroi.habilidades, aleat(0, N_HABILIDADES -1));
    //heroi inicializa sem base
    heroi.base = -1 ;

    return heroi;
}

struct base cria_base (struct mundo *m, int id) {

}


struct missao cria_missao (struct mundo *m, int id) {
    
}