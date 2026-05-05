# Algoritmos de Ordenacao

---

## Objetivo

Organizar um conjunto de registros de acordo com a ordem definida por uma chave de ordenacao. O custo e medido pelo numero de **comparacoes** e **movimentacoes** (trocas/deslocamentos).

---

## Classificacao dos Algoritmos

### Por paradigma

| Categoria | Algoritmos | Complexidade tipica |
|---|---|---|
| Simples (O(n^2)) | Bubble, Selection, Insertion, Shell | O(n^2) a O(n^(4/3)) |
| Eficientes (O(n log n)) | Merge Sort, Quick Sort, Heap Sort | O(n log n) |
| Lineares (nao-comparacao) | Counting, Radix (LSD/MSD), Bucket | O(n) a O(d*n) |

### Por propriedades

| Algoritmo | Estavel | In-place | Pior caso | Melhor caso |
|---|---|---|---|---|
| Bubble Sort | Sim | Sim | O(n^2) | O(n) |
| Selection Sort | Nao | Sim | O(n^2) | O(n^2) |
| Insertion Sort | Sim | Sim | O(n^2) | O(n) |
| Shell Sort | Nao | Sim | O(n^2)* | O(n log n) |
| Merge Sort | Sim | Nao (O(n)) | O(n log n) | O(n log n) |
| Quick Sort | Nao | Sim (O(log n) pilha) | O(n^2) | O(n log n) |
| Heap Sort | Nao | Sim | O(n log n) | O(n log n) |
| Counting Sort | Sim | Nao | O(n+K) | O(n+K) |
| Radix Sort LSD | Sim | Nao | O(d*(n+b)) | O(d*(n+b)) |
| Radix Sort MSD | Sim | Sim (American Flag) | O(d*(n+b)) | O(d*n) |
| Bucket Sort | Sim | Nao | O(n^2) | O(n) |

*Shell Sort: depende da sequencia de gaps.

---

## Algoritmos Simples (`simples/`)

### Bubble Sort (`bubble_sort.c`)

Compara pares adjacentes e os troca se fora de ordem. O maior "flutua" ate o fim a cada passagem.
- **Variante classica**: sempre faz n*(n-1)/2 comparacoes.
- **Variante otimizada**: flag `trocou` -- para cedo se o vetor ja estiver ordenado.

```
[5, 3, 1, 4, 2]
passagem 1: [3,1,4,2,5]  <- 5 esta no lugar
passagem 2: [1,3,2,4,5]  <- 4 esta no lugar
...
```

### Selection Sort (`selection_sort.c`)

Seleciona o minimo da parte nao-ordenada e troca com a primeira posicao livre. Sempre O(n^2) comparacoes mas apenas O(n) trocas.
- **Double Selection Sort**: seleciona min e max simultaneamente, metade das passagens.

### Insertion Sort (`insertion_sort.c`)

Insere cada elemento na posicao correta dentro do subvetor ja ordenado. O melhor algoritmo para dados **quase ordenados** ou **pequenos**.
- **Binary Insertion Sort**: usa busca binaria para encontrar posicao de insercao (O(log n) comparacoes por elemento, mas deslocamentos continuam O(n)).
- Usado internamente pelo **Timsort** (Python) e **Introsort** (C++) para subarrays pequenos.

### Shell Sort (`shell_sort.c`)

Insertion Sort com gaps decrescentes. Elimina inversoes de longa distancia antes do passo final.
- **Shell (gap=n/2)**: pior caso O(n^2)
- **Hibbard (2^k - 1)**: pior caso O(n^(3/2))
- **Ciura (empirica)**: melhor desempenho pratico para n tipicos

---

## Algoritmos Eficientes (`eficientes/`)

### Merge Sort (`merge_sort.c`)

Divide o vetor recursivamente ao meio e intercala os subarrays ordenados. **Unico garantido O(n log n) em todos os casos e estavel**.
- **Top-down (recursivo)**: implementacao classica.
- **Bottom-up (iterativo)**: sem recursao, mais cache-friendly.
- Preferido para **listas ligadas** e **ordenacao externa** (dados em disco).

### Quick Sort (`quick_sort.c`)

Divide pelo pivo (particao) e ordena os dois lados recursivamente. Na pratica o mais rapido para dados em memoria.
- **Particao de Lomuto**: pivo = ultimo, mais simples.
- **Particao de Hoare**: pivo = primeiro, ~3x menos trocas.
- **Mediana de tres + Insertion Sort**: evita pior caso, usado em `qsort()` de producao.

### Heap Sort (`heap_sort.c`)

Constroi um Max-Heap e extrai o maximo n vezes. **O(n log n) garantido e O(1) espaco auxiliar**.
- Fase 1 (Build Heap): O(n) -- sift-down de baixo para cima.
- Fase 2 (Extracao): O(n log n).
- Base do **Introsort**: Quick Sort que degrada para Heap Sort ao detectar pior caso.

---

## Algoritmos Lineares (`lineares/`)

Nao sao baseados em comparacoes. Quebram a barreira de O(n log n).
**Restricao**: funcionam apenas para tipos especificos de dados (inteiros, strings de comprimento fixo, valores em intervalo conhecido).

### Counting Sort (`counting_sort.c`)

Conta ocorrencias de cada valor e usa prefixo acumulado para posicionar.
- O(n+K) onde K = valor maximo.
- **Estavel** (essencial como sub-rotina do Radix Sort).
- Suporte a negativos: desloca pelo minimo.

### Radix Sort LSD (`radix_sort_lsd.c`)

Ordena do digito menos significativo ao mais significativo, usando Counting Sort estavel por digito.
- Simples e iterativo (sem recursao).
- **Base 256** (byte a byte): apenas 4 passagens para inteiros de 32 bits.
- Melhor localidade de cache que o MSD.

### Radix Sort MSD (`radix_sort_msd.c`)

Ordena do digito mais significativo ao menos significativo, recursivamente dentro de cada bucket.
- Suporta strings de comprimento variavel.
- Pode parar cedo (bucket com 1 elemento ja esta ordenado).
- **American Flag Sort**: variante in-place do MSD para strings.

### Bucket Sort (`bucket_sort.c`)

Distribui os elementos em K buckets por uma funcao de mapeamento e ordena cada bucket com Insertion Sort.
- O(n) caso medio com distribuicao uniforme.
- **Versao para floats em [0,1)**: o mais comum na literatura.
- **Versao para inteiros**: usa vetores dinamicos por bucket.

---

## Como compilar

```bash
# Exemplo
gcc -Wall -O2 -o bubble ordenacao/simples/bubble_sort.c && ./bubble
gcc -Wall -O2 -o merge  ordenacao/eficientes/merge_sort.c && ./merge
gcc -Wall -O2 -o radix  ordenacao/lineares/radix_sort_lsd.c && ./radix
```

---

## Escolha do Algoritmo

```
Dados pequenos (n < 50)?
  -> Insertion Sort

Dados quase ordenados?
  -> Insertion Sort ou Timsort

Precisa de estabilidade?
  -> Merge Sort ou Counting/Radix

Apenas inteiros em intervalo pequeno?
  -> Counting Sort (K pequeno) ou Radix Sort

Melhor caso medio geral (dados aleatorios)?
  -> Quick Sort (Mediana de tres)

Garantia O(n log n) sem memoria extra?
  -> Heap Sort

Strings ou chaves compostas?
  -> Radix Sort MSD (American Flag) ou Merge Sort
```
