#ifndef PLACAR_H
#define PLACAR_H

/*
 * ============================================================
 *  PLACAR - Lista simplesmente encadeada (Atividade Avaliativa 2)
 * ============================================================
 *
 *  Cada nó guarda o resultado de UMA partida concluída:
 *    - nome do jogador
 *    - quantidade de passos dados até a saída
 *    - quantidade de vezes que a dica (resposta) foi usada
 *
 *  Os nós são alocados com malloc e ligados por ponteiros
 *  (inicio -> ... -> fim). Não há vetor de tamanho fixo
 *  guardando as partidas.
 */

#define TAM_NOME 50

typedef struct No {
    char nome[TAM_NOME];
    int passos;
    int dicas;
    struct No *prox;
} No;

typedef struct {
    No *inicio;            /* primeiro registro (mais antigo)          */
    No *fim;               /* último registro (inserção em O(1))       */
    int totalRegistros;    /* quantidade de nós na lista               */
    int jogosIniciados;    /* quantidade de jogos feitos               */
    int jogosConcluidos;   /* jogos em que o jogador chegou na saída   */
    int totalDicas;        /* quantidade de vezes que a resposta foi dada */
} Placar;

void placarIniciar(Placar *p);

/* Insere um novo registro no final da lista. Retorna 1 se OK, 0 se faltou memória. */
int  placarRegistrar(Placar *p, const char *nome, int passos, int dicas);

/* Percorre a lista na ordem em que as partidas foram jogadas. */
void placarListar(const Placar *p);

/* Mostra o ranking ordenado (menos passos primeiro; empate: menos dicas). */
void placarRanking(const Placar *p);

/* Mostra os contadores gerais. */
void placarEstatisticas(const Placar *p);

/* Libera todos os nós da lista. */
void placarLiberar(Placar *p);

#endif
