/*
 * Insertion Sort (Ordenacao por Insercao)
 *
 * Ideia: semelhante a ordenar cartas na mao. Mantemos um subvetor ja
 * ordenado a esquerda e inserimos um por um os elementos da parte direita
 * na posicao correta do subvetor ordenado, deslocando os maiores para a
 * direita.
 *
 *   [5, 2, 4, 6, 1, 3]
 *   i=1: chave=2  -> [2, 5, 4, 6, 1, 3]
 *   i=2: chave=4  -> [2, 4, 5, 6, 1, 3]
 *   i=3: chave=6  -> [2, 4, 5, 6, 1, 3]
 *   i=4: chave=1  -> [1, 2, 4, 5, 6, 3]
 *   i=5: chave=3  -> [1, 2, 3, 4, 5, 6]
 *
 * Complexidade:
 *   Melhor caso (ja ordenado): O(n)       -- apenas comparacoes, sem trocas
 *   Caso medio               : O(n^2)
 *   Pior caso (ordem inversa): O(n^2) comparacoes e deslocamentos
 *
 * Espaco: O(1) (in-place)
 * Estavel: SIM
 *
 * Vantagens:
 *   - Eficiente para vetores pequenos ou quase ordenados
 *   - Adaptativo: numero de operacoes reduz conforme o vetor e mais ordenado
 *   - Usado internamente por algoritmos hibridos (Timsort, Introsort) para
 *     subarrays pequenos (tipicamente n < 16 a 32)
 *
 * Variante: Insertion Sort com Busca Binaria (Binary Insertion Sort)
 *   Usa busca binaria para encontrar a posicao de insercao em O(log n), mas
 *   os deslocamentos ainda sao O(n), portanto a complexidade total nao muda.
 *   Util quando comparacoes sao caras mas deslocamentos sao baratos.
 */

#include <stdio.h>

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ---- Versao 1: classica ---- */
void insertion_sort(int v[], int n) {
    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int j = i - 1;
        /* Desloca elementos maiores que 'chave' uma posicao para a direita */
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

/*
 * ---- Versao 2: Binary Insertion Sort ----
 *
 * Usa busca binaria para encontrar o ponto de insercao, reduzindo
 * comparacoes de O(n) para O(log n) por elemento.
 * Os deslocamentos permanecem O(n), entao a complexidade total e O(n^2).
 */
int busca_binaria_insercao(int v[], int esq, int dir, int chave) {
    while (esq < dir) {
        int meio = esq + (dir - esq) / 2;
        if (v[meio] <= chave) esq = meio + 1;
        else                  dir = meio;
    }
    return esq;
}

void binary_insertion_sort(int v[], int n) {
    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int pos = busca_binaria_insercao(v, 0, i, chave);
        /* Desloca [pos..i-1] uma posicao para a direita */
        for (int j = i; j > pos; j--)
            v[j] = v[j - 1];
        v[pos] = chave;
    }
}

int main() {
    int v1[] = {5, 2, 4, 6, 1, 3, 9, 7};
    int v2[] = {5, 2, 4, 6, 1, 3, 9, 7};
    int n = 8;

    printf("Original:             "); imprimir(v1, n);

    insertion_sort(v1, n);
    printf("Insertion (classico): "); imprimir(v1, n);

    binary_insertion_sort(v2, n);
    printf("Insertion (binario):  "); imprimir(v2, n);

    /* Melhor caso: vetor ja ordenado */
    int sorted[] = {1, 2, 3, 4, 5, 6, 7, 8};
    printf("\nJa ordenado - O(n) passagens:\n");
    printf("Antes: "); imprimir(sorted, 8);
    insertion_sort(sorted, 8);
    printf("Depois:"); imprimir(sorted, 8);

    return 0;
}
