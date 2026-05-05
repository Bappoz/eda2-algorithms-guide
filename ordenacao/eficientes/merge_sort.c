/*
 * Merge Sort (Ordenacao por Fusao)
 *
 * Paradigma: Dividir para Conquistar (Divide and Conquer).
 *
 * Ideia:
 *   1. DIVIDE: parte o vetor ao meio recursivamente ate ter subarrays de 1
 *      elemento (que sao trivialmente ordenados).
 *   2. CONQUISTA: intercala (merge) dois subarrays ordenados em um unico
 *      subarray ordenado.
 *
 *   [38, 27, 43, 3, 9, 82, 10]
 *        /               \
 *   [38, 27, 43]    [3, 9, 82, 10]
 *    /       \        /         \
 *  [38]  [27,43]  [3,9]      [82,10]
 *         / \      / \          / \
 *       [27][43] [3] [9]     [82] [10]
 *         \ /      \ /          \ /
 *       [27,43]   [3,9]       [10,82]
 *          \         /            /
 *        [27,38,43] [3,9,10,82]
 *                \     /
 *           [3,9,10,27,38,43,82]
 *
 * Complexidade:
 *   Melhor, medio e pior caso: O(n log n) -- SEMPRE garantido
 *   Espaco: O(n) auxiliar (vetor temporario no merge)
 *
 * Estavel: SIM (merge preserva a ordem relativa de elementos iguais)
 *
 * Versao 1: top-down (recursiva)
 * Versao 2: bottom-up (iterativa) -- sem recursao, merge de segmentos de
 *   tamanho 1, 2, 4, 8, ... de baixo para cima. Mesma complexidade, sem
 *   overhead de chamadas recursivas.
 *
 * Aplicacoes:
 *   - Ordenacao de listas ligadas (nao precisa de acesso aleatorio)
 *   - Merge Sort externo (dados em disco, nao cabem na memoria)
 *   - Base do Timsort (Python, Java) que combina Merge Sort + Insertion Sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* Intercala v[esq..meio] e v[meio+1..dir] usando vetor auxiliar */
void merge(int v[], int esq, int meio, int dir) {
    int n1 = meio - esq + 1;
    int n2 = dir - meio;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    memcpy(L, v + esq,    n1 * sizeof(int));
    memcpy(R, v + meio+1, n2 * sizeof(int));

    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2)
        v[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];

    free(L);
    free(R);
}

/* ---- Versao 1: top-down (recursiva) ---- */
void merge_sort(int v[], int esq, int dir) {
    if (esq >= dir) return;
    int meio = esq + (dir - esq) / 2;
    merge_sort(v, esq, meio);
    merge_sort(v, meio + 1, dir);
    merge(v, esq, meio, dir);
}

/*
 * ---- Versao 2: bottom-up (iterativa) ----
 *
 * Comeca fundindo pares de elementos (tamanho 1), depois grupos de 2,
 * depois de 4, etc. Equivalente ao top-down mas sem recursao.
 */
void merge_sort_bottomup(int v[], int n) {
    for (int tam = 1; tam < n; tam *= 2) {
        for (int esq = 0; esq < n - tam; esq += 2 * tam) {
            int meio = esq + tam - 1;
            int dir  = (esq + 2*tam - 1 < n-1) ? esq + 2*tam - 1 : n-1;
            merge(v, esq, meio, dir);
        }
    }
}

int main() {
    int v1[] = {38, 27, 43, 3, 9, 82, 10};
    int v2[] = {38, 27, 43, 3, 9, 82, 10};
    int n = 7;

    printf("Original:             "); imprimir(v1, n);

    merge_sort(v1, 0, n - 1);
    printf("Merge (top-down):     "); imprimir(v1, n);

    merge_sort_bottomup(v2, n);
    printf("Merge (bottom-up):    "); imprimir(v2, n);

    return 0;
}
