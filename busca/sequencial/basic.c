/*
 * Complexidade:
 *    - Pior Caso: O(n)
 *    - Melhor Caso: O(1)
 *    - Caso Médio: O((n+1)/2)
 *
 * Percorre registros de forma sequencial, comparando cada um deles com o
 * valor buscado até encontrar uma correspondência ou atingir o final da lista.
 *
 * Mais Simples Busca possível.
 */

#include <stdio.h>

int main() {
  int vals[] = {0, 44, 11, 3, 7};
  int n = 5;
  int alvo = 3;

  int encontrado = 0;
  for (int i = 0; i < n; i++) {
    if (vals[i] == alvo) {
      printf("[vetor] Valor %d encontrado na posicao %d\n", vals[i], i);
      encontrado = 1;
      break;
    }
  }
  if (!encontrado)
    printf("[vetor] Valor %d nao encontrado\n", alvo);

  return 0;
}
