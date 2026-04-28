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
#include <stdlib.h>

// 1 = vetor, 0 = lista encadeada
#define USA_VETOR 0

typedef struct No {
  int valor;
  struct No *prox;
} No;

No *criar_lista(int *vals, int n) {
  No *head = NULL, *tail = NULL;
  for (int i = 0; i < n; i++) {
    No *no = malloc(sizeof(No));
    no->valor = vals[i];
    no->prox = NULL;
    if (!head)
      head = no;
    else
      tail->prox = no;
    tail = no;
  }
  return head;
}

void liberar_lista(No *head) {
  while (head) {
    No *tmp = head;
    head = head->prox;
    free(tmp);
  }
}

int main() {
  int vals[] = {0, 44, 11, 3, 7};
  int n = 5;
  int alvo = 3;

#if USA_VETOR
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

#else
  No *lista = criar_lista(vals, n);
  No *cur = lista;
  int pos = 0, encontrado = 0;
  while (cur) {
    if (cur->valor == alvo) {
      printf("[lista] Valor %d encontrado na posicao %d\n", cur->valor, pos);
      encontrado = 1;
      break;
    }
    cur = cur->prox;
    pos++;
  }
  if (!encontrado)
    printf("[lista] Valor %d nao encontrado\n", alvo);
  liberar_lista(lista);
#endif

  return 0;
}
