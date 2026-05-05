/*
 * Heap Sort (Ordenacao por Heap)
 *
 * Usa a estrutura de dados Heap Binario Maximo (Max-Heap) para ordenar.
 *
 * Um Max-Heap e uma arvore binaria quase completa onde cada pai e maior
 * ou igual a seus filhos. Representado em vetor:
 *   - pai de i    : (i-1)/2
 *   - filho esq   : 2*i + 1
 *   - filho dir   : 2*i + 2
 *
 * Algoritmo em duas fases:
 *
 *   Fase 1 - HEAPIFY (construcao do heap): O(n)
 *     Transforma o vetor desordenado em um Max-Heap. Aplica sift-down
 *     de baixo para cima, comecando no ultimo no interno (n/2 - 1).
 *
 *   Fase 2 - EXTRACAO: O(n log n)
 *     Repete n-1 vezes:
 *       a) Troca v[0] (maximo) com v[ultimo]  --> coloca maximo no lugar certo
 *       b) Reduz o tamanho do heap em 1
 *       c) Restaura a propriedade de heap (sift-down em v[0])
 *
 *   Resultado: vetor ordenado de forma crescente.
 *
 * Complexidade:
 *   Melhor, medio e pior: O(n log n) -- SEMPRE garantido
 *   Espaco: O(1) (in-place, sem memoria auxiliar)
 *
 * Estavel: NAO (trocas de longa distancia podem reordenar iguais)
 *
 * Comparacao com Quick Sort:
 *   - Heap Sort garante O(n log n) no pior caso (Quick Sort nao)
 *   - Quick Sort e geralmente mais rapido na pratica (melhor cache locality)
 *   - Heap Sort e a base do Introsort: Quick Sort que troca para Heap Sort
 *     quando a profundidade de recursao ultrapassa 2*log(n)
 *
 * Variante: Smooth Sort (Dijkstra) -- heap de Leonardo, mais eficiente para
 * sequencias quase ordenadas, mas muito mais complexo de implementar.
 */

#include <stdio.h>

void trocar(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void imprimir(int v[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/*
 * sift_down: restaura a propriedade de Max-Heap na subarvore com raiz em i,
 * considerando que o heap vai de 0 ate (tamanho-1).
 * "Afunda" o elemento v[i] enquanto ele for menor que algum filho.
 */
void sift_down(int v[], int i, int tamanho) {
    while (1) {
        int maior = i;
        int esq = 2*i + 1;
        int dir = 2*i + 2;

        if (esq < tamanho && v[esq] > v[maior]) maior = esq;
        if (dir < tamanho && v[dir] > v[maior]) maior = dir;

        if (maior == i) break;
        trocar(&v[i], &v[maior]);
        i = maior;
    }
}

void heap_sort(int v[], int n) {
    /* Fase 1: Build Max-Heap em O(n)
     * Começa no ultimo no interno e aplica sift-down de baixo para cima. */
    for (int i = n/2 - 1; i >= 0; i--)
        sift_down(v, i, n);

    /* Fase 2: Extrai o maximo n-1 vezes */
    for (int tam = n - 1; tam > 0; tam--) {
        trocar(&v[0], &v[tam]);  /* maximo vai para a posicao final */
        sift_down(v, 0, tam);    /* restaura heap no restante */
    }
}

/* Imprime o heap como arvore para visualizacao (ate 15 elementos) */
void imprimir_heap(int v[], int n) {
    int nivel = 0, count = 1;
    printf("Heap (Max): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
        if (i + 1 == count - 1 + (1 << nivel)) {
            nivel++;
            count += (1 << nivel);
        }
    }
    printf("\n");
}

int main() {
    int v[] = {12, 11, 13, 5, 6, 7, 3, 1, 9, 8};
    int n = 10;

    printf("Original:   "); imprimir(v, n);

    /* Mostra o heap apos a fase 1 */
    int heap[10];
    for (int i = 0; i < n; i++) heap[i] = v[i];
    for (int i = n/2 - 1; i >= 0; i--)
        sift_down(heap, i, n);
    imprimir_heap(heap, n);

    heap_sort(v, n);
    printf("Heap Sort:  "); imprimir(v, n);

    return 0;
}
