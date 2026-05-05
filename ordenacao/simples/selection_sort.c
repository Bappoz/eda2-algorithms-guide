/*
 * Selection Sort (Ordenacao por Selecao)
 *
 * Ideia: divide o vetor em duas partes: ordenada (inicio) e nao-ordenada
 * (resto). A cada passo, encontra o MINIMO da parte nao-ordenada e o coloca
 * no final da parte ordenada com uma unica troca.
 *
 *   Passo 0: [64, 25, 12, 22, 11] -> minimo=11, troca com pos 0 -> [11, 25, 12, 22, 64]
 *   Passo 1: [11, 25, 12, 22, 64] -> minimo=12, troca com pos 1 -> [11, 12, 25, 22, 64]
 *   Passo 2: [11, 12, 25, 22, 64] -> minimo=22, troca com pos 2 -> [11, 12, 22, 25, 64]
 *   Passo 3: [11, 12, 22, 25, 64] -> minimo=25, ja na pos 3    -> [11, 12, 22, 25, 64]
 *
 * Complexidade:
 *   Pior, medio e melhor caso: O(n^2) comparacoes
 *   Numero de trocas: O(n) -- apenas n-1 trocas, menor que Bubble Sort
 *
 * Espaco: O(1) (in-place)
 * Estavel: NAO (a troca pode mover elementos iguais)
 *   Exemplo: [3a, 3b, 1] -> troca 1 com 3a -> [1, 3b, 3a]: 3a e 3b invertem.
 *
 * Vantagem sobre Bubble Sort: numero de trocas e sempre O(n), o que o torna
 * preferivel quando a operacao de escrita (troca) e custosa.
 *
 * Variante: Double Selection Sort seleciona simultaneamente o minimo e o
 * maximo em cada passagem, reduzindo o numero de passagens pela metade.
 */

#include <stdio.h>

void trocar(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ---- Versao 1: classica ---- */
void selection_sort(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int idx_min = i;
        for (int j = i + 1; j < n; j++)
            if (v[j] < v[idx_min]) idx_min = j;
        if (idx_min != i)
            trocar(&v[i], &v[idx_min]);
    }
}

/*
 * ---- Versao 2: Double Selection Sort ----
 *
 * A cada iteracao, encontra tanto o minimo quanto o maximo da parte
 * nao-ordenada. O minimo vai para a esquerda e o maximo para a direita.
 * Reduz o numero de passagens pelo vetor de n para n/2, mas o numero total
 * de comparacoes permanece O(n^2).
 */
void double_selection_sort(int v[], int n) {
    int esq = 0, dir = n - 1;
    while (esq < dir) {
        int idx_min = esq, idx_max = esq;
        for (int j = esq + 1; j <= dir; j++) {
            if (v[j] < v[idx_min]) idx_min = j;
            if (v[j] > v[idx_max]) idx_max = j;
        }
        trocar(&v[esq], &v[idx_min]);
        /* Se o maximo estava em 'esq', ele foi movido para idx_min */
        if (idx_max == esq) idx_max = idx_min;
        trocar(&v[dir], &v[idx_max]);
        esq++;
        dir--;
    }
}

int main() {
    int v1[] = {64, 25, 12, 22, 11, 90, 45};
    int v2[] = {64, 25, 12, 22, 11, 90, 45};
    int n = 7;

    printf("Original:              "); imprimir(v1, n);

    selection_sort(v1, n);
    printf("Selection (classico):  "); imprimir(v1, n);

    double_selection_sort(v2, n);
    printf("Selection (double):    "); imprimir(v2, n);

    return 0;
}
