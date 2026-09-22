# Labirinto Recursivo

Atividade Avaliativa 1 — Estrutura de Dados 2026-02 (UTFPR)
Tema: **Aplicar recursividade em um jogo**

Jogo de labirinto em **C puro**, onde tanto a **geração** quanto a
**resolução** do labirinto são feitas por funções **recursivas** que
implementam **busca em profundidade (DFS)** com backtracking.

## Como jogar

O jogador (`@`) começa no canto superior esquerdo e precisa chegar até
a saída (`S`), no canto inferior direito, navegando pelos corredores
do labirinto.

- `w` — mover para o norte
- `s` — mover para o sul
- `d` — mover para o leste
- `a` — mover para o oeste
- `h` — pedir uma **dica** (mostra, com `.`, o caminho até a saída
  calculado pela função recursiva de resolução)
- `q` — sair do jogo

## Como compilar e executar

Pré-requisitos: `gcc` e `make`.

```bash
make
./labirinto
```

Para limpar os arquivos gerados pela compilação:

```bash
make clean
```

## Onde está a recursividade

### 1. Geração do labirinto — `formar()` em `labirinto.c`

Implementa o algoritmo clássico de **busca em profundidade recursiva
com backtracking** para gerar um labirinto "perfeito" (existe sempre
exatamente um caminho entre duas células quaisquer):

1. Marca a célula atual como visitada.
2. Embaralha as 4 direções possíveis.
3. Para cada vizinho ainda não visitado, derruba a parede entre as
   duas células e **chama a si mesma** para esse vizinho.
4. **Condição de parada:** quando uma célula não tem mais vizinhos
   livres, a função simplesmente retorna — a pilha de chamadas volta
   sozinha para a célula anterior (backtracking), que então tenta
   outra direção.

### 2. Resolução do labirinto — `resolverLabirinto()` em `labirinto.c`

Usada pelo comando de dica (`h`). A partir da posição do jogador,
tenta alcançar a saída chamando a si mesma para cada vizinho
acessível (sem parede) e ainda não visitado:

- **Condição de parada 1 (sucesso):** a posição atual é a saída.
- **Condição de parada 2 (falha/backtrack):** nenhuma das quatro
  direções leva a um caminho válido — a função retorna `0` e a
  chamada anterior tenta outra direção, até achar o caminho ou
  esgotar as possibilidades.

## Estrutura dos arquivos

| Arquivo         | Conteúdo                                           |
|------------------|-----------------------------------------------------|
| `labirinto.h`    | Estruturas de dados e protótipos                    |
| `labirinto.c`    | Geração (DFS) e resolução (DFS) recursivas + impressão |
| `jogo.c`         | Laço principal do jogo, entrada do jogador          |
| `Makefile`       | Build do projeto                                    |

## Integrantes

| Nome                             | Função no grupo         |
| Taina Yumi de Araujo Watanabe    | Desenvolvedora, Tester. |
