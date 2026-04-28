
/*
 * Complexidade: O(n) no pior caso, onde n é o número de elementos no vetor.
 * No entanto, o desempenho pode melhorar significativamente para elementos que
 * são buscados com mais frequência, pois eles serão movidos para posições mais
 * próximas do início do vetor.
 *
 *  O método de Busca usando a técnica de "Mover para Frente" (Move to Front) é
 * uma variação da busca sequencial que, ao encontrar o elemento buscado, o move
 * para a frente do vetor. Isso pode melhorar o desempenho em casos onde certos
 * elementos são buscados com mais frequência, pois eles serão movidos para
 * posições mais próximas do início do vetor, reduzindo o tempo de busca para
 * esses elementos no futuro.
 *
 */

#include <stdio.h>

int mover_para_frente(float v, int n, float A[]) {
  for (int i = 0; i < n; i++) {
    if (A[i] == v) {
      // Move o elemento encontrado para a frente do vetor
      float temp = A[i];
      for (int j = i; j > 0; j--) {
        A[j] = A[j - 1];
      }
      A[0] = temp;
      return i; // Retorna a posição original do elemento encontrado
    }
  }
  return -1; // Elemento não encontrado
}

int main() {
  float vetor[] = {10.5, 20.3, 30.1, 40.9, 50.0};
  int n = 5;
  float chave = 30.1;

  int resultado = mover_para_frente(chave, n, vetor);

  if (resultado != -1) {
    printf("Elemento encontrado na posição original: %d\n", resultado);
    printf("Vetor após mover para frente: ");
    for (int i = 0; i < n; i++) {
      printf("%.1f ", vetor[i]);
    }
    printf("\n");
  } else {
    printf("Elemento não encontrado.\n");
  }

  return 0;
}
