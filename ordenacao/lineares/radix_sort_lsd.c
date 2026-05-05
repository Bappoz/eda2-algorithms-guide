/*
 * Radix Sort LSD (Least Significant Digit first)
 *
 * Ideia: ordena digito por digito, do MENOS significativo para o MAIS
 * significativo, usando Counting Sort ESTAVEL como sub-rotina em cada digito.
 *
 * Por que funciona:
 *   - A estabilidade garante que ao ordenar pelo digito i, a ordem dos
 *     digitos j < i (ja ordenados anteriormente) e preservada.
 *   - Apos processar todos os d digitos, o vetor esta totalmente ordenado.
 *
 * Exemplo com base 10 (digitos decimais):
 *   Original: [329, 457, 657, 839, 436, 720, 355]
 *
 *   Passo 1 (unidade):  [720, 355, 436, 457, 657, 329, 839]
 *   Passo 2 (dezena):   [720, 329, 436, 839, 355, 457, 657]
 *   Passo 3 (centena):  [329, 355, 436, 457, 657, 720, 839]
 *
 * Complexidade:
 *   Tempo:  O(d * (n + b))  onde d = numero de digitos, b = base
 *   Espaco: O(n + b)
 *
 * Se d e constante e b = O(n):  O(n)  -- linear!
 * Para inteiros de 32 bits com base 256: d = 4, b = 256 -> O(4*(n+256)) ~ O(n)
 *
 * Estavel: SIM (herda do Counting Sort)
 *
 * Vantagem sobre MSD:
 *   - Implementacao mais simples (nao e recursiva)
 *   - Nao precisa de alocacoes dinamicas por nivel
 *   - Melhor localidade de memoria (acesso sequencial)
 *
 * Desvantagem vs MSD:
 *   - Nao pode ser usado para ordenar strings de comprimentos variados de
 *     forma direta (MSD lida com isso naturalmente)
 *   - Nao permite busca ou ordenacao parcial
 *
 * Base ideal: b = max(2, n) minimiza d*(n+b), mas b=256 (byte) e pratico
 * pois permite usar deslocamentos de bit ao inves de divisao/modulo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* Counting Sort estavel por um unico digito (extraido pela exp: 1, 10, 100...)
 * Ordena A[] com base no digito na posicao 'exp' (1=unidade, 10=dezena, ...) */
void counting_sort_digito(int A[], int n, int exp) {
    int *B = malloc(n * sizeof(int));  /* vetor de saida */
    int C[10] = {0};                   /* base 10: digitos 0..9 */

    /* Conta ocorrencias do digito atual */
    for (int i = 0; i < n; i++)
        C[(A[i] / exp) % 10]++;

    /* Prefixo acumulado */
    for (int k = 1; k < 10; k++)
        C[k] += C[k-1];

    /* Coloca de tras para frente para manter estabilidade */
    for (int i = n - 1; i >= 0; i--) {
        int d = (A[i] / exp) % 10;
        B[C[d] - 1] = A[i];
        C[d]--;
    }

    memcpy(A, B, n * sizeof(int));
    free(B);
}

void radix_sort_lsd(int A[], int n) {
    /* Encontra o maximo para saber quantos digitos processar */
    int mx = A[0];
    for (int i = 1; i < n; i++) if (A[i] > mx) mx = A[i];

    /* Processa cada digito: unidade, dezena, centena, ... */
    for (int exp = 1; mx / exp > 0; exp *= 10)
        counting_sort_digito(A, n, exp);
}

/*
 * Versao otimizada: base 256 (processa byte a byte).
 * Para inteiros de 32 bits, sao apenas 4 passagens (d=4, b=256).
 * Usa deslocamento de bit em vez de divisao, muito mais rapido.
 */
void radix_sort_lsd_base256(int A[], int n) {
    int *B = malloc(n * sizeof(int));
    int C[256];

    for (int shift = 0; shift < 32; shift += 8) {
        memset(C, 0, sizeof(C));

        for (int i = 0; i < n; i++)
            C[(A[i] >> shift) & 0xFF]++;

        for (int k = 1; k < 256; k++)
            C[k] += C[k-1];

        for (int i = n - 1; i >= 0; i--) {
            int d = (A[i] >> shift) & 0xFF;
            B[C[d] - 1] = A[i];
            C[d]--;
        }
        memcpy(A, B, n * sizeof(int));
    }
    free(B);
}

int main() {
    int v1[] = {329, 457, 657, 839, 436, 720, 355, 1, 98, 12};
    int v2[] = {329, 457, 657, 839, 436, 720, 355, 1, 98, 12};
    int n = 10;

    printf("Original:           "); imprimir(v1, n);

    radix_sort_lsd(v1, n);
    printf("Radix LSD (base10): "); imprimir(v1, n);

    radix_sort_lsd_base256(v2, n);
    printf("Radix LSD (base256):"); imprimir(v2, n);

    return 0;
}
