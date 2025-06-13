#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "mundo.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

//GERA UM INTEIRO ALEATORIO
int aleat(int min, int max) {
    return min + rand() % (max - min + 1);
}

//CALCULA DISTANCIA CARTESIANA ENTRE DOIS PONTOS
int calcula_distancia(struct local a, struct local b) {
    double cd;

    cd = pow((b.x - a.x), 2) + pow((b.y -a.y), 2);

    return (int)sqrt(cd);
}

//CRIA E INICIALIZA AS ESTRUTURAS DE UM NOVO EVENTO
struct evento *cria_evento(int tempo, int tipo, int info1, int info2) {
    struct evento *novo_evento = malloc(sizeof(struct evento));

    //inicializa os campos do evento
    novo_evento->tempo = tempo;
    novo_evento->tipo = tipo;
    novo_evento->info1 = info1;
    novo_evento->info2 = info2;

    return novo_evento;

}

//CRIA UM NOVO EVENTO USANDO A CRIA_EVENTO E INSERE ELE NA LEF(FPRIO)
struct evento *agenda_evento(struct mundo *m, int tempo, int tipo, int info1, int info2) {
    //caso de m ou lef invalidos
    if(m == NULL || m->lef == NULL) {
        printf("erro, mundo ou lef invalidos (agenda_evento)"); //printf apenas para testes
        return NULL;
    }
    
    //cria um novo evento
    struct evento *ev = cria_evento(tempo, tipo, info1, info2);
    if(ev == NULL) {
        printf("erro, falha ao criar evento (agenda_evento)"); //printf apenas para testes
    }

    //insere ele na lef
    if(fprio_insere(m->lef, ev, tipo, tempo) == -1) {
        printf("erro, falha ao inserir evento na lef (agenda_evento)"); //printf apenas para testes
        destroi_evento(ev);
    }
    return ev;
}

//DESTROI E LIBERA MEMORIA DE UM EVENTO
struct evento *destroi_evento(struct evento *ev) {
    if (ev != NULL) 
        free(ev);
    
    return NULL;
}

//CRIA E INICIALIZA A ESTRUTURAS DO MUNDO
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

    //cria e inicializa vetor de herois usando a cria_heroi
    //uso de vetores estáticos pois não serão alterados
    for(int i = 0; i < m.n_herois; i++)
        m.herois[i] = cria_heroi(i);

    //cria e inicializa o vetor de bases usando a cria_base
    for(int i = 0; i < m.n_bases; i++)
        m.bases[i] = cria_base(&m, i);
    
    //cria e inicializa o vetor de missoes usando a cria_missoes
    for(int i = 0; i < m.n_missoes; i++)
        m.missoes[i] = cria_missao(&m, i);

    //cria habilidades
    m.habilidades = cjto_cria(m.n_habilidades);
    //inicializa
    for(int i = 0; i < m.n_habilidades; i++)
        cjto_insere(m.habilidades, i);

    //cria a lista de eventos futuros usando a fprio_t e cria eventos inciais
    m.lef = fprio_cria();
    eventos_iniciais(&m);

    return m;
}

//destroi o mundo e todas as suas estruturas
void destroi_mundo(struct mundo *m) {
    //destroi conjunto de habilidades de cada heroi
    for(int i_h = 0; i_h < m->n_herois; i_h++)
        cjto_destroi(m->herois[i_h].habilidades); //aponta para mundo que tem herois e herois tem habilidades

    //destroi base
    for(int i_b = 0; i_b < m->n_bases; i_b++) {
        cjto_destroi(m->bases[i_b].presentes); //destroi presentes na base
        fila_destroi(m->bases[i_b].espera); //destroi a fila de espera
    }

    //destroi as habilidades de cada missao
    for(int i_m = 0; i_m < m->n_missoes; i_m++)
        cjto_destroi(m->missoes[i_m].habilidades_req);

    //destrois habilidades do mundo
    cjto_destroi(m->habilidades);
    //destroi lista de eventos futuros
    fprio_destroi(m->lef);
}


//cria cria cada heroi
struct heroi cria_heroi(int id) {

    struct heroi h;

    h.id = id;
    h.experiencia = 0;
    h.paciencia = aleat(0,100);
    h.velocidade = aleat(50,500);
    h.habilidades = cjto_aleat(aleat(1,3), N_HABILIDADES -1);
    //heroi inicializa sem base
    h.base = -1;
    h.status = VIVO;

    return h;
}

//cria cada base
struct base cria_base(struct mundo *m, int id) {

    struct base b;

    b.id = id;
    b.lotacao = aleat(3,10);
    b.presentes = cjto_cria(N_HEROIS - 1);
    b.espera = fila_cria();
    b.coordenadas.x = aleat(0, m->n_tamanho_mundo);
    b.coordenadas.y = aleat(0, m->n_tamanho_mundo);

    return b;
}

//cria cada missao
struct missao cria_missao(struct mundo *m, int id) {

    struct missao mi;

    mi.id = id;
    mi.habilidades_req = cjto_aleat(aleat(6,10), N_HABILIDADES -1);
    mi.tentativas = 0;
    mi.cumpridas = 0;

    mi.coordenadas.x = aleat(0,m->n_tamanho_mundo);
    mi.coordenadas.y = aleat(0,m->n_tamanho_mundo);

    return mi;
    
}

//crias os eventos iniciais e insere na lef
void eventos_iniciais(struct mundo *m) {
    int i, tempo, base;

    //cria evento para herois chegando em bases aleatórias
    for(i = 0; i < m->n_herois; i++) {
        base = aleat(0, m->n_bases -1);
        tempo = aleat(0, 4320); //primeiros 3 dias da simulacao
        agenda_evento(m, tempo, CHEGA, i, base); //infos aqui sao id heroi e id base
    }

    //cria evento para todas as missoes do mundo
    for(i = 0; i < m->n_missoes; i++) {
        tempo = aleat(0, T_FIM_DO_MUNDO);
        agenda_evento(m, tempo, MISSAO, i, -1);
    }

    //cria evento para o fim do mundo
    tempo = T_FIM_DO_MUNDO;
    agenda_evento(m, tempo, FIM, -1, -1);
}


//eventos da simulacao

//EVENTO DE CHEGADA DO HEROI NA BASE
//CRIA EVENTO ESPERA OU DESISTE
void chega(struct mundo *m, int tempo, int heroi, int base) {

    struct heroi h;
    struct base b;
    int presentes_b, lotacao_b, espera_b, fila_espera;

    //atribui as variaveis heroi e base passados como parametro nesse vento
    h = m->herois[heroi];
    b = m->bases[base];

    //quantos herois estao na base
    presentes_b = cjto_card(b.presentes);
    //tamanho da fila de espera
    fila_espera = fila_tamanho(b.espera);
    //lotação max da base
    lotacao_b = b.lotacao;

    //atualiza a abse do heroi
    m->herois[heroi].base = base;

    //se ha vagas na base e a fila de espera esta vazia
    if ((presentes_b < lotacao_b) && (fila_tamanho(b.espera) == 0))
        espera_b = 1;
    else
        espera_b = h.paciencia > (10 * fila_espera);

    //se esperar cria evento espera
    if (espera_b == 1) {
        agenda_evento(m, tempo, ESPERA, heroi, base); //info1 e info2 sao heroi e base
        printf("%6d: CHEGA HEROI %2d BASE %d ", tempo, heroi, base);
        printf("(%2d/%2d) ESPERA \n", presentes_b, lotacao_b);
    }
    //se não esperar cria evento desiste
    else {
        agenda_evento(m, tempo, DESISTE, heroi, base); //info1 e info2 sao heroi e base
        printf("%6d: CHEGA HEROI %2d BASE %d", tempo, heroi, base);
        printf("(%2d/%2d) DESISTE \n", presentes_b, lotacao_b);
    }    
}


//EVENTO CASO O HEROI DECIDA ESPERAR NA FILA DA BASE
//CRIA O EVENTO AVISA
//ERRO AO IKNSERIR NA FILA DA BASE
void espera(struct mundo *m, int tempo, int heroi, int base) {
    //alocar dinamicamente para evitar erro no free (destroi mundo)
    int *ptr_heroi = malloc(sizeof(int));
    *ptr_heroi = heroi;

    printf("%6d: ESPERA HEROI %2d BASE %d ", tempo, heroi, base);
    printf("(%2d)\n", fila_tamanho(m->bases[base].espera)); 

    //insere heroi na fila da base
    fila_insere(m->bases[base].espera, ptr_heroi);

    //cria evento avisa para o porteiro
    agenda_evento(m, tempo, AVISA, heroi, base);

}

//EVENTO QUE AVISA O PORTEIRO DA BASE
//SE INSERIR NA BASE, CRIA O EVENTO ENTRA
void avisa(struct mundo *m, int tempo, int heroi, int base) { 

    struct base b;
    int lotacao_b, presentes_b;

    //uso de um ponteiro auxliar para evitar vazamento da alocação de heroi na espera
    void *aux = NULL;
    
    //base do evento 
    b = m->bases[base];
    
    lotacao_b = b.lotacao; //lotacao maxima
    presentes_b = cjto_card(b.presentes); //quantos herois ha na base


    printf("%6d: AVISA PORTEIRO BASE %d ", tempo, base);
    printf("(%2d/%2d) FILA [ ", presentes_b, lotacao_b);
    fila_imprime(b.espera);
    printf("]\n");

    //enquanto a base tiver vagas 
    while (presentes_b <= lotacao_b && (fila_tamanho(b.espera) != 0)) {
        aux = fila_retira(b.espera); //tira da fila de espera
        if(aux)
            free(aux);
        cjto_insere(b.presentes, heroi);
        presentes_b ++;

        //cria evento entra
        agenda_evento(m, tempo, ENTRA, heroi, base);
        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, base, heroi);
    }
}


//EVENTO CASO O HEROI DESISTA  DE ESPERAR NA FILA DA BASE
//CRIA O EVENTO VIAJA
void desiste(struct mundo *m, int tempo, int heroi, int base) {

    int destino;
    
    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base);

    //escolhe uma base aleatoria para o heroi viajar
    destino = aleat(0, m->n_bases -1);

    //cria evento viaja
    agenda_evento(m, tempo, VIAJA, heroi, destino);
}


//HEROI EH ADMITIDO E ENTRA NA BASE
//CRIA O EVENTO SAI
void entra(struct mundo *m, int tempo, int heroi, int base) {

    struct base b;
    struct heroi h;
    int permanencia_b, presentes_b, lotacao_b;

    //base e heroi do evento
    b = m->bases[base];
    h = m->herois[heroi];

    presentes_b = cjto_card(b.presentes);
    lotacao_b = b.lotacao;

    permanencia_b = 15 + (h.paciencia * aleat(1, 20));

    //cria evento sai
    agenda_evento(m, (tempo + permanencia_b), SAI, heroi, base);

    printf("%6d: ENTRA HEROI %2d BASE %d ", tempo, heroi, base);
    printf("(%2d/%2d) SAI %d\n", presentes_b, lotacao_b, (tempo + permanencia_b));
}

//HEROI SAI DA BASE
//CRIA EVENTO VIAJA
//CRIA EVENTO AVISA
void sai(struct mundo *m, int tempo, int heroi, int base) {

    struct base b;
    int prox_base;

    b = m->bases[base];

    //tira heroi da base atual
    cjto_retira(b.presentes, heroi);
    //prox base para que o heroi irá viajar
    prox_base = aleat(0, m->n_bases-1);

    //cria evento viaja
    agenda_evento(m, tempo, VIAJA, heroi, prox_base);

    //cria evento avisa
    agenda_evento(m, tempo, AVISA, heroi, base);

    printf("%6d: SAI HEROI %2d BASE %d", tempo, heroi, base);
    printf(" (%2d/%2d)\n", cjto_card(b.presentes), b.lotacao); //cjtocard informa o numero de itens do conjunto

}


//ENCERRA A SIMULACAO DO MUNDO E IMPRIME AS ESTATISTICAS
//IMPRIME TODOS OS HEROIS E XP ADQUIRIDAS
//DEIXAR POR ULTIMO TBM
void fim(struct mundo *m) {

    struct heroi h;
    printf("%6d: FIM\n", m->relogio);

    for (int i = 0; i < m->n_herois; i++) {
        h = m->herois[i];
        printf("HEROI %d VIVO PAC %d VEL %4d EXP %4d HABS [ ", i, h.paciencia, h.velocidade, h.experiencia);
        cjto_imprime(h.habilidades);
        printf(" ]\n");
    }

    destroi_mundo(m);
}

//HEROI VIAJA PARA OUTRA BASE
//CRIA EVENTO CHEGA
void viaja(struct mundo *m, int tempo, int heroi, int base) {

    struct base b_atual, b_prox;
    struct heroi h;
    int duracao, distancia;

    //heroi do evento
    h = m->herois[heroi];
    //base atual do heroi
    b_atual = m->bases[h.base];
    //base que  heroi vai viajar
    b_prox = m->bases[base];          
    
    distancia = calcula_distancia(b_atual.coordenadas, b_prox.coordenadas);
    duracao = (distancia / h.velocidade);

    //cria evento chega
    agenda_evento(m, tempo + duracao, CHEGA, heroi, base);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d", tempo, heroi, h.base, base);
    printf("DIST %d VEL %d CHEGA %d\n", distancia, h.velocidade, tempo + duracao);
}

/*
//HEROI MORRE QUANDO USA O COMPOSTO V
//CRIA EVENTO AVISA???
void morre(struct mundo *m, int tempo, int heroi, int missao) {

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo, heroi, missao);
}

//VE SE UMA BASE ESTA APTA PARA MISSAO
//BASE MAIS PROXIMA TEM PRIORIDADE E SE TEM HABILIDADES NECESSARIAS
//SO PODE USAR COMPOSTO V SE T FOR MULT DE 2500 
//SE FOR USADO DECREMENTAR OS COMPOSROS V
//HEROIS GANHAM XP
//COMO CONTAR AS MISSOES CUMPRIDAS???
//COMO CONTAR AS TENTATIVAS??
//DEIXAR POR ULTIMO, CONFUSO
void missao(struct mundo *m, int tempo, int missao) {

}

*/
