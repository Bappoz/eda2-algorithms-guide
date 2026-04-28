/*
 * Complexidade:
 *    - Pior Caso: O(n)
 *    - Melhor Caso: O(1)
 *    - Caso Médio: O((n+1)/2)
 *
 * Sentinela elmina a verificação de fim de lista, colocando o valor buscado na
 * última posição do vetor. Simples e ligeiramente mais rápida que a busca
 * sequencial básica, pois elimina a necessidade de verificar o fim da lista em
 * cada iteração.
 *
 */

#include <stdio.h>

int busca_com_sentinela(float v, int n, float A[]) {
  // Coloca a chave na posição de sentinela
  A[n] = v;

  int i = 0;
  while (A[i] != v) {
    i++;
  }

  if (i < n) {
    return i;
  } else {
    return -1;
  }
}

int main() {
  float vetor[] = {10.5, 20.3, 30.1, 40.9, 50.0};
  int n = 5;
  float chave = 30.1;

  int resultado = busca_com_sentinela(chave, n, vetor);

  if (resultado != -1) {
    printf("Elemento encontrado na posição: %d\n", resultado);
  } else {
    printf("Elemento não encontrado.\n");
  }

  return 0;
}
