/*
 * Radix Sort MSD (Most Significant Digit first)
 *
 * Ideia: ordena digito por digito, do MAIS significativo para o MENOS
 * significativo. Apos separar em buckets pelo digito mais significativo,
 * aplica MSD recursivamente DENTRO de cada bucket.
 *
 * Diferenca fundamental do LSD:
 *   - LSD: processa TODO o vetor d vezes (d passagens globais)
 *   - MSD: divide em sub-problemas independentes (recursao), processando
 *     apenas os elementos dentro de cada bucket em cada nivel.
 *
 * Exemplo com strings de 3 digitos (base 10):
 *   Original: [329, 457, 657, 839, 436, 720, 355]
 *
 *   Buckets pelo digito das centenas (3,4,5,6,7,8):
 *     3xx: [329]
 *     4xx: [457]
 *     5xx: [355]        <- recursao dentro deste bucket
 *     6xx: [657, 636]   <- recursao dentro deste bucket
 *     7xx: [720]
 *     8xx: [839]
 *
 *   Dentro de 6xx, ordena pelas dezenas...
 *   Dentro de 65x, ordena pelas unidades...
 *
 * Complexidade:
 *   Tempo medio: O(d * n)  -- cada digito de cada elemento e processado 1x
 *   Pior caso:   O(d * (n + b))  -- todos caem no mesmo bucket em cada nivel
 *
 * Vantagens do MSD sobre LSD:
 *   1. Suporta strings de comprimento VARIAVEL naturalmente -- para quando o
 *      bucket tem apenas 1 elemento (ja ordenado)
 *   2. Pode ser LAZY: para cedo quando um bucket tem 1 elemento ou quando
 *      a profundidade atingiu o comprimento da string mais curta
 *   3. Permite busca por prefixo: ordena apenas ate o nivel necessario
 *   4. Paralelizavel: os buckets no mesmo nivel sao independentes
 *
 * Desvantagens:
 *   - Mais complexo de implementar (recursao, particao em buckets)
 *   - Pode usar mais memoria (pilha de recursao + buckets intermediarios)
 *   - Para inteiros de tamanho fixo, LSD geralmente e mais rapido na pratica
 *
 * Implementado aqui:
 *   - Versao 1: MSD para inteiros em base 10 (recursiva)
 *   - Versao 2: MSD para strings em base 256 (American Flag Sort)
 *     - American Flag Sort: variante in-place do MSD que nao usa buffer,
 *       inspirada no algoritmo de tres vias de Dijkstra
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ============================================================
 * Versao 1: MSD para inteiros em base 10
 * ============================================================ */

/* Retorna o digito na posicao 'exp' do numero 'x' */
static int digito(int x, int exp) { return (x / exp) % 10; }

/* Encontra o maior valor para calcular o numero de digitos */
static int maximo(int v[], int n) {
    int mx = v[0];
    for (int i = 1; i < n; i++) if (v[i] > mx) mx = v[i];
    return mx;
}

void msd_recursivo(int A[], int n, int exp) {
    if (n <= 1 || exp == 0) return;  /* base: 1 elemento ou sem mais digitos */

    int *B = malloc(n * sizeof(int));
    int C[10] = {0};

    /* Conta e acumula */
    for (int i = 0; i < n; i++) C[digito(A[i], exp)]++;
    int inicio[10];
    inicio[0] = 0;
    for (int k = 1; k < 10; k++) inicio[k] = inicio[k-1] + C[k-1];

    /* Copia contagens para controle de posicao */
    int pos[10];
    memcpy(pos, inicio, sizeof(pos));

    /* Distribui nos buckets */
    for (int i = 0; i < n; i++) {
        int d = digito(A[i], exp);
        B[pos[d]++] = A[i];
    }
    memcpy(A, B, n * sizeof(int));
    free(B);

    /* Recursao em cada bucket nao vazio */
    for (int k = 0; k < 10; k++)
        if (C[k] > 1)
            msd_recursivo(A + inicio[k], C[k], exp / 10);
}

void radix_sort_msd(int A[], int n) {
    /* Calcula a maior potencia de 10 necessaria */
    int mx = maximo(A, n);
    int exp = 1;
    while (mx / exp >= 10) exp *= 10;

    msd_recursivo(A, n, exp);
}

/* ============================================================
 * Versao 2: American Flag Sort (MSD in-place para strings)
 *
 * Ordena um array de strings em ordem lexicografica usando MSD
 * sem buffer auxiliar. Usa a ideia de "cyclic rotation" do 3-way
 * partition para colocar cada string no bucket certo in-place.
 * ============================================================ */
#define ALPHA 256  /* base: todos os bytes ASCII */

/* Retorna o caractere na posicao 'pos' da string (0 = fim da string) */
static int char_em(const char *s, int pos) {
    return (unsigned char)s[pos];
}

void american_flag_sort(const char *A[], int n, int pos) {
    if (n <= 1) return;

    /* 1. Conta ocorrencias de cada byte na posicao 'pos' */
    int C[ALPHA + 1] = {0};
    for (int i = 0; i < n; i++) C[char_em(A[i], pos) + 1]++;

    /* 2. Prefixo acumulado -> inicio de cada bucket */
    int inicio[ALPHA + 1];
    inicio[0] = 0;
    for (int k = 1; k <= ALPHA; k++)
        inicio[k] = inicio[k-1] + C[k];

    /* 3. Permutacao ciclica in-place (American Flag) */
    int cur[ALPHA + 1];
    memcpy(cur, inicio, sizeof(inicio));

    for (int k = 0; k < ALPHA; k++) {
        while (cur[k] < inicio[k+1]) {
            int d = char_em(A[cur[k]], pos);
            if (d == k) {
                cur[k]++;
            } else {
                /* Troca A[cur[k]] com A[cur[d]] */
                const char *tmp = A[cur[k]];
                A[cur[k]] = A[cur[d]];
                A[cur[d]] = tmp;
                cur[d]++;
            }
        }
    }

    /* 4. Recursao em cada bucket (exceto o bucket '\0' que ja esta ordenado) */
    for (int k = 1; k <= ALPHA; k++) {
        int tam = inicio[k] - inicio[k-1];
        if (tam > 1)
            american_flag_sort(A + inicio[k-1], tam, pos + 1);
    }
}

int main() {
    /* Teste 1: inteiros */
    int v[] = {329, 457, 657, 839, 436, 720, 355, 1, 98, 12, 4521};
    int n = 11;
    printf("Original (inteiros): "); imprimir(v, n);
    radix_sort_msd(v, n);
    printf("Radix MSD:           "); imprimir(v, n);

    /* Teste 2: strings (American Flag Sort) */
    const char *palavras[] = {"banana", "abacate", "caju", "abacaxi",
                               "coco", "acerola", "abacate", "mamao"};
    int m = 8;
    printf("\nStrings originais:\n  ");
    for (int i = 0; i < m; i++) printf("%s  ", palavras[i]);
    printf("\n");

    american_flag_sort(palavras, m, 0);

    printf("American Flag Sort (MSD):\n  ");
    for (int i = 0; i < m; i++) printf("%s  ", palavras[i]);
    printf("\n");

    return 0;
}
