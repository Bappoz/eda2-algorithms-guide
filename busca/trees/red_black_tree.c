#include <stdio.h>
#include <stdlib.h>

// --- DEFINIÇÕES ---

#define VERMELHO 0
#define PRETO 1

typedef struct No {
  int chave;
  int cor;
  struct No *esq, *dir, *pai;
} No;

// Nó sentinela NIL: folha preta compartilhada (evita NULL checks)
static No NIL_NO = {0, PRETO, NULL, NULL, NULL};
static No *NIL = &NIL_NO;

// --- UTILITÁRIOS ---

No *novoNo(int chave) {
  No *no = (No *)malloc(sizeof(No));
  no->chave = chave;
  no->cor = VERMELHO; // todo nó novo começa vermelho
  no->esq = no->dir = no->pai = NIL;
  return no;
}

// --- ROTAÇÕES ---

//      y               x
//     / \     →       / \
//    x   C           A   y
//   / \                 / \
//  A   B               B   C
void rotarDireita(No **raiz, No *y) {
  No *x = y->esq;
  y->esq = x->dir;
  if (x->dir != NIL)
    x->dir->pai = y;
  x->pai = y->pai;
  if (y->pai == NIL)
    *raiz = x;
  else if (y == y->pai->dir)
    y->pai->dir = x;
  else
    y->pai->esq = x;
  x->dir = y;
  y->pai = x;
}

//    x                 y
//   / \      →        / \
//  A   y             x   C
//     / \           / \
//    B   C         A   B
void rotarEsquerda(No **raiz, No *x) {
  No *y = x->dir;
  x->dir = y->esq;
  if (y->esq != NIL)
    y->esq->pai = x;
  y->pai = x->pai;
  if (x->pai == NIL)
    *raiz = y;
  else if (x == x->pai->esq)
    x->pai->esq = y;
  else
    x->pai->dir = y;
  y->esq = x;
  x->pai = y;
}

// --- INSERÇÃO ---

// Corrige as 3 violações possíveis após inserir nó vermelho:
//   Caso 1: tio é vermelho → recolorir pai, tio e avô
//   Caso 2: tio é preto, nó é filho interno → rotação para virar Caso 3
//   Caso 3: tio é preto, nó é filho externo → rotação + recolorir
void inserirFixup(No **raiz, No *z) {
  while (z->pai->cor == VERMELHO) {
    if (z->pai == z->pai->pai->esq) {
      No *tio = z->pai->pai->dir;
      if (tio->cor == VERMELHO) {
        // Caso 1: tio vermelho
        z->pai->cor = PRETO;
        tio->cor = PRETO;
        z->pai->pai->cor = VERMELHO;
        z = z->pai->pai;
      } else {
        if (z == z->pai->dir) {
          // Caso 2: nó é filho direito → rotação esquerda para cair no Caso 3
          z = z->pai;
          rotarEsquerda(raiz, z);
        }
        // Caso 3: nó é filho esquerdo
        z->pai->cor = PRETO;
        z->pai->pai->cor = VERMELHO;
        rotarDireita(raiz, z->pai->pai);
      }
    } else {
      // Espelho: pai é filho direito do avô
      No *tio = z->pai->pai->esq;
      if (tio->cor == VERMELHO) {
        z->pai->cor = PRETO;
        tio->cor = PRETO;
        z->pai->pai->cor = VERMELHO;
        z = z->pai->pai;
      } else {
        if (z == z->pai->esq) {
          z = z->pai;
          rotarDireita(raiz, z);
        }
        z->pai->cor = PRETO;
        z->pai->pai->cor = VERMELHO;
        rotarEsquerda(raiz, z->pai->pai);
      }
    }
  }
  (*raiz)->cor = PRETO; // invariante: raiz sempre preta
}

void inserir(No **raiz, int chave) {
  No *z = novoNo(chave);
  No *y = NIL, *x = *raiz;

  // Desce até a posição de inserção (igual ao BST)
  while (x != NIL) {
    y = x;
    x = (z->chave < x->chave) ? x->esq : x->dir;
  }
  z->pai = y;

  if (y == NIL)
    *raiz = z;
  else if (z->chave < y->chave)
    y->esq = z;
  else
    y->dir = z;

  inserirFixup(raiz, z);
}

// --- REMOÇÃO ---

// Substitui a subárvore enraizada em u pela enraizada em v
void transplantar(No **raiz, No *u, No *v) {
  if (u->pai == NIL)
    *raiz = v;
  else if (u == u->pai->esq)
    u->pai->esq = v;
  else
    u->pai->dir = v;
  v->pai = u->pai; // sempre válido pois v pode ser NIL e NIL->pai é atualizado
}

No *minimo(No *x) {
  while (x->esq != NIL)
    x = x->esq;
  return x;
}

// Corrige "duplo-preto" após remoção de nó preto:
//   Caso 1: irmão vermelho → recolorir + rotação, reduz ao Caso 2/3/4
//   Caso 2: irmão preto, filhos pretos → recolorir irmão, sobe o problema
//   Caso 3: irmão preto, filho externo preto → recolorir + rotação, vira Caso 4
//   Caso 4: irmão preto, filho externo vermelho → rotação + recolorir, encerra
void removerFixup(No **raiz, No *x) {
  while (x != *raiz && x->cor == PRETO) {
    if (x == x->pai->esq) {
      No *w = x->pai->dir; // irmão
      if (w->cor == VERMELHO) {
        // Caso 1
        w->cor = PRETO;
        x->pai->cor = VERMELHO;
        rotarEsquerda(raiz, x->pai);
        w = x->pai->dir;
      }
      if (w->esq->cor == PRETO && w->dir->cor == PRETO) {
        // Caso 2
        w->cor = VERMELHO;
        x = x->pai;
      } else {
        if (w->dir->cor == PRETO) {
          // Caso 3
          w->esq->cor = PRETO;
          w->cor = VERMELHO;
          rotarDireita(raiz, w);
          w = x->pai->dir;
        }
        // Caso 4
        w->cor = x->pai->cor;
        x->pai->cor = PRETO;
        w->dir->cor = PRETO;
        rotarEsquerda(raiz, x->pai);
        x = *raiz; // encerra o loop
      }
    } else {
      // Espelho
      No *w = x->pai->esq;
      if (w->cor == VERMELHO) {
        w->cor = PRETO;
        x->pai->cor = VERMELHO;
        rotarDireita(raiz, x->pai);
        w = x->pai->esq;
      }
      if (w->dir->cor == PRETO && w->esq->cor == PRETO) {
        w->cor = VERMELHO;
        x = x->pai;
      } else {
        if (w->esq->cor == PRETO) {
          w->dir->cor = PRETO;
          w->cor = VERMELHO;
          rotarEsquerda(raiz, w);
          w = x->pai->esq;
        }
        w->cor = x->pai->cor;
        x->pai->cor = PRETO;
        w->esq->cor = PRETO;
        rotarDireita(raiz, x->pai);
        x = *raiz;
      }
    }
  }
  x->cor = PRETO;
}

void remover(No **raiz, int chave) {
  // Busca o nó a remover
  No *z = *raiz;
  while (z != NIL && z->chave != chave)
    z = (chave < z->chave) ? z->esq : z->dir;
  if (z == NIL) return; // não encontrado

  No *y = z;
  int corOriginalY = y->cor;
  No *x;

  if (z->esq == NIL) {
    x = z->dir;
    transplantar(raiz, z, z->dir);
  } else if (z->dir == NIL) {
    x = z->esq;
    transplantar(raiz, z, z->esq);
  } else {
    // y = sucessor de z (mínimo da subárvore direita)
    y = minimo(z->dir);
    corOriginalY = y->cor;
    x = y->dir;

    if (y->pai == z) {
      x->pai = y; // necessário mesmo se x == NIL
    } else {
      transplantar(raiz, y, y->dir);
      y->dir = z->dir;
      y->dir->pai = y;
    }
    transplantar(raiz, z, y);
    y->esq = z->esq;
    y->esq->pai = y;
    y->cor = z->cor;
  }

  free(z);

  // Só precisamos corrigir se removemos um nó preto (perda de altura preta)
  if (corOriginalY == PRETO)
    removerFixup(raiz, x);
}

// --- BUSCA ---

No *buscar(No *raiz, int chave) {
  while (raiz != NIL) {
    if (chave == raiz->chave) return raiz;
    raiz = (chave < raiz->chave) ? raiz->esq : raiz->dir;
  }
  return NULL;
}

// --- VERIFICAÇÃO DAS INVARIANTES ---

// Retorna a altura preta do nó (-1 se violar invariante)
int alturaPretal(No *no) {
  if (no == NIL) return 1; // NIL conta como 1 nó preto

  if (no->cor == VERMELHO &&
      (no->esq->cor == VERMELHO || no->dir->cor == VERMELHO))
    return -1; // violação: vermelho com filho vermelho

  int esq = alturaPretal(no->esq);
  int dir = alturaPretal(no->dir);

  if (esq == -1 || dir == -1 || esq != dir)
    return -1; // violação de altura preta

  return esq + (no->cor == PRETO ? 1 : 0);
}

int verificar(No *raiz) {
  if (raiz->cor != PRETO) return 0; // raiz deve ser preta
  return alturaPretal(raiz) != -1;
}

// --- IMPRESSÃO ---

void emOrdem(No *raiz) {
  if (raiz != NIL) {
    emOrdem(raiz->esq);
    printf("[%d %s] ", raiz->chave, raiz->cor == VERMELHO ? "V" : "P");
    emOrdem(raiz->dir);
  }
}

// Impressão visual da árvore (rotacionada 90°)
void imprimirArvore(No *no, int nivel) {
  if (no == NIL) return;
  imprimirArvore(no->dir, nivel + 1);
  for (int i = 0; i < nivel; i++) printf("    ");
  printf("%d(%s)\n", no->chave, no->cor == VERMELHO ? "V" : "P");
  imprimirArvore(no->esq, nivel + 1);
}

// --- MAIN ---

int main() {
  No *raiz = NIL;

  int valores[] = {41, 38, 31, 12, 19, 8};
  int n = sizeof(valores) / sizeof(valores[0]);

  printf("=== Insercoes ===\n");
  for (int i = 0; i < n; i++) {
    inserir(&raiz, valores[i]);
    printf("Inseriu %d | Invariantes: %s\n", valores[i],
           verificar(raiz) ? "OK" : "VIOLACAO");
  }

  printf("\nArvore em-ordem (chave cor):\n");
  emOrdem(raiz);
  printf("\n");

  printf("\nArvore visual (90 graus, direita=cima):\n");
  imprimirArvore(raiz, 0);

  printf("\n=== Buscas ===\n");
  int buscas[] = {19, 99};
  for (int i = 0; i < 2; i++) {
    No *r = buscar(raiz, buscas[i]);
    printf("Busca %d: %s\n", buscas[i], r ? "encontrado" : "nao encontrado");
  }

  printf("\n=== Remocoes ===\n");
  int remocoes[] = {8, 38, 41};
  for (int i = 0; i < 3; i++) {
    remover(&raiz, remocoes[i]);
    printf("Removeu %d | Invariantes: %s\n", remocoes[i],
           (raiz == NIL || verificar(raiz)) ? "OK" : "VIOLACAO");
  }

  printf("\nArvore apos remocoes:\n");
  emOrdem(raiz);
  printf("\n");

  return 0;
}
