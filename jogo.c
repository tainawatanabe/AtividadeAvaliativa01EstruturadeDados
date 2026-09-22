#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "labirinto.h"

/*
 *  JOGO: LABIRINTO RECURSIVO
 *  Estrutura de Dados 2026-02 - UTFPR
 * Regras:
 *   - O jogador (@) começa no canto superior esquerdo.
 *   - O objetivo é alcançar a saída (S) no canto inferior direito.
 *   - Comandos: w = norte, s = sul, d = leste, a = oeste
 *               h = dica (mostra o caminho encontrado pela busca
 *                          em profundidade recursiva)
 *               q = sair
 *
 * O labirinto é gerado com uma função recursiva (carvar, em
 * labirinto.c) e a dica é calculada com outra função recursiva
 * (resolverLabirinto), aplicando exatamente o conceito de
 * recursividade + condição de parada proposto na atividade.
 */

static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta resto da linha */ }
}

int main(void) {
    Labirinto lab;
    int jogadorX, jogadorY;
    int movimentos = 0;
    int mostrarDica = 0;
    int caminhoX[TAMANHO * TAMANHO];
    int caminhoY[TAMANHO * TAMANHO];
    int tamanhoCaminho = 0;

    srand((unsigned int) time(NULL));

    gerarLabirinto(&lab);
    jogadorX = lab.inicioX;
    jogadorY = lab.inicioY;

    printf("        LABIRINTO RECURSIVO - UTFPR\n");
    printf("Voce eh o '@'. Chegue ate o 'S'.\n");
    printf("Comandos: w=norte  s=sul  d=leste  a=oeste\n");
    printf("          h=dica (busca em profundidade)  q=sair\n\n");

    while (1) {
        printf("\n");
        imprimirLabirinto(&lab, jogadorX, jogadorY,
                           mostrarDica ? caminhoX : NULL, caminhoY, tamanhoCaminho);
        printf("Movimentos: %d\n", movimentos);

        if (jogadorX == lab.saidaX && jogadorY == lab.saidaY) {
            printf("\nPARABENS! Voce escapou do labirinto em %d movimentos! ***\n", movimentos);
            break;
        }

        printf("Comando (w/a/s/d/h/q): ");
        char comando;
        if (scanf(" %c", &comando) != 1) break;
        limparBuffer();

        int novoX = jogadorX, novoY = jogadorY;

        switch (comando) {
            case 'w': case 'W':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_NORTE)) novoY--;
                movimentos++;
                break;
            case 's': case 'S':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_SUL)) novoY++;
                movimentos++;
                break;
            case 'd': case 'D':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_LESTE)) novoX++;
                movimentos++;
                break;
            case 'a': case 'A':
                if (podeMover(&lab, jogadorX, jogadorY, PAREDE_OESTE)) novoX--;
                movimentos++;
                break;
            case 'h': case 'H': {
                int visitado[TAMANHO][TAMANHO] = {0};
                tamanhoCaminho = 0;
                /* chamada da função recursiva de resolução (DFS) */
                resolverLabirinto(&lab, jogadorX, jogadorY, visitado,
                                   caminhoX, caminhoY, &tamanhoCaminho);
                mostrarDica = 1;
                printf(">> Dica: caminho encontrado pela busca em profundidade recursiva!\n");
                continue;
            }
            case 'q': case 'Q':
                printf("\nAte a proxima!\n");
                return 0;
            default:
                printf(">> Comando invalido.\n");
                continue;
        }

        jogadorX = novoX;
        jogadorY = novoY;
        mostrarDica = 0; /* dica some após um movimento real */
    }

    return 0;
}
