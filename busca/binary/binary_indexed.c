

// Busca binária no índice + sequencial no bloco
// Reduz acessos: busca o bloco em O(log m), depois O(n/m) no bloco
// Ótimo quando a tabela principal é grande demais para busca binária direta no
// disco.
//
// Vantagens:
//  - O(log n) garantido
//  - Implementação simples
//  - Sem memória externa
//
// Desvantagens:
//  - Exige vetor ordenado
//  - Inserção e remoção são O(n)
//  - Não funciona em listas

typedef struct Indice {
  int chave;
  int pos;
} Indice;

int busca_bin_indexada(int A[], Indice idx[], int m, int x) {
  // 1. Busca binária no índice (m entradas)
  int inf = 0, sup = m - 1, bloco = 0;
  while (inf <= sup) {
    int meio = (inf + sup) / 2;
    if (idx[meio].chave == x)
      return idx[meio].pos;
    if (idx[meio].chave < x) {
      bloco = meio;
      inf = meio + 1;
    } else
      sup = meio - 1;
  }

  // 2. Busca sequencial no bloco identificado
  int inicio = idx[bloco].pos;
  int fim = (bloco < m - 1) ? idx[bloco + 1].pos - 1 : /* tamanho total */ -1;
  for (int i = inicio; i <= fim; i++) {
    if (A[i] == x)
      return i;
    if (A[i] > x)
      return -1;
  }
  return -1;
}
