#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "fprio.h"
#include "mundo.h"
#include "conjunto.h"

// seus #defines vão aqui
// programa principal
int main () {
  //iniciar o mundo
  struct mundo m;
  struct evento *ev;
  int fim_simulacao;
  int tempo_atual = 0; 
  int tipo_atual = 0;

  srand(0);

  m = cria_mundo();

  fim_simulacao = 0;

  while (!fim_simulacao && (ev = fprio_retira(m.lef, &tipo_atual, &tempo_atual))) { //como faz a fprio retira funcionar???
      m.relogio = ev->tempo;
      switch (ev->tipo) {

          case CHEGA:
              chega(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case ESPERA:
              espera(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case DESISTE:
              desiste(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case AVISA:
              avisa(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case ENTRA:
              entra(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case FIM:
              fim(&m);
              fim_simulacao = 1;
              break;         
      }
      destroi_evento(ev);
  }
  return 1;
}

