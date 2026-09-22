#ifndef LABIRINTO_H
#define LABIRINTO_H

/*
 *  LABIRINTO RECURSIVO
 *  Estrutura de Dados 2026-02 - UTFPR
 *  Atividade Avaliativa 1 - Recursividade aplicada a um jogo
 *
 *  Cada célula do labirinto guarda, em um único inteiro, a
 *  informação de quais paredes ela possui (bit = 1 -> parede
 *  presente). As direções são:
 *
 *        NORTE
 *          |
 *  OESTE -- + -- LESTE
 *          |
 *        SUL
 */

#define TAMANHO 12          /* labirinto TAMANHO x TAMANHO células */

#define PAREDE_NORTE 1
#define PAREDE_SUL   2
#define PAREDE_LESTE 4
#define PAREDE_OESTE 8
#define TODAS_PAREDES (PAREDE_NORTE | PAREDE_SUL | PAREDE_LESTE | PAREDE_OESTE)

typedef struct {
    int paredes[TAMANHO][TAMANHO];   /* mapa de paredes de cada célula   */
    int saidaX, saidaY;              /* posição da saída do labirinto    */
    int inicioX, inicioY;            /* posição inicial do jogador       */
} Labirinto;

/* ---- Geração (recursiva - busca em profundidade / backtracking) ---- */
void gerarLabirinto(Labirinto *lab);

/* ---- Resolução (recursiva - busca em profundidade / backtracking) ---- */
int resolverLabirinto(Labirinto *lab, int x, int y,
                       int visitado[TAMANHO][TAMANHO],
                       int caminhoX[], int caminhoY[], int *tamanhoCaminho);

/* ---- Impressão do labirinto no terminal ---- */
void imprimirLabirinto(Labirinto *lab, int jogadorX, int jogadorY,
                        int caminhoX[], int caminhoY[], int tamanhoCaminho);

/* ---- Movimento auxiliar ---- */
int podeMover(Labirinto *lab, int x, int y, int direcao);

#endif
