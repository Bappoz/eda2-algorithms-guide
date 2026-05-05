/*
 * Shell Sort (Ordenacao de Shell / Gap Sort)
 *
 * Ideia: generalizacao do Insertion Sort que compara e move elementos
 * distantes antes de operar em elementos adjacentes. Usa uma sequencia
 * de "gaps" (intervalos) decrescentes ate gap=1 (que equivale ao
 * Insertion Sort classico). Com os elementos ja quase no lugar certo
 * quando gap=1, poucos deslocamentos sao necessarios.
 *
 *   Exemplo com gaps [4, 2, 1] para [8, 3, 6, 1, 5, 7, 2, 4]:
 *
 *   gap=4: compara v[0]-v[4], v[1]-v[5], v[2]-v[6], v[3]-v[7]
 *          [5, 3, 2, 1, 8, 7, 6, 4]
 *   gap=2: insertion sort de 2 em 2
 *          [2, 1, 5, 3, 6, 4, 8, 7]
 *   gap=1: insertion sort final (quase ordenado -> rapido)
 *          [1, 2, 3, 4, 5, 6, 7, 8]
 *
 * A complexidade depende CRITICAMENTE da sequencia de gaps escolhida:
 *
 *   Sequencia de Shell (n/2, n/4, ..., 1):
 *     - Pior caso: O(n^2)  -- a mais simples, mas nao a melhor
 *
 *   Sequencia de Hibbard (1, 3, 7, 15, ..., 2^k - 1):
 *     - Pior caso: O(n^(3/2))
 *
 *   Sequencia de Sedgewick (1, 5, 19, 41, 109, ...):
 *     - Pior caso: O(n^(4/3))  -- uma das melhores praticas
 *
 *   Sequencia de Ciura (1, 4, 10, 23, 57, 132, 301, 701, ...):
 *     - Empiricamente a melhor para n tipicos
 *
 * Espaco: O(1) (in-place)
 * Estavel: NAO (trocas de longa distancia podem reordenar iguais)
 *
 * Na pratica, Shell Sort supera O(n^2) e e competitive com Merge/Quick
 * para n moderado (~10.000), sem custo extra de memoria do Merge Sort.
 */

#include <stdio.h>

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ---- Versao 1: Shell (gap = n/2) ---- */
void shell_sort_shell(int v[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        /* Insertion Sort com passo 'gap' */
        for (int i = gap; i < n; i++) {
            int chave = v[i];
            int j = i - gap;
            while (j >= 0 && v[j] > chave) {
                v[j + gap] = v[j];
                j -= gap;
            }
            v[j + gap] = chave;
        }
    }
}

/* ---- Versao 2: Sequencia de Hibbard (2^k - 1) ---- */
void shell_sort_hibbard(int v[], int n) {
    /* Encontra o maior gap da sequencia de Hibbard menor que n */
    int gap = 1;
    while (gap < n) gap = 2 * gap + 1;
    gap /= 2;

    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int chave = v[i];
            int j = i - gap;
            while (j >= 0 && v[j] > chave) {
                v[j + gap] = v[j];
                j -= gap;
            }
            v[j + gap] = chave;
        }
        gap /= 2;
    }
}

/* ---- Versao 3: Sequencia de Ciura (empiricamente otima) ---- */
void shell_sort_ciura(int v[], int n) {
    /* Gaps de Ciura: {1, 4, 10, 23, 57, 132, 301, 701}
     * Para n > ~1750, multiplica por 2.25 */
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int num_gaps = 8;

    for (int g = 0; g < num_gaps; g++) {
        int gap = gaps[g];
        if (gap >= n) continue;
        for (int i = gap; i < n; i++) {
            int chave = v[i];
            int j = i - gap;
            while (j >= 0 && v[j] > chave) {
                v[j + gap] = v[j];
                j -= gap;
            }
            v[j + gap] = chave;
        }
    }
}

int main() {
    int base[] = {8, 3, 6, 1, 5, 7, 2, 4, 9, 0, 11, 13, 10, 12};
    int n = 14;

    int v1[14], v2[14], v3[14];
    for (int i = 0; i < n; i++) v1[i] = v2[i] = v3[i] = base[i];

    printf("Original:              "); imprimir(base, n);

    shell_sort_shell(v1, n);
    printf("Shell (gap=n/2):       "); imprimir(v1, n);

    shell_sort_hibbard(v2, n);
    printf("Shell (Hibbard):       "); imprimir(v2, n);

    shell_sort_ciura(v3, n);
    printf("Shell (Ciura):         "); imprimir(v3, n);

    return 0;
}
