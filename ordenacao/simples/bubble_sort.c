/*
 * Bubble Sort (Ordenacao por Bolha)
 *
 * Ideia: compara pares adjacentes e os troca se estiverem fora de ordem.
 * A cada passagem, o maior elemento "flutua" ate a sua posicao final, como
 * uma bolha subindo. Apos k passagens, os k maiores elementos ja estao
 * corretamente posicionados no final do vetor.
 *
 * Complexidade:
 *   Pior caso  (ordem inversa): O(n^2) comparacoes, O(n^2) trocas
 *   Caso medio               : O(n^2)
 *   Melhor caso (ja ordenado): O(n)  -- com a otimizacao da flag 'trocou'
 *
 * Espaco: O(1) (in-place)
 * Estavel: SIM (nao troca elementos iguais)
 *
 * Versao 1: classica (sem otimizacao) - sempre faz n*(n-1)/2 comparacoes
 * Versao 2: com flag 'trocou' - para cedo se o vetor ja estiver ordenado
 *
 * Na pratica, Bubble Sort e mais lento que Insertion Sort para a maioria dos
 * casos, pois faz mais trocas. E primariamente didatico.
 */

#include <stdio.h>

/* Troca dois inteiros */
void trocar(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ---- Versao 1: classica ---- */
void bubble_sort(int v[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (v[j] > v[j + 1])
                trocar(&v[j], &v[j + 1]);
}

/*
 * ---- Versao 2: com flag de parada precoce ----
 *
 * Se uma passagem completa nao gerou nenhuma troca, o vetor ja esta
 * ordenado e podemos parar. Reduz o numero de passagens no caso medio.
 * No melhor caso (vetor ja ordenado), faz apenas 1 passagem -> O(n).
 */
void bubble_sort_otimizado(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (v[j] > v[j + 1]) {
                trocar(&v[j], &v[j + 1]);
                trocou = 1;
            }
        }
        if (!trocou) break;  /* nenhuma troca -> ja ordenado */
    }
}

int main() {
    int v1[] = {64, 34, 25, 12, 22, 11, 90};
    int v2[] = {64, 34, 25, 12, 22, 11, 90};
    int n = 7;

    printf("Original:          "); imprimir(v1, n);

    bubble_sort(v1, n);
    printf("Bubble (classico): "); imprimir(v1, n);

    bubble_sort_otimizado(v2, n);
    printf("Bubble (otimizado):"); imprimir(v2, n);

    /* Demonstra vantagem da flag no melhor caso */
    int sorted[] = {1, 2, 3, 4, 5};
    printf("\nJa ordenado: "); imprimir(sorted, 5);
    bubble_sort_otimizado(sorted, 5);
    printf("Apos 1 passagem:   "); imprimir(sorted, 5);

    return 0;
}
