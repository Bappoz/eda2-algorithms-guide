/*
 * Trie (Arvore de Prefixos / Arvore Digital / Arvore Multidirecional)
 *
 * Uma Trie e uma arvore onde cada caminho da raiz ate uma folha representa
 * uma string. Cada no tem ate ALPHA filhos (um por caractere do alfabeto).
 * Nao armazena a chave no no: a chave e determinada pelo caminho percorrido.
 *
 *   Exemplo com {"cao", "casa", "carro", "gato"}:
 *
 *       (raiz)
 *      /       \
 *     c          g
 *     |          |
 *     a          a
 *    / \         |
 *   o  s  r      t
 *      |  |      |
 *      a  r      o*
 *         |
 *         o*
 *
 *   (*) marca fim de palavra
 *
 * Complexidade:
 *   Busca/Insercao: O(m), onde m = comprimento da string
 *   Independente do numero de palavras na arvore (n nao importa)!
 *
 * Vantagens:
 *   - Busca por prefixo em O(m): "todas as palavras que comecam com 'ca'"
 *   - Mais rapido que hash para strings com prefixo compartilhado
 *   - Ordena lexicograficamente de forma natural (percurso em-ordem)
 *
 * Desvantagens:
 *   - Consumo de memoria: O(ALPHA * n * m) no pior caso
 *   - Eficiente so para alfabetos pequenos ou strings com muitos prefixos
 *     compartilhados
 *
 * Aplicacoes: autocomplete, corretores ortograficos, roteadores IP,
 *             dicionarios, busca por prefixo em bancos de dados.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 26  /* apenas letras minusculas a-z */

typedef struct NoTrie {
    struct NoTrie *filhos[ALPHA];
    int fim_palavra;  /* 1 se este no termina uma palavra */
} NoTrie;

NoTrie *novoNo() {
    NoTrie *no = calloc(1, sizeof(NoTrie));
    return no;
}

/* Insere a string s na trie */
void inserir(NoTrie *raiz, const char *s) {
    NoTrie *cur = raiz;
    for (int i = 0; s[i] != '\0'; i++) {
        int idx = s[i] - 'a';
        if (cur->filhos[idx] == NULL)
            cur->filhos[idx] = novoNo();
        cur = cur->filhos[idx];
    }
    cur->fim_palavra = 1;
}

/* Retorna 1 se a string s existe na trie, 0 caso contrario */
int buscar(NoTrie *raiz, const char *s) {
    NoTrie *cur = raiz;
    for (int i = 0; s[i] != '\0'; i++) {
        int idx = s[i] - 'a';
        if (cur->filhos[idx] == NULL) return 0;
        cur = cur->filhos[idx];
    }
    return cur->fim_palavra;
}

/* Retorna 1 se alguma palavra na trie comeca com o prefixo dado */
int buscar_prefixo(NoTrie *raiz, const char *prefixo) {
    NoTrie *cur = raiz;
    for (int i = 0; prefixo[i] != '\0'; i++) {
        int idx = prefixo[i] - 'a';
        if (cur->filhos[idx] == NULL) return 0;
        cur = cur->filhos[idx];
    }
    return 1;  /* prefixo existe; pode ou nao ser uma palavra completa */
}

/* Percurso em-ordem: imprime todas as palavras em ordem lexicografica */
void percurso(NoTrie *no, char *buf, int nivel) {
    if (no->fim_palavra) {
        buf[nivel] = '\0';
        printf("  %s\n", buf);
    }
    for (int i = 0; i < ALPHA; i++) {
        if (no->filhos[i]) {
            buf[nivel] = 'a' + i;
            percurso(no->filhos[i], buf, nivel + 1);
        }
    }
}

int main() {
    NoTrie *raiz = novoNo();

    const char *palavras[] = {"cao", "casa", "carro", "gato", "gata",
                              "cana", "capim", "garra"};
    int n = sizeof(palavras) / sizeof(palavras[0]);

    for (int i = 0; i < n; i++)
        inserir(raiz, palavras[i]);

    printf("Palavras na Trie (ordem lexicografica):\n");
    char buf[100];
    percurso(raiz, buf, 0);

    printf("\nBuscas exatas:\n");
    const char *testes[] = {"casa", "carro", "caro", "gat"};
    for (int i = 0; i < 4; i++)
        printf("  \"%s\": %s\n", testes[i],
               buscar(raiz, testes[i]) ? "encontrada" : "nao encontrada");

    printf("\nBusca por prefixo:\n");
    const char *prefixos[] = {"ca", "ga", "ze"};
    for (int i = 0; i < 3; i++)
        printf("  prefixo \"%s\": %s\n", prefixos[i],
               buscar_prefixo(raiz, prefixos[i]) ? "existe" : "nao existe");

    return 0;
}
