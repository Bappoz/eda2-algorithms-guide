/*
 * Hashing com Tratamento de Colisoes por Encadeamento Exterior
 * (Separate Chaining)
 *
 * Ideia: a tabela hash e um vetor de listas ligadas. Cada posicao (bucket)
 * guarda uma lista de todos os elementos que colidiram naquele indice.
 *
 *   Funcao hash simples: h(k) = k mod M   (M = tamanho da tabela)
 *
 *   Exemplo com M=7, insercao de {10, 17, 24, 31}:
 *     h(10) = 3  -> bucket[3] -> [10]
 *     h(17) = 3  -> bucket[3] -> [17] -> [10]  (colisao!)
 *     h(24) = 3  -> bucket[3] -> [24] -> [17] -> [10]
 *     h(31) = 3  -> bucket[3] -> [31] -> [24] -> ...
 *
 * Complexidade (n = numero de elementos, M = tamanho da tabela):
 *   Fator de carga: alfa = n/M
 *   Busca:    O(1 + alfa)  em media
 *             O(n)         no pior caso (tudo no mesmo bucket)
 *   Insercao: O(1) (insere no inicio da lista)
 *   Remocao:  O(1 + alfa)  em media
 *
 * Vantagens:
 *   - Simples de implementar
 *   - Nao ha problema de "tabela cheia" (a lista pode crescer)
 *   - Degradacao gracosa quando alfa > 1
 *
 * Desvantagens:
 *   - Uso extra de memoria para ponteiros das listas
 *   - Cache-unfriendly (listas ligadas nao sao contiguas na memoria)
 *
 * Boas praticas:
 *   - Manter alfa < 0.75 para performance aceitavel
 *   - Usar M primo para distribuicao mais uniforme
 */

#include <stdio.h>
#include <stdlib.h>

#define M 11  /* tamanho da tabela (numero primo recomendado) */

typedef struct No {
    int chave;
    struct No *prox;
} No;

typedef struct {
    No *buckets[M];
    int tamanho;
} TabelaHash;

int hash(int chave) {
    return ((chave % M) + M) % M;  /* trata chaves negativas */
}

TabelaHash *criar() {
    TabelaHash *t = calloc(1, sizeof(TabelaHash));
    return t;
}

void inserir(TabelaHash *t, int chave) {
    int h = hash(chave);
    /* Insere no inicio da lista (O(1)) */
    No *no = malloc(sizeof(No));
    no->chave = chave;
    no->prox = t->buckets[h];
    t->buckets[h] = no;
    t->tamanho++;
}

/* Retorna ponteiro para o no com a chave, ou NULL */
No *buscar(TabelaHash *t, int chave) {
    int h = hash(chave);
    for (No *cur = t->buckets[h]; cur != NULL; cur = cur->prox)
        if (cur->chave == chave) return cur;
    return NULL;
}

int remover(TabelaHash *t, int chave) {
    int h = hash(chave);
    No *cur = t->buckets[h], *ant = NULL;
    while (cur != NULL) {
        if (cur->chave == chave) {
            if (ant) ant->prox = cur->prox;
            else     t->buckets[h] = cur->prox;
            free(cur);
            t->tamanho--;
            return 1;
        }
        ant = cur;
        cur = cur->prox;
    }
    return 0;
}

void imprimir(TabelaHash *t) {
    printf("Tabela Hash (M=%d, n=%d, alfa=%.2f):\n",
           M, t->tamanho, (double)t->tamanho / M);
    for (int i = 0; i < M; i++) {
        printf("  [%2d] -> ", i);
        for (No *cur = t->buckets[i]; cur != NULL; cur = cur->prox)
            printf("%d -> ", cur->chave);
        printf("NULL\n");
    }
}

int main() {
    TabelaHash *t = criar();

    int vals[] = {5, 16, 27, 3, 14, 25, 36, 47, 8, 19, 30};
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++)
        inserir(t, vals[i]);

    imprimir(t);

    printf("\nBusca 27: %s\n", buscar(t, 27) ? "encontrado" : "nao encontrado");
    printf("Busca 99: %s\n", buscar(t, 99) ? "encontrado" : "nao encontrado");

    remover(t, 27);
    printf("\nApos remover 27:\n");
    imprimir(t);

    return 0;
}
