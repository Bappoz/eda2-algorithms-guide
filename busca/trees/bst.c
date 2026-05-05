/*
 * Arvore Binaria de Busca (ABB / BST - Binary Search Tree)
 *
 * Propriedade fundamental: para todo no X,
 *   - todos os nos da subarvore ESQUERDA tem chave < X.chave
 *   - todos os nos da subarvore DIREITA  tem chave > X.chave
 *
 * Complexidade (arvore balanceada / degenerada):
 *   Busca:   O(log n) / O(n)
 *   Insercao: O(log n) / O(n)
 *   Remocao: O(log n) / O(n)
 *
 * Diferenca para AVL: a ABB nao se auto-balanceia, portanto insercoes em
 * ordem crescente/decrescente produzem uma lista ligada (pior caso O(n)).
 * A AVL resolve isso com rotacoes. A ABB e a base conceitual da AVL e da
 * Red-Black Tree.
 *
 * Operacoes implementadas:
 *   - inserir      : insere chave mantendo a propriedade BST
 *   - buscar       : retorna ponteiro para o no ou NULL
 *   - minimo       : no mais a esquerda (menor chave)
 *   - remover      : tres casos (folha, 1 filho, 2 filhos - substitui pelo
 *                    sucessor em-ordem)
 *   - emOrdem      : percurso L-Raiz-R imprime chaves em ordem crescente
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int chave;
    struct No *esq, *dir;
} No;

No *novoNo(int chave) {
    No *n = malloc(sizeof(No));
    n->chave = chave;
    n->esq = n->dir = NULL;
    return n;
}

/* Insercao recursiva */
No *inserir(No *raiz, int chave) {
    if (raiz == NULL) return novoNo(chave);
    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserir(raiz->dir, chave);
    /* duplicatas sao ignoradas */
    return raiz;
}

/* Busca recursiva - retorna o no ou NULL */
No *buscar(No *raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave) return raiz;
    return chave < raiz->chave
        ? buscar(raiz->esq, chave)
        : buscar(raiz->dir, chave);
}

/* No com menor chave (mais a esquerda) */
No *minimo(No *raiz) {
    while (raiz->esq != NULL) raiz = raiz->esq;
    return raiz;
}

/*
 * Remocao - tres casos:
 *   1. No e folha         -> deleta diretamente
 *   2. No tem um filho    -> substitui pelo filho
 *   3. No tem dois filhos -> substitui pela chave do sucessor em-ordem
 *      (menor da subarvore direita) e remove o sucessor recursivamente
 */
No *remover(No *raiz, int chave) {
    if (raiz == NULL) return NULL;

    if (chave < raiz->chave) {
        raiz->esq = remover(raiz->esq, chave);
    } else if (chave > raiz->chave) {
        raiz->dir = remover(raiz->dir, chave);
    } else {
        /* Caso 1 e 2 */
        if (raiz->esq == NULL) {
            No *tmp = raiz->dir;
            free(raiz);
            return tmp;
        }
        if (raiz->dir == NULL) {
            No *tmp = raiz->esq;
            free(raiz);
            return tmp;
        }
        /* Caso 3: substitui pelo sucessor em-ordem */
        No *suc = minimo(raiz->dir);
        raiz->chave = suc->chave;
        raiz->dir = remover(raiz->dir, suc->chave);
    }
    return raiz;
}

void emOrdem(No *raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("%d ", raiz->chave);
    emOrdem(raiz->dir);
}

int main() {
    No *raiz = NULL;
    int valores[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < n; i++)
        raiz = inserir(raiz, valores[i]);

    printf("Em-ordem: ");
    emOrdem(raiz);
    printf("\n");

    int alvo = 40;
    No *res = buscar(raiz, alvo);
    printf("Busca %d: %s\n", alvo, res ? "encontrado" : "nao encontrado");

    raiz = remover(raiz, 30);
    printf("Apos remover 30 - Em-ordem: ");
    emOrdem(raiz);
    printf("\n");

    return 0;
}
