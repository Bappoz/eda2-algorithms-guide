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
