#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "placar.h"

/* ---------- Funções auxiliares (internas) ---------- */

/* Cria um nó novo na memória dinâmica. Retorna NULL se malloc falhar. */
static No *criarNo(const char *nome, int passos, int dicas) {
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) return NULL;

    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    novo->passos = passos;
    novo->dicas = dicas;
    novo->prox = NULL;
    return novo;
}

/* Critério de ordenação: menos passos é melhor; empate -> menos dicas. */
static int melhorQue(const No *a, const No *b) {
    if (a->passos != b->passos) return a->passos < b->passos;
    return a->dicas < b->dicas;
}

/* Insere 'novo' em uma lista já ordenada, mantendo a ordem (inserção ordenada). */
static void inserirOrdenado(No **cabeca, No *novo) {
    if (*cabeca == NULL || melhorQue(novo, *cabeca)) {
        novo->prox = *cabeca;
        *cabeca = novo;
        return;
    }
    No *atual = *cabeca;
    while (atual->prox != NULL && !melhorQue(novo, atual->prox)) {
        atual = atual->prox;
    }
    novo->prox = atual->prox;
    atual->prox = novo;
}

static void liberarLista(No *cabeca) {
    while (cabeca != NULL) {
        No *proximo = cabeca->prox;
        free(cabeca);
        cabeca = proximo;
    }
}

/* ---------- Operações públicas ---------- */

void placarIniciar(Placar *p) {
    p->inicio = NULL;
    p->fim = NULL;
    p->totalRegistros = 0;
    p->jogosIniciados = 0;
    p->jogosConcluidos = 0;
    p->totalDicas = 0;
}

int placarRegistrar(Placar *p, const char *nome, int passos, int dicas) {
    No *novo = criarNo(nome, passos, dicas);
    if (novo == NULL) return 0;

    if (p->inicio == NULL) {        /* lista vazia */
        p->inicio = novo;
    } else {                        /* encadeia depois do último */
        p->fim->prox = novo;
    }
    p->fim = novo;
    p->totalRegistros++;
    return 1;
}

void placarListar(const Placar *p) {
    printf("\n===== HISTORICO DE PARTIDAS (ordem cronologica) =====\n");
    if (p->inicio == NULL) {
        printf("Nenhuma partida concluida ainda.\n");
        return;
    }

    printf("%-4s %-20s %8s %8s\n", "No", "Jogador", "Passos", "Dicas");
    int i = 1;
    for (const No *atual = p->inicio; atual != NULL; atual = atual->prox) {
        printf("%-4d %-20s %8d %8d\n", i++, atual->nome, atual->passos, atual->dicas);
    }
}

void placarRanking(const Placar *p) {
    printf("\n===== RANKING (menos passos primeiro) =====\n");
    if (p->inicio == NULL) {
        printf("Nenhuma partida concluida ainda.\n");
        return;
    }

    /* Monta uma lista auxiliar ordenada com CÓPIAS dos nós, assim o
     * histórico original continua na ordem cronológica. */
    No *ordenada = NULL;
    for (const No *atual = p->inicio; atual != NULL; atual = atual->prox) {
        No *copia = criarNo(atual->nome, atual->passos, atual->dicas);
        if (copia == NULL) {
            printf("Erro: memoria insuficiente para montar o ranking.\n");
            liberarLista(ordenada);
            return;
        }
        inserirOrdenado(&ordenada, copia);
    }

    printf("%-4s %-20s %8s %8s\n", "Pos", "Jogador", "Passos", "Dicas");
    int pos = 1;
    for (const No *atual = ordenada; atual != NULL; atual = atual->prox) {
        printf("%-4d %-20s %8d %8d\n", pos++, atual->nome, atual->passos, atual->dicas);
    }

    liberarLista(ordenada);
}

void placarEstatisticas(const Placar *p) {
    printf("\n===== ESTATISTICAS GERAIS =====\n");
    printf("Jogos feitos (iniciados):        %d\n", p->jogosIniciados);
    printf("Jogos concluidos:                %d\n", p->jogosConcluidos);
    printf("Jogos abandonados:               %d\n", p->jogosIniciados - p->jogosConcluidos);
    printf("Vezes que a resposta foi dada:   %d\n", p->totalDicas);
}

void placarLiberar(Placar *p) {
    liberarLista(p->inicio);
    p->inicio = NULL;
    p->fim = NULL;
    p->totalRegistros = 0;
}
