//estado inicial
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 2000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_COMPOSTOS_V N_HABILIDADES * 3

//codigo dos eventos
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10

//status dos herois
#define MORTO 0
#define VIVO 1

//estrutura de local
struct local {
    int x;
    int y;
};

//estrutura do heroi
struct heroi {
    int id;
    struct cjto_t *habilidades;
    int paciencia;
    int velocidade;
    int experiencia;
    int base;
    int status;
};

//estrutura da base
struct base {
    int id; //id da base
    int lotacao; //numero maximo de herois na base
    struct cjto_t *presentes; //lista de herois presentes na base
    struct fila_t *espera; //fila de espera da base
    struct local coordenadas; // coordenadas da base
};

//estrutura da missao
struct missao {
    int id;
    struct cjto_t *habilidades_req;
    struct local coordenadas;
};

//estrutura do mundo
struct mundo {
    int n_herois; //numero total de herois
    struct heroi herois[N_HEROIS]; //vetor com todos os herois

    int n_bases;
    struct base bases[N_BASES];

    int n_missoes;
    struct missao missoes[N_MISSOES];

    int n_habilidades;
    struct cjto_t *habilidades;

    int n_compostos_v;

    int n_tamanho_mundo;

    int relogio;

    struct fprio_t *lef;
};

//estrutura do evento
struct evento {
    int tempo; //quando acontece o evento
    int tipo; //tipo do evento
    int info1; // guarda dados do evento (base, heroi, etc)
    int info2;  //guarda dados do evento
};


//retorna um inteiro aleatorio entre min e max
int aleat( int min, int max);

//calcula distancia entre os pontos 
int calcula_distancia(struct local a, struct local b);

//cria e inicializa estruturas um novo evento
struct evento *cria_evento(int tempo, int tipo, int info1, int info2);

//agenda um evento inserindo ele na lef
struct evento *agenda_evento(struct mundo *m, int tempo, int tipo, int info1, int info2);

//destroi e libera memoria de um evento
struct evento *destroi_evento(struct evento *ev);

//cria e inicializa as estruturas dos mundo
struct mundo cria_mundo ();

//cria o vetor de herois
struct heroi cria_heroi (struct mundo *m, int id);

//cria cada base
struct base cria_base(struct mundo *m, int id);

//cria cada missao
struct missao cria_missao(struct mundo *m, int id);

//crias os eventos iniciais e insere na lef
void eventos_iniciais(struct mundo *m);




