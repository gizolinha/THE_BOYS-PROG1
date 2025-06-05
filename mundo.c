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
struct evento *cria_evento(int tempo, int tipo, int info1, int info2) {
    struct evento *novo_evento = malloc(sizeof(struct evento));

    //inicializa os campos do evento
    novo_evento->tempo = tempo;
    novo_evento->tipo = tipo;
    novo_evento->info1 = info1;
    novo_evento->info2 = info2;

    return novo_evento;

}

// cria um evento usando a cria_evento e agenda inserindo ele na lef (fprio)
void agenda_evento(struct mundo *m, int tempo, int tipo, int info1, int info2) {
    //caso de m ou lef invalidos
    if (m == NULL || m->lef == NULL) {
        printf("erro, mundo ou lef invalidos (agenda_evento)"); //printf apenas para testes
        return;
    }
    
    //cria um novo evento
    struct evento *ev = cria_evento(tempo, tipo, info1, info2);
    if (ev == NULL) {
        printf("erro, falha ao criar evento (agenda_evento)"); //printf apenas para testes
        return;
    }

    //insere ele na lef
    if (insere_fprio(m->lef, ev, tipo, tempo) != 0)
        printf("erro, falha ao inserir evento na lef (agenda_evento)"); //printf apenas para testes
        destroi_evento(ev);
}

//destroi e libera memoria de um evento
void destroi_evento(struct evento *ev) {
    //caso ev for nulo
    if (ev == NULL)
        return NULL;

    free(ev);
}

//cria e inicializa as estruturas dos mundo
struct mundo cria_mundo() {
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

    //cria a lista de eventos futuros usando a fprio_t e cria eventos inciais
    m.lef = fprio_cria;
    ev_iniciais(&m);

    return m;
}

//destroi o mundo e todas as suas estruturas
void destroi_mundo(struct mundo *m) {
    //destroi conjunto de habilidades de cada heroi
    for (int i_h = 0; i_h < m->n_herois; i_h++)
        cjto_destroi(m->herois[i_h].habilidades); //aponta para mundo que tem herois e herois tem habilidades

    //destroi base
    for (int i_b = 0; i_b < m->n_bases; i_b++) {
        cjto_destroi(w->bases[i_b].presentes); //destroi presentes na base
        fila_destroi(&m->bases[i_b].espera); //destroi a fila de espera
    }

    //destroi as habilidades de cada missao
    for (int i_m = 0; i_m < m->n_missoes; i_m++) 
        cjto_destroi(w->missoes[i_m].habilidades_req);

    //destrois habilidades do mundo
    cjto_destroi(w->habilidades);
    //destroi lista de eventos futuros
    cjto_destroi(w->lef);
}

//cria o vetor de herois
struct heroi cria_heroi(struct mundo *m, int id) {

    heroi.id = id;
    heroi.experiencia = 0;
    heroi.paciencia = aleat(0,100);
    heroi.velocidade = aleat(50,500);
    heroi.habilidades = cjt_cria(N_HABILIDADES);
    //sorteia as habilidades e insere no conjunto de habilidades do heroi
    for(int i = 0; i < aleat(1,3); i++)
        cjto_insere(heroi.habilidades, aleat(0, N_HABILIDADES -1));
    //heroi inicializa sem base
    heroi.base = -1;
    heroi.status = VIVO;

    return heroi;
}

//cria o vetor de bases
struct base cria_base(struct mundo *m, int id) {

    base.id = id;
    base.lotacao = aleat(3,10);
    base.presentes = cjt_cria(base.lotacao);
    base.espera = fila_cria();
    base.coordenadas.x = aleat(0, m->n_tamanho_mundo);
    base.coordenadas.y = aleat(0, m->n_tamanho_mundo);

    return base;
}

//cria o vetor de missoes
struct missao cria_missao(struct mundo *m, int id) {
    //testar interior da funcao depois
    missao.id = id;
    int num_hab = aleat(6,10);
    missao.habilidades_req = cjt_cria(N_HABILIDADES);
    for (int i = 0; i < num_hab; i++)
        cjto_insere(missao.habilidades_req, aleat(0, num_hab));
    
    missao.coordenadas.x = aleat(0, m->n_tamanho_mundo);
    missao.coordenadas.y = aleat(0, m->n_tamanho_mundo);
    
}

//crias os eventos iniciais e insere na lef
void eventos_iniciais(struct mundo *m) {
    struct evento *ev;

    //cria evento para herois chegando em bases aleatórias
    for (int i = 0; i < m->n_herois; i++) {
        int base = aleat(0, m->bases -1);
        tempo = aleat(0, 4320); //primeiros 3 dias da simulacao
        ev = agenda_evento(&m, tempo, CHEGA, i, base) //infos aqui sao id heroi e id base
    }

    //cria evento para todas as missoes do mundo
    for (int i = 0; i < m->n_missoes; i++) {
        tempo = aleat(0, T_FIM_DO_MUNDO);
        ev = agenda_evento(&m, MISSAO, i, -1);
    }

    //cria evento para o fim do mundo
    tempo = T_FIM_DO_MUNDO;
    ev = agenda_evento(&m, FIM, -1, -1);
}

//eventos da simulacao

//evento de chegada do heroi na base
void chega(struct mundo *m, int tempo, int heroi, int base) {


    printf("%6d: CHEGA HEROI %2d BASE %d", tempo, heroi, base);
    printf("(%2d/%2d) ESPERA \n", presentes, lotacao);

    printf("%6d: CHEGA HEROI %2d BASE %d", tempo, heroi, base);
    printf("(%2d/%2d) DESISTE \n", presentes, lotacao);
}

void espera(struct mundo *m, int tempo, int heroi, int base) {

    printf("%6d: ESPERA HEROI %2d BASE %d", tempo, heroi, base);
    printf("(%2d)\n", fila_tamanho(w->bases[base].espera)); //REVER???????????????????

}

void desiste(struct mundo *m, int tempo, inte heroi, int base) {

    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base);

}

void avisa(struct mundo *m, int tempo, int base) {

    printf("%6d: AVISA PORTEIRO BASE %d ", tempo, base);
    printf("(%2d/%2d) FILA ", presentes, lotacao);

    printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, base, heroi);
}
