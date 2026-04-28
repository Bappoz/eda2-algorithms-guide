

/*
 * Complexidade de tempo: O(n) no pior caso, onde n é o número de elementos no
 * vetor. No entanto, em casos onde certos elementos são buscados com mais
 * frequência, a complexidade pode ser melhorada para O(1) para esses elementos,
 * devido à técnica de transposição.
 *
 * O metodo de Busca com a técnica de "Transposição" é uma variação da busca
 * sequencial que, ao encontrar o elemento buscado, o troca de posição com o
 * elemento anterior no vetor. Isso pode melhorar o desempenho em casos onde
 * certos elementos são buscados com mais frequência, pois eles serão movidos
 * para posições mais próximas do início do vetor, reduzindo o tempo de busca
 * para esses elementos no futuro.
 */

#include <stdio.h>

int busca_com_transposicao(float v, int n, float A[]) {
  for (int i = 0; i < n; i++) {
    if (A[i] == v) {
      // Transpõe o elemento encontrado com o elemento anterior
      if (i > 0) {
        float temp = A[i];
        A[i] = A[i - 1];
        A[i - 1] = temp;
      }
      return i; // Retorna a posição original do elemento encontrado
    }
  }
}

int main() {
  float vetor[] = {10.5, 20.3, 30.1, 40.9, 50.0};
  int n = 5;
  float chave = 50.0;

  int resultado = busca_com_transposicao(chave, n, vetor);

  if (resultado != -1) {
    printf("Elemento encontrado na posição original: %d\n", resultado);
    printf("Vetor após transposição: ");
    for (int i = 0; i < n; i++) {
      printf("%.1f ", vetor[i]);
    }
    printf("\n");
  } else {
    printf("Elemento não encontrado.\n");
  }

  return 0;
}
