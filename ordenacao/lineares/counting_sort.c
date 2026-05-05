/*
 * Counting Sort (Ordenacao por Contagem)
 *
 * Nao e baseado em comparacoes -- rompe a barreira teorica de O(n log n)
 * dos algoritmos de comparacao!
 *
 * Ideia: conta quantas vezes cada valor aparece no vetor (vetor de
 * contagem C[k]). Depois acumula as contagens (prefixo) para saber a
 * posicao correta de cada elemento no vetor de saida.
 *
 *   Entrada: A = [4, 2, 2, 8, 3, 3, 1]  (valores em [0..K])
 *   K = 8
 *
 *   Passo 1 - Contar:
 *     C = [0, 1, 2, 2, 1, 0, 0, 0, 1]
 *          0  1  2  3  4  5  6  7  8
 *
 *   Passo 2 - Prefixo acumulado:
 *     C = [0, 1, 3, 5, 6, 6, 6, 6, 7]
 *     C[k] = numero de elementos <= k = posicao APOS o ultimo k no resultado
 *
 *   Passo 3 - Colocar no lugar (de tras para frente, para ser ESTAVEL):
 *     A[6]=1 -> pos C[1]-1=0 -> B[0]=1,  C[1]=0
 *     A[5]=3 -> pos C[3]-1=4 -> B[4]=3,  C[3]=4
 *     ...
 *     B = [1, 2, 2, 3, 3, 4, 8]
 *
 * Complexidade:
 *   Tempo:  O(n + K)  onde K = valor maximo
 *   Espaco: O(n + K)  para o vetor de saida e o de contagem
 *
 * Estavel: SIM (passo 3 percorre de tras para frente)
 *   Essencial quando Counting Sort e usado como sub-rotina do Radix Sort.
 *
 * Restricoes:
 *   - Funciona APENAS para inteiros (ou mapeados a inteiros)
 *   - K deve ser pequeno; se K >> n o algoritmo fica ineficiente
 *     (ex: K = 10^9 com n = 100 seria absurdo)
 *
 * Variante: quando os valores podem ser negativos, desloca todos os
 * elementos para que o minimo seja 0 antes de contar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* Counting Sort para valores em [0..K] */
void counting_sort(int A[], int B[], int n, int K) {
    int *C = calloc(K + 1, sizeof(int));

    /* Passo 1: conta ocorrencias */
    for (int i = 0; i < n; i++) C[A[i]]++;

    /* Passo 2: prefixo acumulado */
    for (int k = 1; k <= K; k++) C[k] += C[k-1];

    /* Passo 3: coloca no vetor de saida (de tras para frente = estavel) */
    for (int i = n - 1; i >= 0; i--) {
        B[C[A[i]] - 1] = A[i];
        C[A[i]]--;
    }

    free(C);
}

/*
 * Variante: suporte a valores negativos.
 * Desloca todos os elementos em -minimo para mapear ao range [0..K].
 */
void counting_sort_com_negativos(int A[], int B[], int n) {
    /* Encontra min e max */
    int mn = A[0], mx = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] < mn) mn = A[i];
        if (A[i] > mx) mx = A[i];
    }
    int K = mx - mn;

    int *C = calloc(K + 1, sizeof(int));
    for (int i = 0; i < n; i++) C[A[i] - mn]++;
    for (int k = 1; k <= K; k++) C[k] += C[k-1];
    for (int i = n - 1; i >= 0; i--) {
        B[C[A[i] - mn] - 1] = A[i];
        C[A[i] - mn]--;
    }
    free(C);
}

int main() {
    /* Caso 1: valores positivos */
    int A[] = {4, 2, 2, 8, 3, 3, 1, 7, 5};
    int n = 9, K = 8;
    int B[9];

    printf("Original:           "); imprimir(A, n);
    counting_sort(A, B, n, K);
    printf("Counting Sort:      "); imprimir(B, n);

    /* Caso 2: valores com negativos */
    int C[] = {-3, 1, 0, -1, 2, -2, 3};
    int n2 = 7;
    int D[7];

    printf("\nCom negativos:      "); imprimir(C, n2);
    counting_sort_com_negativos(C, D, n2);
    printf("Counting Sort:      "); imprimir(D, n2);

    return 0;
}
