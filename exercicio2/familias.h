/**
 * @file familias.h
 * @brief Contratos, estruturas e enumerações para o processamento matemático das famílias numéricas.
 * 
 * Este arquivo define a interface pública do módulo de análise de números,
 * garantindo o desacoplamento entre a lógica matemática e a interface de usuário.
 */

#ifndef FAMILIAS_H
#define FAMILIAS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Bitmask de representação das famílias matemáticas.
 * Permite a combinação de múltiplos conjuntos aos quais um número pertence.
 */
typedef enum {
    FAM_NONE = 0x00, /*!< Nenhuma família associada */
    FAM_N    = 0x01, /*!< Naturais (N) */
    FAM_Z    = 0x02, /*!< Inteiros (Z) */
    FAM_Q    = 0x04, /*!< Racionais (Q) */
    FAM_I    = 0x08, /*!< Irracionais (I) */
    FAM_R    = 0x10  /*!< Reais (R) */
} FamiliaNumerica;

/**
 * @brief Classificação semântica para orientar a geração de justificativas.
 * Evita que a interface precise deduzir regras matemáticas.
 */
typedef enum {
    TIPO_DESCONHECIDO = 0,     /*!< Valor não classificado ou inválido */
    TIPO_NATURAL,              /*!< Inteiro não negativo (incluindo zero matemático) */
    TIPO_INTEIRO_NEGATIVO,     /*!< Inteiro estritamente negativo */
    TIPO_FRACAO,               /*!< Fração que não resulta em inteiro */
    TIPO_DECIMAL_EXATO,        /*!< Número com casas decimais finitas */
    TIPO_DIZIMA,               /*!< Dízima periódica formatada com reticências */
    TIPO_IRRACIONAL,           /*!< Raiz quadrada não exata */
    TIPO_RAIZ_EXATA            /*!< Raiz quadrada que resulta em número inteiro */
} TipoEntrada;

/**
 * @brief Códigos de erro da análise e sanitização matemática.
 */
typedef enum {
    ERR_OK = 0,             /*!< Sucesso na análise matemática */
    ERR_EMPTY_INPUT,        /*!< Entrada vazia ou contendo apenas espaços */
    ERR_INVALID_FORMAT,     /*!< Sintaxe corrompida ou caracteres residuais inválidos */
    ERR_DIV_ZERO,           /*!< Tentativa matemática de divisão por zero */
    ERR_OVERFLOW,           /*!< Valor ultrapassou os limites seguros do tipo numérico */
    ERR_NON_REAL_ROOT       /*!< Radicando negativo (fora do escopo dos Reais) */
} ErrorCode;

/**
 * @brief Estrutura que encapsula o resultado completo da análise numérica.
 */
typedef struct {
    uint8_t familias;           /*!< Bitmask das famílias correspondentes */
    char fracao_geratriz[64];   /*!< Representação textual da fração (irredutível) */
    long long valor_inteiro;    /*!< Valor simplificado (se pertencer aos Inteiros) */
    TipoEntrada tipo;           /*!< Natureza matemática real identificada */
    ErrorCode status;           /*!< Status do processamento */
} ResultadoClassificacao;

/**
 * @brief Analisa a string numérica, processa seu valor seguro e classifica nos conjuntos.
 * 
 * @param input String contendo a entrada bruta fornecida pelo usuário.
 * @return ResultadoClassificacao Struct contendo as classificações, dados extraídos ou erros detectados.
 */
ResultadoClassificacao classificar_numero(const char* input);

#endif /* FAMILIAS_H */