/*
 * Quick Sort (Ordenacao Rapida)
 *
 * Paradigma: Dividir para Conquistar.
 *
 * Ideia: escolhe um elemento como PIVO e rearranja o vetor de modo que:
 *   - todos os elementos menores que o pivo ficam a sua esquerda
 *   - todos os maiores ficam a sua direita
 * Em seguida, aplica recursivamente nas duas partes.
 *
 * Complexidade:
 *   Melhor/medio: O(n log n) -- pivo sempre divide ao meio
 *   Pior caso:    O(n^2)    -- pivo sempre e o minimo ou maximo
 *                             (ex: vetor ja ordenado com pivo = ultimo)
 *
 * Espaco: O(log n) na pilha de recursao (caso medio)
 *         O(n)     no pior caso (recursao degenerada)
 *
 * Estavel: NAO (a particao pode reordenar elementos iguais)
 *
 * Na pratica Quick Sort e geralmente mais rapido que Merge Sort por:
 *   - Melhor localidade de cache (acesso sequencial)
 *   - Sem alocacao de memoria extra
 *   - Constante pequena na notacao big-O
 *
 * Tres esquemas de particao implementados:
 *
 * 1. Particao de Lomuto
 *    Pivo = ultimo elemento. Usa um unico ponteiro que "cresce" da esquerda.
 *    Mais simples de implementar. Mais trocas que Hoare.
 *
 * 2. Particao de Hoare
 *    Pivo = primeiro elemento. Dois ponteiros que se encontram no meio.
 *    Em media 3x menos trocas que Lomuto. Original do algoritmo.
 *
 * 3. Quick Sort com Pivo na Mediana de Tres (Median-of-Three)
 *    Pivo = mediana entre v[esq], v[meio] e v[dir].
 *    Evita o pior caso para vetores ja ordenados/inversamente ordenados.
 *    Estrategia usada em implementacoes de producao (qsort() do glibc).
 *
 * Bonus: Quick Sort com corte para Insertion Sort em subarrays pequenos
 *    (estrategia do Introsort/Timsort): quando n < limiar (tipicamente 16),
 *    usa Insertion Sort que tem overhead menor para arrays pequenos.
 */

#include <stdio.h>

#define LIMIAR 8  /* subarrays menores que isto usam Insertion Sort */

void trocar(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ---- Insertion Sort auxiliar para subarrays pequenos ---- */
void insertion_sort(int v[], int esq, int dir) {
    for (int i = esq + 1; i <= dir; i++) {
        int chave = v[i];
        int j = i - 1;
        while (j >= esq && v[j] > chave) { v[j+1] = v[j]; j--; }
        v[j+1] = chave;
    }
}

/* ==============================================================
 * 1. Particao de LOMUTO
 * ============================================================== */
int particao_lomuto(int v[], int esq, int dir) {
    int pivo = v[dir];
    int i = esq - 1;
    for (int j = esq; j < dir; j++)
        if (v[j] <= pivo) trocar(&v[++i], &v[j]);
    trocar(&v[i+1], &v[dir]);
    return i + 1;
}

void quick_sort_lomuto(int v[], int esq, int dir) {
    if (esq >= dir) return;
    int p = particao_lomuto(v, esq, dir);
    quick_sort_lomuto(v, esq, p - 1);
    quick_sort_lomuto(v, p + 1, dir);
}

/* ==============================================================
 * 2. Particao de HOARE
 * Retorna o indice j tal que v[esq..j] e v[j+1..dir] estao
 * particionados. O pivo nao esta necessariamente em j.
 * ============================================================== */
int particao_hoare(int v[], int esq, int dir) {
    int pivo = v[esq];
    int i = esq - 1, j = dir + 1;
    while (1) {
        do { i++; } while (v[i] < pivo);
        do { j--; } while (v[j] > pivo);
        if (i >= j) return j;
        trocar(&v[i], &v[j]);
    }
}

void quick_sort_hoare(int v[], int esq, int dir) {
    if (esq >= dir) return;
    int p = particao_hoare(v, esq, dir);
    quick_sort_hoare(v, esq, p);
    quick_sort_hoare(v, p + 1, dir);
}

/* ==============================================================
 * 3. Mediana de tres + Insertion Sort para subarrays pequenos
 * ============================================================== */
int mediana_de_tres(int v[], int esq, int dir) {
    int meio = esq + (dir - esq) / 2;
    /* Ordena os tres e coloca a mediana em v[meio] */
    if (v[esq] > v[meio])  trocar(&v[esq],  &v[meio]);
    if (v[esq] > v[dir])   trocar(&v[esq],  &v[dir]);
    if (v[meio] > v[dir])  trocar(&v[meio], &v[dir]);
    /* v[meio] e a mediana; coloca como pivo em v[dir-1] */
    trocar(&v[meio], &v[dir]);
    return v[dir];
}

void quick_sort_med3(int v[], int esq, int dir) {
    if (dir - esq < LIMIAR) {
        insertion_sort(v, esq, dir);
        return;
    }
    int pivo = mediana_de_tres(v, esq, dir);
    int i = esq - 1, j = dir;
    while (1) {
        while (v[++i] < pivo);
        while (v[--j] > pivo);
        if (i >= j) break;
        trocar(&v[i], &v[j]);
    }
    trocar(&v[i], &v[dir]);
    quick_sort_med3(v, esq, i - 1);
    quick_sort_med3(v, i + 1, dir);
}

int main() {
    int base[] = {10, 80, 30, 90, 40, 50, 70, 60, 20};
    int n = 9;
    int v1[9], v2[9], v3[9];
    for (int i = 0; i < n; i++) v1[i] = v2[i] = v3[i] = base[i];

    printf("Original:              "); imprimir(base, n);

    quick_sort_lomuto(v1, 0, n - 1);
    printf("Quick (Lomuto):        "); imprimir(v1, n);

    quick_sort_hoare(v2, 0, n - 1);
    printf("Quick (Hoare):         "); imprimir(v2, n);

    quick_sort_med3(v3, 0, n - 1);
    printf("Quick (Med3+InsSort):  "); imprimir(v3, n);

    /* Demonstra pior caso do Lomuto: vetor ja ordenado */
    int sorted[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    printf("\nJa ordenado (pior caso para pivo=ultimo):\n");
    printf("Antes:  "); imprimir(sorted, 9);
    /* Med3 evita pior caso aqui */
    quick_sort_med3(sorted, 0, 8);
    printf("Depois: "); imprimir(sorted, 9);

    return 0;
}
