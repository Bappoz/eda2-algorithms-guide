# Algoritmos de Busca
--- 

## Objetivo
O objetivo deste módulo é apresentar os algoritmos de busca, que são fundamentais para localizar elementos em estruturas de dados. Serão abordados os algoritmos de busca sequencial, busca em árvore, hashing e busca binária, além de suas aplicações e complexidades. Cada pasta possuirá uma explicação detalhada do algoritmo, exemplos de implementação e exercícios para praticar.

--- 

## Busca 

Formalmente, é o algoritmo que aceita um argumento ´a´ e tenta encontrar um elemento ´x´, a partir da chave ´a´ em uma estrutura de dados. O algoritmo retorna o índice do elemento encontrado ou -1 se o elemento não estiver presente na estrutura de dados.

### Operações na Tabela de Busca
- *Inserção*: Adicionar um novo elemento à tabela de busca.
- *Algoritmo de Busca e inserção*: se não encontrar o elemento, insere-o na tabela de busca.
- *Remoção*: remover um elemento da tabela de busca.
- *Recuperação*: recuperar um elemento da tabela de busca.

### Tipos de Busca

1. A *Tabela* pode ser: 
  - Um vetor de registros 
  - Uma listaencadeada de registros
  - Uma árvore de registros
  - Etc...

2. A *Tabela* pode ficar: 
  - Totalmente na memória (*busca interna*)
  - Totalmente no armazenamento auxiliar (*busca externa*)
  - Ou em Ambos (*busca semi-externa*).

3. Técnicas de busca: 
  - *Busca Sequencial*: percorre a estrutura de dados elemento por elemento até encontrar o elemento desejado ou chegar ao final da estrutura.
  - *Busca Binária*: funciona em estruturas de dados ordenadas, dividindo repetidamente o espaço de busca pela metade até encontrar o elemento ou determinar que ele não está presente.
  - *Busca em Árvore*: utiliza uma estrutura de árvore para organizar os dados, permitindo buscas eficientes.
  - *Hashing*: utiliza uma função de hash para mapear chaves a índices em uma tabela, permitindo buscas rápidas.

4. O objetivo é encontrar um dado registro com o menor custo possível, ou seja, com o menor número de comparações. O custo de um algoritmo de busca é medido pelo número de comparações realizadas para encontrar o elemento desejado.

---

## Estrutura do Modulo

### Busca Sequencial (`sequencial/`)

| Arquivo | Tecnica | Complexidade |
|---|---|---|
| `basic.c` | Sequencial simples | O(n) |
| `sentinel.c` | Com sentinela | O(n), menos comparacoes por iteracao |
| `busca_transposicao.c` | Transposicao (move um passo para frente) | O(n) amortizado |
| `busca_mpf.c` | Move-to-Front (mover para frente) | O(n) amortizado |

**Sentinela**: coloca o valor buscado na posicao `A[n]`, eliminando o teste de fim de lista dentro do loop.

**Auto-organizacao (Transposicao e MPF)**: elementos buscados com frequencia migram para o inicio, reduzindo comparacoes futuras. MPF converge mais rapido que Transposicao mas pode causar mais movimentacoes.

### Busca Binaria (`binary/`)

Requer vetor **ordenado**. Divide o espaco de busca ao meio a cada passo.

| Arquivo | Tecnica | Complexidade |
|---|---|---|
| `iterativa.c` | Iterativa | O(log n) |
| `recursive.c` | Recursiva | O(log n), O(log n) pilha |
| `binary_indexed.c` | Com indice esparso | O(log m) + O(n/m) |

### Busca por Interpolacao (`interpolation/`)

| Arquivo | Tecnica | Complexidade |
|---|---|---|
| `interpolation.c` | Interpolacao linear | O(log log n) medio, O(n) pior |

Estima a posicao proporcional ao valor: `pos = begin + (item - v[begin]) * (end - begin) / (v[end] - v[begin])`. Supera busca binaria para dados uniformemente distribuidos.

### Busca em Arvores (`trees/`)

| Arquivo | Estrutura | Complexidade busca |
|---|---|---|
| `bst.c` | ABB - Arvore Binaria de Busca | O(log n) medio, O(n) pior |
| `avl_tree.c` | AVL (balanceada automaticamente) | O(log n) garantido |
| `red_black_tree.c` | Rubro-Negra (balanceada por cor) | O(log n) garantido |
| `B_tree.c` | Arvore B (grau minimo t) | O(t * log_t n) |
| `multidirection_tree.c` | Trie (arvore de prefixos) | O(m), m = tamanho da string |

**ABB**: sem balanceamento, degrada para lista em insercoes ordenadas.

**AVL**: auto-balanceamento por rotacoes, garante altura O(log n). Melhor para buscas (altura minima).

**Rubro-Negra**: balanceamento por coloracao (vermelho/preto) com 5 invariantes. Menos rotacoes que AVL em insercao/remocao — preferida em implementacoes reais (STL `map`, Java `TreeMap`, Linux CFS). Altura <= 2*log2(n+1).

Invariantes da ARN:
1. Todo no e vermelho ou preto
2. A raiz e preta
3. Folhas (NIL) sao pretas
4. Filho de no vermelho e sempre preto (sem vermelho-vermelho)
5. Todo caminho da raiz a uma folha tem o mesmo numero de nos pretos (altura preta)

Operacoes criticas:
- **Insercao**: nó novo entra vermelho; `inserirFixup` resolve ate 3 casos com recoloricao + rotacoes
- **Remocao**: se no removido era preto, `removerFixup` corrige "duplo-preto" com ate 4 casos
- **Sentinela NIL**: no preto compartilhado (evita NULL-checks em toda operacao)

**Arvore B**: minimiza acessos a disco (um no = uma pagina). Usada em bancos de dados e sistemas de arquivos.

**Trie**: busca em O(m) independente de n. Ideal para autocomplete e busca por prefixo.

### Hashing (`hashing/`)

Mapeia chaves a posicoes por uma funcao hash h(k). O(1) amortizado.

| Arquivo | Tecnica de Colisao | Complexidade media |
|---|---|---|
| `encadeamento.c` | Encadeamento exterior (listas) | O(1 + alfa) |
| `enderecamento_aberto.c` | Linear / Quadratica / Hash Duplo | O(1/(1-alfa)) |

**Fator de carga** alfa = n/M. Manter alfa < 0.75 (encadeamento) ou < 0.70 (end. aberto).

Tres sondagens no endereçamento aberto:
- **Linear**: `h(k,i) = (h'(k) + i) mod M` -- agrupamento primario
- **Quadratica**: `h(k,i) = (h'(k) + c1*i + c2*i^2) mod M` -- agrupamento secundario
- **Hash Duplo**: `h(k,i) = (h1(k) + i*h2(k)) mod M` -- melhor distribuicao

---

## Como compilar

```bash
gcc -Wall -O2 -o saida busca/sequencial/basic.c && ./saida
gcc -Wall -O2 -o saida busca/trees/avl_tree.c && ./saida
gcc -Wall -O2 -o saida busca/trees/red_black_tree.c && ./saida
gcc -Wall -O2 -o saida busca/hashing/encadeamento.c && ./saida
```

---

## Comparativo Geral

| Tecnica | Estrutura necessaria | Complexidade busca |
|---|---|---|
| Sequencial | Qualquer | O(n) |
| Binaria | Vetor ordenado | O(log n) |
| Interpolacao | Vetor ordenado uniforme | O(log log n) |
| ABB | - | O(log n) / O(n) |
| AVL | - | O(log n) |
| Rubro-Negra | - | O(log n) |
| Arvore B | - | O(log_t n) |
| Trie | - | O(m) |
| Hashing | - | O(1) amortizado |
