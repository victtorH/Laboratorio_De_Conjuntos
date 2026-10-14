#ifndef CONJUNTOS_H
#define CONJUNTOS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Limite maximo de elementos permitidos por conjunto 
 *        para prevencao de estouro de memoria (OOM).
 */
#define MAX_SET_SIZE 1000

/**
 * @brief Estrutura base para gerenciar conjuntos de strings dinamicamente.
 */
typedef struct {
    char **elements;   /**< Vetor dinamico de ponteiros para strings. */
    size_t size;       /**< Quantidade atual de elementos validos na caixa. */
    size_t capacity;   /**< Capacidade maxima alocada no momento. */
} Set;

/**
 * @brief Cria e inicializa um conjunto dinamicamente.
 * @param capacity Capacidade inicial de elementos a ser reservada na memoria.
 * @return Retorna um ponteiro para o novo conjunto criado, ou NULL em caso de falha de alocacao.
 */
Set* set_create(size_t capacity);

/**
 * @brief Libera completamente a memoria de um conjunto.
 *        Garante a liberacao individual de cada string armazenada antes de liberar a estrutura principal.
 * @param s Ponteiro para o conjunto que sera destruido.
 */
void set_free(Set *s);

/**
 * @brief Verifica se um elemento textual existe no conjunto (Pertinencia).
 * @param s Ponteiro para o conjunto a ser consultado.
 * @param value String do elemento procurado.
 * @return Retorna true se o elemento pertencer rigorosamente a caixa, false caso contrario.
 */
bool set_contains(const Set *s, const char *value);

/**
 * @brief Adiciona uma string ao conjunto, gerenciando a alocacao e impedindo duplicatas.
 * @param s Ponteiro para o conjunto alvo.
 * @param value Elemento textual a ser copiado e inserido.
 * @return Retorna true se inserido com sucesso, false se o elemento ja existia ou ocorreu erro de memoria.
 */
bool set_add(Set *s, const char *value);

/**
 * @brief Calcula a uniao matematica entre dois conjuntos (A U B).
 * @param A Ponteiro para o primeiro conjunto.
 * @param B Ponteiro para o segundo conjunto.
 * @return Retorna um novo conjunto contendo todos os elementos de A e B sem elementos duplicados.
 */
Set* set_union(const Set *A, const Set *B);

/**
 * @brief Calcula a intersecao matematica entre dois conjuntos (A intersecao B).
 * @param A Ponteiro para o primeiro conjunto.
 * @param B Ponteiro para o segundo conjunto.
 * @return Retorna um novo conjunto contendo apenas os elementos comuns a A e B.
 */
Set* set_intersection(const Set *A, const Set *B);

/**
 * @brief Calcula a diferenca matematica entre dois conjuntos (A - B).
 * @param A Ponteiro para o conjunto base.
 * @param B Ponteiro para o conjunto subtrator.
 * @return Retorna um novo conjunto com os elementos exclusivos de A (que nao estao em B).
 */
Set* set_difference(const Set *A, const Set *B);

/**
 * @brief Verifica se todos os elementos de A estao contidos em B (Inclusao / A contido em B).
 * @param A Ponteiro para o subconjunto candidato.
 * @param B Ponteiro para o superconjunto candidato.
 * @return Retorna true se A estiver totalmente contido em B, incluindo o caso de A ser vazio.
 */
bool set_is_subset(const Set *A, const Set *B);

/**
 * @brief Retorna a cardinalidade de um conjunto matematico (|A|).
 * @param s Ponteiro para o conjunto.
 * @return Retorna a quantidade de elementos validos no conjunto.
 */
size_t set_cardinality(const Set *s);

/**
 * @brief Imprime os elementos do conjunto formatados no terminal.
 * @param s Ponteiro para o conjunto que sera impresso.
 */
void set_print(const Set *s);

#endif