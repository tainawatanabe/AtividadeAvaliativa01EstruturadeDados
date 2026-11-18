# Labirinto Recursivo + Placar Dinâmico

Estrutura de Dados 2026-02 (UTFPR) — C puro

- **Atividade Avaliativa 1:** recursividade aplicada a um jogo (labirinto, busca em profundidade).
- **Atividade Avaliativa 2:** estrutura dinâmica (lista simplesmente encadeada) para gerenciar a pontuação dos jogadores.

## Como compilar e executar

Pré-requisitos: `gcc` e `make`.

```bash
make
./labirinto          # labirinto aleatório
./labirinto 42       # (opcional) seed fixo: repete sempre o mesmo labirinto
make clean           # remove os arquivos gerados
```

Sem `make` (ex.: Windows/MinGW):

```bash
gcc -Wall -Wextra -std=c11 -O2 -o labirinto jogo.c labirinto.c placar.c
```

## Como jogar

Menu principal: `1` novo jogo · `2` histórico · `3` ranking · `4` estatísticas · `0` sair.

Ao iniciar um jogo, informe o nome do jogador. Durante a partida:

| Tecla | Ação |
|-------|------|
| `w` `a` `s` `d` | norte / oeste / sul / leste |
| `h` | **dica**: mostra com `.` o caminho até a saída |
| `q` | abandonar a partida (não entra no ranking) |

O jogador (`@`) começa no canto superior esquerdo e deve chegar à saída (`S`).
Apenas movimentos que realmente andam contam como **passo** (bater na parede não conta).

## O que é registrado

- **Jogos feitos:** incrementa a cada partida iniciada (concluída ou não).
- **Vezes que a resposta foi dada:** incrementa a cada uso da dica `h` (por partida e no total).
- **Nome + passos:** ao concluir uma partida, o nome do jogador, os passos e as dicas usadas são
  gravados na lista encadeada.

## Atividade 1 — Recursividade (`labirinto.c`)

- `carvar()` — **geração** do labirinto por busca em profundidade recursiva com backtracking.
  Condição de parada: nenhum vizinho livre → a função retorna e a pilha de chamadas volta sozinha.
- `resolverLabirinto()` — **dica**: DFS recursivo até a saída.
  Parada 1: chegou na saída (sucesso). Parada 2: nenhuma direção funciona (falha → backtracking).

Ambas usam **recursão direta** (a função chama a si mesma).

## Atividade 2 — Lista simplesmente encadeada (`placar.c` / `placar.h`)

Implementada pelos próprios alunos, com alocação dinâmica (`malloc`) — sem vetores de tamanho fixo
guardando as partidas.

```c
typedef struct No {
    char nome[TAM_NOME];
    int passos;
    int dicas;
    struct No *prox;
} No;
```

| Operação | Função | Descrição |
|----------|--------|-----------|
| Cadastrar | `placarRegistrar()` | insere no fim da lista (ponteiro `fim`, O(1)) |
| Percorrer e apresentar | `placarListar()` | histórico em ordem cronológica |
| Apresentar ordenado | `placarRanking()` | menos passos primeiro; empate: menos dicas |
| Contadores | `placarEstatisticas()` | jogos feitos/concluídos/abandonados e total de dicas |
| Liberar memória | `placarLiberar()` | `free` de todos os nós |

O ranking é montado em uma **lista auxiliar** (cópias dos nós, inserção ordenada), então o histórico
original permanece na ordem em que as partidas aconteceram. A lista auxiliar é liberada ao final.

> O placar vive em memória durante a execução do programa (ao fechar, os dados se perdem).

## Arquivos

| Arquivo | Conteúdo |
|---------|----------|
| `labirinto.h` / `labirinto.c` | geração e resolução recursivas, impressão |
| `placar.h` / `placar.c` | lista encadeada, ranking, estatísticas |
| `jogo.c` | menu, partida, integração com o placar |
| `Makefile` | build |

## Integrantes

| Nome | Função no grupo |
|------|-----------------|
|  Taina |   Dev + Tester  |
