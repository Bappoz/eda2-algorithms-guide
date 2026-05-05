/*
 * Bucket Sort (Ordenacao por Baldes / Bin Sort)
 *
 * Ideia: distribui os elementos em K "baldes" (buckets) de acordo com
 * alguma funcao de mapeamento. Cada balde e ordenado individualmente
 * (geralmente com Insertion Sort, pois espera-se que fique pequeno).
 * Por fim, concatena todos os baldes.
 *
 * Pressuposto fundamental: os elementos estao distribuidos de forma
 * UNIFORME no intervalo [0, 1) (ou [min, max] apos normalizacao).
 * Com distribuicao uniforme, cada balde tem em media n/K elementos.
 *
 * Complexidade:
 *   Caso medio (distribuicao uniforme): O(n + K)
 *   Pior caso (todos no mesmo bucket): O(n^2)  -- Insertion Sort em n elems
 *   Espaco: O(n + K)
 *
 * Estavel: SIM, se o algoritmo interno for estavel (Insertion Sort e)
 *
 * Funcao de mapeamento:
 *   Para floats em [0,1): bucket(x) = (int)(x * K)
 *   Para inteiros em [min,max]: bucket(x) = (int)((x-min) * K / (max-min+1))
 *
 * Aplicacoes:
 *   - Ordenacao de notas/percentuais (0.0 a 1.0)
 *   - Ordenacao de coordenadas geograficas
 *   - Complemento ao Radix Sort para dados nao inteiros
 *
 * Variante: Proxmap Sort (PROXimity MAP Sort)
 *   Variacao que calcula o mapeamento com base em um "proximo mapa"
 *   dos elementos, garantindo distribuicao mais uniforme.
 *   Complexidade media O(n), pior caso O(n^2).
 */

#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * Versao 1: Bucket Sort para floats em [0, 1)
 * ============================================================ */

typedef struct No {
    float val;
    struct No *prox;
} No;

/* Insertion Sort numa lista ligada */
No *insertion_sort_lista(No *head) {
    if (!head || !head->prox) return head;
    No *sorted = NULL;
    while (head) {
        No *cur = head;
        head = head->prox;
        /* Insere cur na posicao correta em sorted */
        if (!sorted || cur->val <= sorted->val) {
            cur->prox = sorted;
            sorted = cur;
        } else {
            No *p = sorted;
            while (p->prox && p->prox->val < cur->val) p = p->prox;
            cur->prox = p->prox;
            p->prox = cur;
        }
    }
    return sorted;
}

void bucket_sort_float(float A[], int n) {
    No **baldes = calloc(n, sizeof(No *));

    /* Distribui nos baldes */
    for (int i = 0; i < n; i++) {
        int b = (int)(A[i] * n);  /* bucket de 0 a n-1 */
        No *no = malloc(sizeof(No));
        no->val = A[i];
        no->prox = baldes[b];
        baldes[b] = no;
    }

    /* Ordena cada balde e concatena */
    int k = 0;
    for (int b = 0; b < n; b++) {
        baldes[b] = insertion_sort_lista(baldes[b]);
        No *cur = baldes[b];
        while (cur) {
            A[k++] = cur->val;
            No *tmp = cur;
            cur = cur->prox;
            free(tmp);
        }
    }
    free(baldes);
}

/* ============================================================
 * Versao 2: Bucket Sort para inteiros em [min, max]
 * Usa vetores dinamicos por bucket em vez de listas ligadas
 * (melhor localidade de cache)
 * ============================================================ */

/* Insertion Sort em subvetor v[esq..dir] */
static void ins_sort(int v[], int esq, int dir) {
    for (int i = esq + 1; i <= dir; i++) {
        int chave = v[i];
        int j = i - 1;
        while (j >= esq && v[j] > chave) { v[j+1] = v[j]; j--; }
        v[j+1] = chave;
    }
}

void bucket_sort_int(int A[], int n, int K) {
    /* Encontra min e max */
    int mn = A[0], mx = A[0];
    for (int i = 1; i < n; i++) {
        if (A[i] < mn) mn = A[i];
        if (A[i] > mx) mx = A[i];
    }
    int range = mx - mn + 1;

    /* Aloca K baldes como vetores dinamicos */
    int **baldes = calloc(K, sizeof(int *));
    int *tamanhos = calloc(K, sizeof(int));
    int *capacidades = calloc(K, sizeof(int));

    for (int i = 0; i < n; i++) {
        int b = (int)((long)(A[i] - mn) * K / range);
        if (b >= K) b = K - 1;  /* trata o caso extremo do maximo */
        if (tamanhos[b] == capacidades[b]) {
            capacidades[b] = capacidades[b] ? capacidades[b] * 2 : 4;
            baldes[b] = realloc(baldes[b], capacidades[b] * sizeof(int));
        }
        baldes[b][tamanhos[b]++] = A[i];
    }

    /* Ordena cada balde e concatena */
    int k = 0;
    for (int b = 0; b < K; b++) {
        if (tamanhos[b] > 1) ins_sort(baldes[b], 0, tamanhos[b] - 1);
        for (int j = 0; j < tamanhos[b]; j++) A[k++] = baldes[b][j];
        free(baldes[b]);
    }
    free(baldes); free(tamanhos); free(capacidades);
}

int main() {
    /* Teste 1: floats em [0, 1) */
    float fv[] = {0.78f, 0.17f, 0.39f, 0.26f, 0.72f, 0.94f, 0.21f, 0.12f};
    int fn = 8;
    printf("Floats originais:   ");
    for (int i = 0; i < fn; i++) printf("%.2f ", fv[i]);
    printf("\n");
    bucket_sort_float(fv, fn);
    printf("Bucket Sort:        ");
    for (int i = 0; i < fn; i++) printf("%.2f ", fv[i]);
    printf("\n");

    /* Teste 2: inteiros */
    int iv[] = {42, 17, 63, 5, 89, 31, 78, 56, 24, 11};
    int in = 10, K = 5;
    printf("\nInteiros originais: ");
    for (int i = 0; i < in; i++) printf("%d ", iv[i]);
    printf("\n");
    bucket_sort_int(iv, in, K);
    printf("Bucket Sort (K=%d): ", K);
    for (int i = 0; i < in; i++) printf("%d ", iv[i]);
    printf("\n");

    return 0;
}
