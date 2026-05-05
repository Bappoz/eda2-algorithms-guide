/*
 * Arvore B (B-Tree) de grau minimo t
 *
 * Propriedades:
 *   - Cada no tem no minimo t-1 chaves (exceto a raiz) e no maximo 2t-1 chaves.
 *   - Cada no interno tem no minimo t filhos e no maximo 2t filhos.
 *   - Todas as folhas estao no mesmo nivel (arvore perfeitamente balanceada em
 *     altura).
 *   - As chaves dentro de um no sao ordenadas.
 *   - Projetada para minimizar acessos a disco: um no = uma pagina de disco.
 *
 * Complexidade (n = total de chaves, t = grau minimo):
 *   Altura maxima: O(log_t n)
 *   Busca:         O(t * log_t n)
 *   Insercao:      O(t * log_t n)  -- pode causar splits
 *
 * Aplicacoes tipicas: sistemas de arquivos (ext4, NTFS), bancos de dados
 * (PostgreSQL, MySQL usam B+ Tree, variante que guarda dados so nas folhas).
 *
 * Implementacao: insercao com split proativo (insere descendo, split antes
 * de no ficar cheio) para evitar uma segunda passagem de baixo para cima.
 */

#include <stdio.h>
#include <stdlib.h>

#define T 3  /* grau minimo: cada no tem entre T-1 e 2T-1 chaves */

typedef struct No {
    int chaves[2 * T - 1];
    struct No *filhos[2 * T];
    int n;          /* numero atual de chaves */
    int folha;      /* 1 se for folha */
} No;

No *novoNo(int folha) {
    No *no = calloc(1, sizeof(No));
    no->folha = folha;
    return no;
}

/* Busca a chave x na subarvore com raiz no. Retorna o no e o indice, ou
 * (NULL, -1) se nao encontrada. */
No *buscar(No *no, int x, int *idx) {
    int i = 0;
    while (i < no->n && x > no->chaves[i]) i++;

    if (i < no->n && no->chaves[i] == x) {
        *idx = i;
        return no;
    }
    if (no->folha) return NULL;
    return buscar(no->filhos[i], x, idx);
}

/* Divide o filho cheio no->filhos[i] ao meio.
 * Precondition: no nao esta cheio, filhos[i] tem 2T-1 chaves. */
void splitFilho(No *pai, int i) {
    No *cheio = pai->filhos[i];
    No *novo  = novoNo(cheio->folha);
    novo->n = T - 1;

    /* Copia a metade direita do no cheio para o novo no */
    for (int j = 0; j < T - 1; j++)
        novo->chaves[j] = cheio->chaves[j + T];
    if (!cheio->folha)
        for (int j = 0; j < T; j++)
            novo->filhos[j] = cheio->filhos[j + T];

    cheio->n = T - 1;

    /* Abre espaco no pai para o novo filho */
    for (int j = pai->n; j > i; j--)
        pai->filhos[j + 1] = pai->filhos[j];
    pai->filhos[i + 1] = novo;

    for (int j = pai->n - 1; j >= i; j--)
        pai->chaves[j + 1] = pai->chaves[j];
    pai->chaves[i] = cheio->chaves[T - 1];  /* chave mediana sobe */
    pai->n++;
}

/* Insere k em uma subarvore que NAO esta cheia */
void inserirNaoCheio(No *no, int k) {
    int i = no->n - 1;

    if (no->folha) {
        /* Desloca chaves maiores para abrir espaco */
        while (i >= 0 && no->chaves[i] > k) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = k;
        no->n++;
    } else {
        /* Encontra o filho correto */
        while (i >= 0 && no->chaves[i] > k) i--;
        i++;
        if (no->filhos[i]->n == 2 * T - 1) {
            splitFilho(no, i);
            if (no->chaves[i] < k) i++;
        }
        inserirNaoCheio(no->filhos[i], k);
    }
}

No *inserir(No *raiz, int k) {
    if (raiz->n == 2 * T - 1) {
        /* Raiz cheia: cria nova raiz e split */
        No *nova = novoNo(0);
        nova->filhos[0] = raiz;
        splitFilho(nova, 0);
        inserirNaoCheio(nova, k);
        return nova;
    }
    inserirNaoCheio(raiz, k);
    return raiz;
}

void percurso(No *no, int nivel) {
    printf("Nivel %d: ", nivel);
    for (int i = 0; i < no->n; i++)
        printf("%d ", no->chaves[i]);
    printf("\n");
    if (!no->folha)
        for (int i = 0; i <= no->n; i++)
            percurso(no->filhos[i], nivel + 1);
}

int main() {
    No *raiz = novoNo(1);

    int vals[] = {10, 20, 5, 6, 12, 30, 7, 17, 3, 25, 18, 1};
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++)
        raiz = inserir(raiz, vals[i]);

    printf("Percurso da Arvore B (grau minimo t=%d):\n", T);
    percurso(raiz, 0);

    int idx;
    int alvo = 18;
    No *res = buscar(raiz, alvo, &idx);
    printf("\nBusca %d: %s", alvo,
           res ? "encontrado" : "nao encontrado");
    if (res) printf(" (indice %d no no)", idx);
    printf("\n");

    return 0;
}
