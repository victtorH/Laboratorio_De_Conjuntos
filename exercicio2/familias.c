/**
 * @file familias.c
 * @brief Implementação do motor de classificação matemática e processamento de frações geratrizes.
 * 
 * Contém os algoritmos de sanitização léxica, cálculos seguros de mitigação de 
 * transbordamento de memória (overflow) e a aplicação da lógica matemática de conjuntos.
 */

#include "familias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/* =====================================================================
 * Algoritmos Auxiliares de Programação Defensiva
 * ===================================================================== */

/**
 * @brief Converte string para long long com proteção rigorosa contra estouro numérico.
 *
 * Encapsula strtoll() acionando a flag errno e inspecionando ERANGE para rejeitar
 * números cujas magnitudes superam as suportadas pela máquina.
 *
 * @param str String numérica de entrada.
 * @param endptr Ponteiro que marca o primeiro caractere que não pôde ser convertido.
 * @param out_val Ponteiro para armazenar o valor convertido em caso de sucesso.
 * @return ERR_OK se a conversão for segura, ou ERR_OVERFLOW se violou a capacidade do tipo.
 */
static ErrorCode safe_strtoll(const char* str, char** endptr, long long* out_val) {
    errno = 0;
    long long val = strtoll(str, endptr, 10);
    if (errno == ERANGE) {
        return ERR_OVERFLOW;
    }
    *out_val = val;
    return ERR_OK;
}

/**
 * @brief Realiza soma segura validando os limites de long long.
 *
 * @param a Primeiro número.
 * @param b Segundo número.
 * @param res Ponteiro para armazenar a soma.
 * @return true em caso de sucesso; false se ocorrer overflow.
 */
static bool safe_add(long long a, long long b, long long* res) {
    if (b > 0 && a > LLONG_MAX - b) return false;
    if (b < 0 && a < LLONG_MIN - b) return false;
    *res = a + b;
    return true;
}

/**
 * @brief Realiza multiplicação segura validando os limites de long long.
 *
 * @param a Primeiro fator.
 * @param b Segundo fator.
 * @param res Ponteiro para armazenar o produto.
 * @return true em caso de sucesso; false se ocorrer overflow.
 */
static bool safe_mult(long long a, long long b, long long* res) {
    if (a == 0 || b == 0) {
        *res = 0;
        return true;
    }
    if (a > 0 && b > 0 && a > LLONG_MAX / b) return false;
    if (a > 0 && b < 0 && b < LLONG_MIN / a) return false;
    if (a < 0 && b > 0 && a < LLONG_MIN / b) return false;
    if (a < 0 && b < 0 && a < LLONG_MAX / b) return false;
    *res = a * b;
    return true;
}

/**
 * @brief Calcula potência inteira com proteção contra overflow iterativo.
 *
 * @param base Número base.
 * @param exp Expoente positivo.
 * @param overflow Ponteiro sinalizador alterado para true se estourar o limite.
 * @return O valor da potência (o retorno será 0 e a flag marcada caso dê overflow).
 */
static long long safe_int_pow(long long base, int exp, bool* overflow) {
    long long res = 1;
    *overflow = false;
    for (int i = 0; i < exp; i++) {
        if (!safe_mult(res, base, &res)) {
            *overflow = true;
            return 0;
        }
    }
    return res;
}

/**
 * @brief Calcula o Máximo Divisor Comum (MDC) pelo algoritmo de Euclides.
 *
 * Utiliza aritmética sem sinal (unsigned long long) internamente para
 * evitar comportamento indefinido ao calcular o valor absoluto de LLONG_MIN.
 * Garante que as operações de módulo funcionem corretamente em todos os limites.
 *
 * @param a Primeiro número (pode ser negativo, positivo ou zero).
 * @param b Segundo número (pode ser negativo, positivo ou zero).
 * @return O Máximo Divisor Comum positivo entre a e b.
 *
 * @note O retorno da função é convertido para long long, que é o tipo nativo
 * utilizado pelas funções consumidoras no cálculo de frações.
 * @warning Retornar o absoluto de LLONG_MIN causaria overflow no cast de volta
 * para long long. Contudo, o sistema realiza a rejeição prévia de LLONG_MIN
 * como denominador nas rotinas consumidoras.
 */
static long long calcular_mdc(long long a, long long b) {
    unsigned long long ua = (a < 0) ? -(unsigned long long)a : (unsigned long long)a;
    unsigned long long ub = (b < 0) ? -(unsigned long long)b : (unsigned long long)b;
    while (ub != 0) {
        unsigned long long temp = ub;
        ub = ua % ub;
        ua = temp;
    }
    return (long long)ua;
}

/**
 * @brief Realiza busca binária para encontrar raiz quadrada inteira exata.
 *
 * @param n Radicando (número inteiro a ser avaliado).
 * @return A raiz inteira do número, ou -1 caso ele não seja um quadrado perfeito.
 */
static long long raiz_inteira_exata(long long n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return n;
    
    long long low = 1, high = n, ans = -1;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (mid <= n / mid) { 
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (ans * ans == n) ? ans : -1;
}

/**
 * @brief Remove espaços em branco apenas das extremidades da entrada (trim).
 *
 * Elimina caracteres em branco (espaços, tabulações, etc.) do início e
 * do final da string, preservando inteiramente os espaços internos. Isso é
 * vital para que a validação estrutural perceba expressões malformadas
 * como "1 2" e as rejeite corretamente ao invés de formatá-las como "12".
 *
 * @param dest Buffer onde a string higienizada será gravada.
 * @param src String de origem a ser processada.
 *
 * @note Se a entrada contiver apenas espaços ou for vazia, dest conterá "".
 * @warning O chamador deve garantir que dest tem capacidade para src.
 */
static void remover_espacos(char* dest, const char* src) {
    if (!src || !dest) return;

    /* Avança espaços iniciais */
    while (isspace((unsigned char)*src)) {
        src++;
    }

    if (*src == '\0') {
        *dest = '\0';
        return;
    }

    /* Recua até remover espaços finais */
    const char* end = src + strlen(src) - 1;
    while (end > src && isspace((unsigned char)*end)) {
        end--;
    }

    /* Copia o núcleo significativo */
    size_t len = end - src + 1;
    memmove(dest, src, len);
    dest[len] = '\0';
}

/* =====================================================================
 * Motor Principal de Classificação
 * ===================================================================== */

ResultadoClassificacao classificar_numero(const char* input) {
    ResultadoClassificacao res = { FAM_NONE, "", 0, TIPO_DESCONHECIDO, ERR_OK };
    
    if (input == NULL || input[0] == '\0') {
        res.status = ERR_EMPTY_INPUT; 
        return res;
    }

    char limpo[256];
    remover_espacos(limpo, input);
    
    if (strlen(limpo) == 0) {
        res.status = ERR_EMPTY_INPUT; 
        return res;
    }

    /* === CASO 1: Tratamento de Raízes (Reconhecimento UTF-8 e ASCII) === */
    bool is_root = false;
    long long root_val = 0;
    char* radicando_str = NULL;
    bool expects_closing_paren = false;

    if (strncmp(limpo, "sqrt(", 5) == 0) {
        radicando_str = limpo + 5;
        expects_closing_paren = true;
    } 
    else if (strncmp(limpo, "\xE2\x88\x9A", 3) == 0) { 
        radicando_str = limpo + 3;
    } 
    else if (limpo[0] == 'V' || limpo[0] == 'v') {
        radicando_str = limpo + 1;
    }

    if (radicando_str != NULL) {
        if (!expects_closing_paren && radicando_str[0] == '(') {
            radicando_str++;
            expects_closing_paren = true;
        }
        size_t len = strlen(radicando_str);
        if (expects_closing_paren) {
            if (len > 0 && radicando_str[len - 1] == ')') {
                radicando_str[len - 1] = '\0';
                len--;
            } else {
                radicando_str = NULL; 
            }
        }
        if (radicando_str != NULL && len > 0) {
            char* endptr;
            if (safe_strtoll(radicando_str, &endptr, &root_val) == ERR_OVERFLOW) {
                res.status = ERR_OVERFLOW; 
                return res;
            }
            if (*endptr == '\0' && endptr != radicando_str) {
                is_root = true;
            }
        }
    }

    if (is_root) {
        if (root_val < 0) {
            res.status = ERR_NON_REAL_ROOT; 
            return res;
        }
        long long raiz_exata = raiz_inteira_exata(root_val);
        if (raiz_exata != -1) {
            res.tipo = TIPO_RAIZ_EXATA;
            res.valor_inteiro = raiz_exata;
            res.familias |= FAM_R | FAM_Q | FAM_Z | FAM_N;
        } else {
            res.tipo = TIPO_IRRACIONAL;
            res.familias |= FAM_R | FAM_I;
        }
        return res;
    }

    /* === CASO 2: Análise de Frações Explicitas === */
    char* barra = strchr(limpo, '/');
    if (barra != NULL) {
        *barra = '\0';
        char* num_str = limpo;
        char* den_str = barra + 1;
        
        char* endptr1, *endptr2;
        long long num, den;
        
        if (safe_strtoll(num_str, &endptr1, &num) == ERR_OVERFLOW || 
            safe_strtoll(den_str, &endptr2, &den) == ERR_OVERFLOW) {
            res.status = ERR_OVERFLOW; 
            return res;
        }
        
        if (*endptr1 != '\0' || *endptr2 != '\0' || strlen(den_str) == 0) {
            res.status = ERR_INVALID_FORMAT; 
            return res;
        }
        if (den == 0) {
            res.status = ERR_DIV_ZERO; 
            return res;
        }
        
        if (den < 0) { 
            if (num == LLONG_MIN || den == LLONG_MIN) { 
                res.status = ERR_OVERFLOW; 
                return res; 
            }
            num = -num; 
            den = -den; 
        }

        long long mdc = calcular_mdc(num, den);
        num /= mdc; 
        den /= mdc;
        
        snprintf(res.fracao_geratriz, sizeof(res.fracao_geratriz), "%lld/%lld", num, den);
        res.familias |= FAM_R | FAM_Q;
        
        if (den == 1) { 
            res.familias |= FAM_Z;
            res.valor_inteiro = num;
            if (num >= 0) { res.familias |= FAM_N; res.tipo = TIPO_NATURAL; }
            else { res.tipo = TIPO_INTEIRO_NEGATIVO; }
        } else {
            res.tipo = TIPO_FRACAO;
        }
        return res;
    }

    /* === CASO 3: Decimais e Dízimas Periódicas === */
    char* reticencias = strstr(limpo, "...");
    bool is_periodic = (reticencias != NULL);
    if (is_periodic) *reticencias = '\0';
    
    char* separador_decimal = strchr(limpo, '.');
    if (!separador_decimal) separador_decimal = strchr(limpo, ',');

    if (separador_decimal) {
        *separador_decimal = '\0';
        char* int_str = limpo;
        char* dec_str = separador_decimal + 1;
        
        char* endptr1, *endptr2;
        long long num_int, num_dec;
        
        if (strlen(int_str) == 0 || (strlen(int_str) == 1 && int_str[0] == '-')) {
             num_int = 0;
             endptr1 = int_str + strlen(int_str);
        } else {
             if (safe_strtoll(int_str, &endptr1, &num_int) == ERR_OVERFLOW) {
                 res.status = ERR_OVERFLOW; 
                 return res;
             }
        }

        if (safe_strtoll(dec_str, &endptr2, &num_dec) == ERR_OVERFLOW) {
            res.status = ERR_OVERFLOW; 
            return res;
        }

        if (*endptr1 != '\0' || *endptr2 != '\0' || strlen(dec_str) == 0) {
            res.status = ERR_INVALID_FORMAT; 
            return res;
        }
        
        int casas = strlen(dec_str);
        bool is_negative = (limpo[0] == '-');
        
        if (is_periodic) {
            bool overflow = false;
            long long den_base = safe_int_pow(10, casas, &overflow);
            if (overflow) { res.status = ERR_OVERFLOW; return res; }
            den_base -= 1;
            
            if (num_int == LLONG_MIN) { res.status = ERR_OVERFLOW; return res; }
            
            long long part1, numerador_absoluto;
            if (!safe_mult(llabs(num_int), den_base, &part1) || !safe_add(part1, num_dec, &numerador_absoluto)) {
                res.status = ERR_OVERFLOW; return res;
            }

            if (is_negative) numerador_absoluto = -numerador_absoluto;

            long long mdc = calcular_mdc(numerador_absoluto, den_base);
            snprintf(res.fracao_geratriz, sizeof(res.fracao_geratriz), "%lld/%lld", numerador_absoluto/mdc, den_base/mdc);
            
            res.tipo = TIPO_DIZIMA;
            res.familias |= FAM_R | FAM_Q; 
            return res;
        } else {
            if (num_dec == 0) { 
                res.valor_inteiro = num_int;
                res.familias |= FAM_R | FAM_Q | FAM_Z;
                if (num_int >= 0) { res.familias |= FAM_N; res.tipo = TIPO_NATURAL; }
                else { res.tipo = TIPO_INTEIRO_NEGATIVO; }
                return res;
            }
            
            bool overflow = false;
            long long den = safe_int_pow(10, casas, &overflow);
            if (overflow) { res.status = ERR_OVERFLOW; return res; }
            
            if (num_int == LLONG_MIN) { res.status = ERR_OVERFLOW; return res; }
            
            long long part1, num_total;
            if (!safe_mult(llabs(num_int), den, &part1) || !safe_add(part1, num_dec, &num_total)) {
                res.status = ERR_OVERFLOW; return res;
            }
            
            if (is_negative) num_total = -num_total;
            
            long long mdc = calcular_mdc(num_total, den);
            snprintf(res.fracao_geratriz, sizeof(res.fracao_geratriz), "%lld/%lld", num_total/mdc, den/mdc);
            
            res.tipo = TIPO_DECIMAL_EXATO;
            res.familias |= FAM_R | FAM_Q;
            return res;
        }
    }

    /* === CASO 4: Inteiros Básicos === */
    char* endptr;
    long long num;
    if (safe_strtoll(limpo, &endptr, &num) == ERR_OVERFLOW) {
        res.status = ERR_OVERFLOW;
        return res;
    }
    
    if (*endptr != '\0') {
        res.status = ERR_INVALID_FORMAT; 
        return res;
    }
    
    res.valor_inteiro = num;
    res.familias |= FAM_R | FAM_Q | FAM_Z;
    
    if (num >= 0) {
        res.familias |= FAM_N;
        res.tipo = TIPO_NATURAL;
    } else {
        res.tipo = TIPO_INTEIRO_NEGATIVO;
    }
    
    return res;
}