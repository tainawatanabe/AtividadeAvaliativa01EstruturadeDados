#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"

/*
 *  GERAÇÃO DO LABIRINTO - Busca em Profundidade Recursiva
 *
 * Essa é a mesma ideia sugerida no enunciado da atividade:
 *
 *   A. Crie uma função.                (formar)
 *   B. Faça-a chamar a si mesma.        (chamada recursiva p/ vizinho)
 *   C. Defina uma condição de parada.   (sem vizinhos não visitados)
 *   D. Observe o que acontece.          (labirinto perfeito é formado)
 *   E. Transforme em mecânica de jogo.  (o jogador precisa atravessá-lo)
 *
 * A função visita uma célula, embaralha as 4 direções possíveis e,
 * para cada vizinho ainda não visitado, derruba a parede entre as
 * duas células e chama a si mesma recursivamente para esse vizinho.
 * Quando uma célula não tem mais vizinhos livres, a recursão
 * simplesmente retorna (backtracking) e a pilha de chamadas volta
 * para a célula anterior, exatamente como o PDF descreve:
 * "um caminho que não leva a lugar nenhum ensina você a voltar e
 * tentar outra possibilidade".
 */

static void embaralhar(int vetor[4]) {
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = tmp;
    }
}

/* Retorna o deslocamento (dx, dy) de cada direção */
static void deslocamento(int direcao, int *dx, int *dy) {
    switch (direcao) {
        case PAREDE_NORTE: *dx = 0;  *dy = -1; break;
        case PAREDE_SUL:   *dx = 0;  *dy = 1;  break;
        case PAREDE_LESTE: *dx = 1;  *dy = 0;  break;
        case PAREDE_OESTE: *dx = -1; *dy = 0;  break;
        default:           *dx = 0;  *dy = 0;  break;
    }
}

/* Parede oposta a uma direção (usada para remover a parede dos dois lados) */
static int paredeOposta(int direcao) {
    switch (direcao) {
        case PAREDE_NORTE: return PAREDE_SUL;
        case PAREDE_SUL:   return PAREDE_NORTE;
        case PAREDE_LESTE: return PAREDE_OESTE;
        case PAREDE_OESTE: return PAREDE_LESTE;
    }
    return 0;
}

/* Função RECURSIVA principal de geração (busca em profundidade) */
static void formar(Labirinto *lab, int x, int y, int visitado[TAMANHO][TAMANHO]) {
    visitado[y][x] = 1;

    int direcoes[4] = {PAREDE_NORTE, PAREDE_SUL, PAREDE_LESTE, PAREDE_OESTE};
    embaralhar(direcoes);

    for (int i = 0; i < 4; i++) {
        int dx, dy;
        deslocamento(direcoes[i], &dx, &dy);
        int nx = x + dx;
        int ny = y + dy;

        /* condição de parada / poda: fora do labirinto ou já visitado */
        if (nx < 0 || nx >= TAMANHO || ny < 0 || ny >= TAMANHO) continue;
        if (visitado[ny][nx]) continue;

        /* derruba a parede entre a célula atual e a vizinha */
        lab->paredes[y][x] &= ~direcoes[i];
        lab->paredes[ny][nx] &= ~paredeOposta(direcoes[i]);

        /* chamada recursiva: a função chama a si mesma para o vizinho */
        formar(lab, nx, ny, visitado);
    }
    /* sem vizinhos livres -> a recursão retorna (backtrack automático) */
}

void gerarLabirinto(Labirinto *lab) {
    int visitado[TAMANHO][TAMANHO];

    for (int y = 0; y < TAMANHO; y++)
        for (int x = 0; x < TAMANHO; x++) {
            lab->paredes[y][x] = TODAS_PAREDES;
            visitado[y][x] = 0;
        }

    lab->inicioX = 0;
    lab->inicioY = 0;
    lab->saidaX = TAMANHO - 1;
    lab->saidaY = TAMANHO - 1;

    formar(lab, lab->inicioX, lab->inicioY, visitado);
}

/*
 * RESOLUÇÃO DO LABIRINTO - Busca em Profundidade Recursiva
 *
 * Usada pelo comando de "dica" do jogo. A função tenta, a partir
 * da posição (x, y), alcançar a saída chamando a si mesma para
 * cada vizinho acessível (sem parede) e ainda não visitado.
 *
 * Condição de parada:
 *   1) (x, y) é a saída            -> sucesso, encerra a recursão
 *   2) nenhuma direção leva a um
 *      caminho válido               -> falha, a função retorna 0
 *      e a chamada anterior tenta outra direção (backtracking)
 */
int resolverLabirinto(Labirinto *lab, int x, int y,
                       int visitado[TAMANHO][TAMANHO],
                       int caminhoX[], int caminhoY[], int *tamanhoCaminho) {

    /* condição de parada 1: chegou na saída */
    if (x == lab->saidaX && y == lab->saidaY) {
        caminhoX[*tamanhoCaminho] = x;
        caminhoY[*tamanhoCaminho] = y;
        (*tamanhoCaminho)++;
        return 1;
    }

    visitado[y][x] = 1;

    int direcoes[4] = {PAREDE_NORTE, PAREDE_SUL, PAREDE_LESTE, PAREDE_OESTE};

    for (int i = 0; i < 4; i++) {
        int dx, dy;
        deslocamento(direcoes[i], &dx, &dy);
        int nx = x + dx;
        int ny = y + dy;

        if (nx < 0 || nx >= TAMANHO || ny < 0 || ny >= TAMANHO) continue;
        if (visitado[ny][nx]) continue;
        if (lab->paredes[y][x] & direcoes[i]) continue; /* tem parede */

        /* chamada recursiva para o vizinho */
        if (resolverLabirinto(lab, nx, ny, visitado, caminhoX, caminhoY, tamanhoCaminho)) {
            caminhoX[*tamanhoCaminho] = x;
            caminhoY[*tamanhoCaminho] = y;
            (*tamanhoCaminho)++;
            return 1;
        }
    }

    /* condição de parada 2: nenhum vizinho leva à saída -> backtrack */
    return 0;
}

int podeMover(Labirinto *lab, int x, int y, int direcao) {
    return !(lab->paredes[y][x] & direcao);
}

/* ---- Impressão em ASCII --------------------------------------------- */
void imprimirLabirinto(Labirinto *lab, int jogadorX, int jogadorY,
                        int caminhoX[], int caminhoY[], int tamanhoCaminho) {

    for (int x = 0; x < TAMANHO; x++) printf("+---");
    printf("+\n");

    for (int y = 0; y < TAMANHO; y++) {
        printf("|");
        for (int x = 0; x < TAMANHO; x++) {
            char c = ' ';

            if (x == jogadorX && y == jogadorY) {
                c = '@';
            } else if (x == lab->saidaX && y == lab->saidaY) {
                c = 'S';
            } else if (caminhoX != NULL) {
                for (int i = 0; i < tamanhoCaminho; i++) {
                    if (caminhoX[i] == x && caminhoY[i] == y) { c = '.'; break; }
                }
            }

            printf(" %c ", c);
            printf("%s", (lab->paredes[y][x] & PAREDE_LESTE) ? "|" : " ");
        }
        printf("\n+");
        for (int x = 0; x < TAMANHO; x++) {
            printf("%s", (lab->paredes[y][x] & PAREDE_SUL) ? "---+" : "   +");
        }
        printf("\n");
    }
}
