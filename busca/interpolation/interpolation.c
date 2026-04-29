/**
 * Busca por interpolação no vetor ordenado vector. Em vez de sempre dividir
 * o espaço de busca ao meio (como na busca binária), estima a posição do
 * item com base em uma interpolação linear entre os extremos, assumindo que
 * os valores estão distribuídos uniformemente.
 *
 * Fórmula da posição estimada:
 *   pos = begin + ((item - vector[begin]) * (end - begin))
 *                / (vector[end] - vector[begin])
 *
 * Ao encontrar o item retorna seu índice. Caso contrário retorna -1.
 *
 * Complexidade:
 *  - Melhor/médio caso (distribuição uniforme): O(log log n)
 *  - Pior caso (distribuição não uniforme):     O(n)
 *
 * Vantagens:
 *  - Muito mais rápido que busca binária para dados uniformemente distribuídos
 *  - Menos comparações em média
 *
 * Desvantagens:
 *  - Requer vetor ordenado
 *  - Degrada para O(n) se a distribuição for muito irregular
 *  - Mais complexo de implementar corretamente
 */

int interpolation_search(int vector[], int n, int item) {
  int begin = 0;
  int end = n - 1;

  while (begin <= end && item >= vector[begin] && item <= vector[end]) {
    if (begin == end) {
      // Espaço de busca colapsou em um único elemento
      return (vector[begin] == item) ? begin : -1;
    }

    // Estima a posição proporcional ao valor buscado
    int pos = begin + ((long)(item - vector[begin]) * (end - begin))
                      / (vector[end] - vector[begin]);

    if (vector[pos] == item) {
      return pos; // Item encontrado
    }

    if (vector[pos] < item) {
      begin = pos + 1; // Item está à direita da estimativa
    } else {
      end = pos - 1;   // Item está à esquerda da estimativa
    }
  }

  return -1; // Item não encontrado
}
