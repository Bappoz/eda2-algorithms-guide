/**
 * Busca iterativamente o item dentro do vetor vector. A cada iteração
 * reduz o espaço de busca pela metade sem fazer chamadas recursivas.
 * Ao encontrar o item retorna seu índice. Caso contrário retorna -1
 *
 * Complexidade: O(log n) => caso médio
 *
 * Vantagens:
 *  - Eficiência na Busca
 *  - simplicidade na implementação
 *
 * Desvantagens:
 *  - Nem todo arranjo está ordenado
 *  - Inserção e remoção de elementos são ineficientes
 */

int iterative_binary_search(int vector[], int n, int item) {
  int begin = 0;
  int end = n - 1;

  while (begin <= end) {
    int i = (begin + end) / 2;

    if (vector[i] == item) {
      return i;
    }

    if (vector[i] < item) {
      begin = i + 1;
    } else {
      end = i;
    }
  }
  return -1;
}
