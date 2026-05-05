#include <stdio.h>
#include <stdlib.h>

// Estrutura do Nó
typedef struct No {
  int chave;
  struct No *esq, *dir;
  int altura;
} No;

// Função auxiliar para obter a altura
int altura(No *n) { return (n == NULL) ? 0 : n->altura; }

// Função auxiliar para obter o máximo entre dois inteiros
int max(int a, int b) { return (a > b) ? a : b; }

// Cria um novo nó
No *novoNo(int chave) {
  No *no = (No *)malloc(sizeof(No));
  no->chave = chave;
  no->esq = no->dir = NULL;
  no->altura = 1;
  return no;
}

// --- ROTAÇÕES PARA MANTER O BALANCEAMENTO ---

No *rotarDireita(No *y) {
  No *x = y->esq;
  No *T2 = x->dir;
  x->dir = y;
  y->esq = T2;
  y->altura = max(altura(y->esq), altura(y->dir)) + 1;
  x->altura = max(altura(x->esq), altura(x->dir)) + 1;
  return x;
}

No *rotarEsquerda(No *x) {
  No *y = x->dir;
  No *T2 = y->esq;
  y->esq = x;
  x->dir = T2;
  x->altura = max(altura(x->esq), altura(x->dir)) + 1;
  y->altura = max(altura(y->esq), altura(y->dir)) + 1;
  return y;
}

int getFatorEquilibrio(No *n) {
  return (n == NULL) ? 0 : altura(n->esq) - altura(n->dir);
}

// --- INSERÇÃO ---

No *inserir(No *no, int chave) {
  if (no == NULL)
    return novoNo(chave);

  if (chave < no->chave)
    no->esq = inserir(no->esq, chave);
  else if (chave > no->chave)
    no->dir = inserir(no->dir, chave);
  else
    return no;

  no->altura = 1 + max(altura(no->esq), altura(no->dir));
  int fb = getFatorEquilibrio(no);

  if (fb > 1 && chave < no->esq->chave)
    return rotarDireita(no);
  if (fb < -1 && chave > no->dir->chave)
    return rotarEsquerda(no);
  if (fb > 1 && chave > no->esq->chave) {
    no->esq = rotarEsquerda(no->esq);
    return rotarDireita(no);
  }
  if (fb < -1 && chave < no->dir->chave) {
    no->dir = rotarDireita(no->dir);
    return rotarEsquerda(no);
  }
  return no;
}

// --- FUNÇÃO DE BUSCA (O que você quer aprender) ---

No *buscar(No *raiz, int chave) {
  // Caso base: raiz nula (não achou) ou chave encontrada
  if (raiz == NULL || raiz->chave == chave)
    return raiz;

  // Se a chave for maior que a da raiz, procura na direita
  if (chave > raiz->chave)
    return buscar(raiz->dir, chave);

  // Se a chave for menor que a da raiz, procura na esquerda
  return buscar(raiz->esq, chave);
}

// Impressão em ordem para conferir a árvore
void emOrdem(No *raiz) {
  if (raiz != NULL) {
    emOrdem(raiz->esq);
    printf("[%d, h:%d] ", raiz->chave, raiz->altura);
    emOrdem(raiz->dir);
  }
}

// --- MAIN ---

int main() {
  No *raiz = NULL;
  int valores[] = {10, 20, 30, 40, 50, 25};
  int n = sizeof(valores) / sizeof(valores[0]);

  for (int i = 0; i < n; i++) {
    raiz = inserir(raiz, valores[i]);
  }

  printf("Arvore AVL (Em-Ordem): ");
  emOrdem(raiz);
  printf("\n\n");

  // Testando a Busca
  int alvo = 25;
  printf("Buscando valor %d...\n", alvo);
  No *resultado = buscar(raiz, alvo);

  if (resultado != NULL) {
    printf("Elemento %d encontrado! Altura do no: %d\n", resultado->chave,
           resultado->altura);
  } else {
    printf("Elemento %d nao encontrado.\n", alvo);
  }

  return 0;
}
