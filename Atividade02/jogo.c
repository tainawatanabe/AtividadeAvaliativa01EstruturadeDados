#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "labirinto.h"
#include "placar.h"

/*
 * ============================================================
 *  JOGO: LABIRINTO RECURSIVO + PLACAR DINAMICO
 *  Estrutura de Dados 2026-02 - UTFPR
 *  Atividade 1: recursividade (geração e resolução - DFS)
 *  Atividade 2: lista encadeada para guardar as pontuações
 * ============================================================
 *
 * Comandos durante a partida:
 *   w = norte, s = sul, d = leste, a = oeste
 *   h = dica (resposta calculada pela busca em profundidade)
 *   q = abandonar a partida
 */

/* Lê uma linha do teclado de forma segura. Retorna 0 em EOF. */
static int lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) return 0;
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return 1;
}

/* Joga uma partida completa e atualiza o placar. */
static void jogarPartida(Placar *placar) {
    Labirinto lab;
    char nome[TAM_NOME];
    char linha[64];
    int passos = 0;
    int dicas = 0;
    int mostrarDica = 0;
    int caminhoX[TAMANHO * TAMANHO];
    int caminhoY[TAMANHO * TAMANHO];
    int tamanhoCaminho = 0;

    printf("\nNome do jogador: ");
    if (!lerLinha(nome, sizeof(nome)) || nome[0] == '\0') {
        strcpy(nome, "Anonimo");
    }

    placar->jogosIniciados++;   /* contagem de jogos feitos */

    gerarLabirinto(&lab);
    int jogadorX = lab.inicioX;
    int jogadorY = lab.inicioY;

    printf("\nBoa sorte, %s! Chegue ate o 'S'.\n", nome);
    printf("Comandos: w=norte s=sul d=leste a=oeste h=dica q=abandonar\n");

    while (1) {
        printf("\n");
        imprimirLabirinto(&lab, jogadorX, jogadorY,
                           mostrarDica ? caminhoX : NULL, caminhoY, tamanhoCaminho);
        printf("Jogador: %s | Passos: %d | Dicas usadas: %d\n", nome, passos, dicas);

        if (jogadorX == lab.saidaX && jogadorY == lab.saidaY) {
            printf("\n*** PARABENS, %s! Escapou em %d passos (%d dicas)! ***\n",
                   nome, passos, dicas);
            placar->jogosConcluidos++;
            if (!placarRegistrar(placar, nome, passos, dicas)) {
                printf("Erro: memoria insuficiente para registrar a pontuacao.\n");
            }
            return;
        }

        printf("Comando (w/a/s/d/h/q): ");
        if (!lerLinha(linha, sizeof(linha))) {
            printf("\nEntrada encerrada. Partida abandonada.\n");
            return;
        }

        int novoX = jogadorX, novoY = jogadorY;

        switch (linha[0]) {
            case 'w': case 'W':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_NORTE)) { novoY--; passos++; }
                else printf(">> Parede!\n");
                break;
            case 's': case 'S':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_SUL)) { novoY++; passos++; }
                else printf(">> Parede!\n");
                break;
            case 'd': case 'D':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_LESTE)) { novoX++; passos++; }
                else printf(">> Parede!\n");
                break;
            case 'a': case 'A':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_OESTE)) { novoX--; passos++; }
                else printf(">> Parede!\n");
                break;
            case 'h': case 'H': {
                int visitado[TAMANHO][TAMANHO] = {{0}};
                tamanhoCaminho = 0;
                /* chamada da função recursiva de resolução (DFS) */
                resolverLabirinto(&lab, jogadorX, jogadorY, visitado,
                                   caminhoX, caminhoY, &tamanhoCaminho);
                mostrarDica = 1;
                dicas++;                 /* desta partida */
                placar->totalDicas++;    /* de todas as partidas */
                printf(">> Dica usada! Caminho marcado com '.'\n");
                continue;
            }
            case 'q': case 'Q':
                printf("\nPartida abandonada (nao entra no ranking).\n");
                return;
            default:
                printf(">> Comando invalido.\n");
                continue;
        }

        jogadorX = novoX;
        jogadorY = novoY;
        mostrarDica = 0;   /* a dica some após um movimento */
    }
}

int main(int argc, char *argv[]) {
    Placar placar;
    char linha[64];

    /* Semente opcional (./labirinto 42) para repetir o mesmo labirinto. */
    srand(argc > 1 ? (unsigned int) atoi(argv[1]) : (unsigned int) time(NULL));

    placarIniciar(&placar);

    printf("============================================\n");
    printf("        LABIRINTO RECURSIVO - UTFPR\n");
    printf("============================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1) Novo jogo\n");
        printf("2) Historico de partidas\n");
        printf("3) Ranking (ordenado)\n");
        printf("4) Estatisticas gerais\n");
        printf("0) Sair\n");
        printf("Opcao: ");

        if (!lerLinha(linha, sizeof(linha))) break;

        switch (atoi(linha)) {
            case 1: jogarPartida(&placar);     break;
            case 2: placarListar(&placar);     break;
            case 3: placarRanking(&placar);    break;
            case 4: placarEstatisticas(&placar); break;
            case 0:
                placarLiberar(&placar);
                printf("\nAte a proxima!\n");
                return 0;
            default: printf("Opcao invalida.\n");
        }
    }

    placarLiberar(&placar);
    return 0;
}
