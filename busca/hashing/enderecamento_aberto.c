/*
 * Hashing com Enderecamento Aberto (Open Addressing)
 *
 * Diferenca do encadeamento: NAO ha listas ligadas. Todas as entradas ficam
 * dentro da propria tabela. Em caso de colisao, procura-se outra posicao
 * pela funcao de sondagem (probe sequence).
 *
 * Restricao: tabela NUNCA pode ficar completamente cheia (alfa < 1).
 * Recomendado manter alfa < 0.7 para evitar degradacao.
 *
 * Tres estrategias de sondagem implementadas:
 *
 * 1. Sondagem Linear (Linear Probing)
 *    h(k, i) = (h'(k) + i) mod M
 *    - Simples, boa localidade de cache
 *    - Problema: agrupamento primario (primary clustering): sequencias longas
 *      de posicoes ocupadas se formam, piorando buscas futuras
 *
 * 2. Sondagem Quadratica (Quadratic Probing)
 *    h(k, i) = (h'(k) + c1*i + c2*i^2) mod M
 *    - Evita agrupamento primario
 *    - Problema: agrupamento secundario (secondary clustering): elementos com
 *      o mesmo h'(k) seguem a mesma sequencia de sondagem
 *    - Nem sempre visita todos os slots (depende de M ser primo e alfa < 0.5)
 *
 * 3. Hashing Duplo (Double Hashing)
 *    h(k, i) = (h1(k) + i * h2(k)) mod M
 *    - Evita ambos os tipos de agrupamento
 *    - Melhor distribuicao teorica entre as tres
 *    - h2(k) deve ser coprimo com M (se M for primo, qualquer h2(k) != 0 serve)
 *
 * Complexidade esperada (alfa = n/M):
 *   Busca com sucesso:    O(1/(1-alfa))
 *   Busca sem sucesso:    O(1/(1-alfa)^2)
 *
 * Remocao: nao pode simplesmente apagar (quebraria a cadeia de busca).
 * Usa-se uma marcacao DELETADO (tombstone) que e ignorada na busca mas
 * ocupa slot na insercao.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 13   /* tamanho da tabela (primo) */

typedef enum { VAZIO, OCUPADO, DELETADO } Estado;

typedef struct {
    int chave;
    Estado estado;
} Slot;

typedef struct {
    Slot tabela[M];
    int n;
    const char *estrategia;
} TabelaAberta;

/* ---- Funcoes hash auxiliares ---- */
int h1(int k) { return ((k % M) + M) % M; }

/* h2 deve ser diferente de 0 e coprimo com M (M primo => qualquer valor serve) */
int h2(int k) { return 1 + (((k / M) % (M - 1)) + M - 1) % (M - 1); }

/* ---- Sondagens ---- */
int sonda_linear(int k, int i)    { return (h1(k) + i) % M; }
int sonda_quadratica(int k, int i){ return (h1(k) + i + 3*i*i) % M; }
int sonda_dupla(int k, int i)     { return (h1(k) + i * h2(k)) % M; }

typedef int (*FnSonda)(int, int);

TabelaAberta *criar(const char *estrategia) {
    TabelaAberta *t = calloc(1, sizeof(TabelaAberta));
    t->estrategia = estrategia;
    return t;
}

FnSonda obter_sonda(TabelaAberta *t) {
    if (strcmp(t->estrategia, "linear")     == 0) return sonda_linear;
    if (strcmp(t->estrategia, "quadratica") == 0) return sonda_quadratica;
    return sonda_dupla;
}

int inserir(TabelaAberta *t, int chave) {
    if (t->n >= M) { printf("Tabela cheia!\n"); return 0; }
    FnSonda sonda = obter_sonda(t);
    for (int i = 0; i < M; i++) {
        int pos = sonda(chave, i);
        if (t->tabela[pos].estado != OCUPADO) {
            t->tabela[pos].chave  = chave;
            t->tabela[pos].estado = OCUPADO;
            t->n++;
            return pos;
        }
    }
    return -1;
}

int buscar(TabelaAberta *t, int chave) {
    FnSonda sonda = obter_sonda(t);
    for (int i = 0; i < M; i++) {
        int pos = sonda(chave, i);
        if (t->tabela[pos].estado == VAZIO) return -1;          /* nao existe */
        if (t->tabela[pos].estado == OCUPADO &&
            t->tabela[pos].chave == chave)  return pos;
        /* DELETADO: continua sondando */
    }
    return -1;
}

int remover(TabelaAberta *t, int chave) {
    int pos = buscar(t, chave);
    if (pos == -1) return 0;
    t->tabela[pos].estado = DELETADO;  /* tombstone */
    t->n--;
    return 1;
}

void imprimir(TabelaAberta *t) {
    printf("Tabela [%s] (M=%d, n=%d, alfa=%.2f):\n",
           t->estrategia, M, t->n, (double)t->n / M);
    for (int i = 0; i < M; i++) {
        printf("  [%2d] ", i);
        switch (t->tabela[i].estado) {
            case VAZIO:    printf("--- (vazio)\n");                     break;
            case DELETADO: printf("DEL (deletado)\n");                  break;
            case OCUPADO:  printf("%d\n", t->tabela[i].chave);          break;
        }
    }
}

void testar(const char *estrategia, int vals[], int n) {
    printf("\n=== Estrategia: %s ===\n", estrategia);
    TabelaAberta *t = criar(estrategia);

    for (int i = 0; i < n; i++) inserir(t, vals[i]);
    imprimir(t);

    printf("Busca 25: pos=%d\n", buscar(t, 25));
    remover(t, 25);
    printf("Apos remover 25, busca 25: pos=%d\n", buscar(t, 25));
    /* Insere de volta para mostrar que DELETADO e reaproveitado */
    inserir(t, 99);
    printf("Apos inserir 99:\n");
    imprimir(t);

    free(t);
}

int main() {
    int vals[] = {5, 25, 14, 3, 36, 47, 18};
    int n = sizeof(vals) / sizeof(vals[0]);

    testar("linear",     vals, n);
    testar("quadratica", vals, n);
    testar("dupla",      vals, n);

    return 0;
}
