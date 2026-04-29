/**
 * Busca recursivamente o item dentro do vetor vector ordenado. A cada iteração
 * reduz o espaço de busca pela metade. Quando encontra o item retorna
 * o índice onde ele foi encontrado. Caso contrário retorna -1
 */

int recursive_binary_search(float item, int begin, int end, float vector[]) {
  if (begin > end) {
    return -1; // Item não encontrado
  }

  int mid = begin + (end - begin) / 2; // Calcula o índice do meio

  if (vector[mid] == item) {
    return mid; // Item encontrado no índice mid
  } else if (vector[mid] > item) {
    return recursive_binary_search(item, begin, mid - 1,
                                   vector); // Busca na metade inferior
  } else {
    return recursive_binary_search(item, mid + 1, end,
                                   vector); // Busca na metade superior
  }
}
