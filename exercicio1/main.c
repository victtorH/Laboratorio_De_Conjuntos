#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "conjuntos.h"
#include "terminal_ui.h"

#define MAX_INPUT_LEN 256

/**
 * @brief Le um texto de forma segura via terminal, removendo quebras de linha e impedindo o uso de espacos.
 * @param prompt Mensagem exibida ao usuario antes da leitura.
 * @param buffer Ponteiro para o array de caracteres onde a string sera armazenada.
 * @param size Tamanho maximo do buffer.
 * @return Retorna true em caso de sucesso na leitura, ou false caso detecte Fim de Arquivo (EOF) ou erro critico.
 */
static bool read_string_secure(const char *prompt, char *buffer, size_t size) {
    while (true) {
        printf("%s", prompt);
        if (!fgets(buffer, (int)size, stdin)) {
            clearerr(stdin);
            return false;
        }

        size_t len = strlen(buffer);
        
        if (len == size - 1 && buffer[len - 1] != '\n' && buffer[len - 1] != '\r') {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            tui_print_warning("A entrada excede o tamanho permitido. Informe um valor menor.");
            continue;
        }

        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[len - 1] = '\0';
            len--;
        }

        bool has_spaces = false;
        for (size_t i = 0; i < len; i++) {
            if (isspace((unsigned char)buffer[i])) {
                has_spaces = true;
                break;
            }
        }

        if (len == 0 || has_spaces) {
            tui_print_warning("Espacos nao sao permitidos. Digite um elemento unico e sem espacos.");
            continue;
        }
        
        return true;
    }
}

/**
 * @brief Le e valida um numero longo a partir da entrada de texto seguro.
 * @param prompt Mensagem exibida ao usuario antes da leitura.
 * @param success Ponteiro para flag booleana que indica se a operacao obteve sucesso ou encontrou EOF.
 * @return Retorna o numero inteiro digitado. Retorna -1 apenas se success se tornar false por EOF.
 */
static long read_long_secure(const char *prompt, bool *success) {
    char buffer[64];
    long val = -1;
    
    if (success) *success = true;

    while (true) {
        if (!read_string_secure(prompt, buffer, sizeof(buffer))) {
            if (success) *success = false;
            return -1;
        }
        
        char *endptr;
        val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\0') {
            tui_print_warning("Entrada invalida. Digite um numero inteiro valido.");
        } else {
            break;
        }
    }
    return val;
}

/**
 * @brief Orquestra a interacao completa com o usuario para preencher um novo conjunto de dados.
 * @param name Rotulo de identificacao do conjunto (ex: "A" ou "B").
 * @param eof_flag Ponteiro para flag booleana que detecta e relata quebra por Fim de Arquivo (EOF).
 * @return Retorna o novo conjunto criado e populado, ou um conjunto vazio em caso de interrupcao segura.
 */
Set* read_set_from_user(const char* name, bool *eof_flag) {
    char prompt[128];
    snprintf(prompt, sizeof(prompt), ANSI_BOLD "Informe o tamanho da caixa %s: " ANSI_RESET, name);
    
    bool success = true;
    long size;

    while (true) {
        size = read_long_secure(prompt, &success);
        if (!success) {
            if (eof_flag) *eof_flag = true;
            return set_create(0);
        }

        if (size > MAX_SET_SIZE) {
            char warn[128];
            snprintf(warn, sizeof(warn), "Tamanho excede o limite maximo permitido (%d). Informe um valor valido.", MAX_SET_SIZE);
            tui_print_warning(warn);
            continue;
        }
        break;
    }

    if (size <= 0) {
        char warn[128];
        snprintf(warn, sizeof(warn), "Tamanho %s. Assumindo conjunto vazio.", size < 0 ? "negativo" : "zero");
        tui_print_warning(warn);
        return set_create(0);
    }

    Set *s = set_create((size_t)size);
    if (!s) {
        tui_print_error("Falha ao alocar memoria para o conjunto. Sistema abortado.");
        exit(EXIT_FAILURE);
    }

    printf(ANSI_BOLD "Digite os %ld elementos da caixa %s (um por linha, sem espacos):\n" ANSI_RESET, size, name);
    char buffer[MAX_INPUT_LEN];
    for (long i = 0; i < size; i++) {
        snprintf(prompt, sizeof(prompt), "Elemento %ld: ", i + 1);
        if (!read_string_secure(prompt, buffer, sizeof(buffer))) {
            tui_print_warning("Fim de entrada ou falha critica na leitura. Abortando preenchimento.");
            if (eof_flag) *eof_flag = true;
            break;
        }
        set_add(s, buffer);
    }
    return s;
}

/**
 * @brief Converte o codigo numerico da operacao matematica para sua representacao em texto.
 * @param opcao Codigo inteiro da operacao selecionada.
 * @return Retorna uma string constante com o nome da operacao.
 */
const char* get_operation_name(int opcao) {
    switch (opcao) {
        case 1: return "Uniao (A U B)";
        case 2: return "Intersecao (A ^ B)";
        case 3: return "Diferenca (A - B)";
        case 4: return "Pertinencia (x em A)";
        case 5: return "Inclusao (A contido em B)";
        case 6: return "Cardinalidade (|A|)";
        default: return "Desconhecida";
    }
}

/**
 * @brief Ponto de entrada do executavel.
 *        Gerencia a TUI (Terminal User Interface), o ciclo interativo e a limpeza de memoria.
 * @return Retorna 0 apos o termino seguro do programa.
 */
int main() {
    setbuf(stdout, NULL);
    char continuar = 'S';
    bool eof_detected = false;
    
    do {
        tui_clear_screen();
        tui_print_header();
        
        Set *A = read_set_from_user("A", &eof_detected);
        if (eof_detected) {
            set_free(A);
            break;
        }
        printf("\n");

        Set *B = read_set_from_user("B", &eof_detected);
        if (eof_detected) {
            set_free(A);
            set_free(B);
            break;
        }

        tui_clear_screen();
        tui_print_header();
        tui_print_set("A", A);
        tui_print_set("B", B);

        int opcao = -1;
        while (opcao < 0 || opcao > 6) {
            tui_print_menu();
            bool success = true;
            opcao = (int)read_long_secure(ANSI_BOLD "\nSelecione uma operacao: " ANSI_RESET, &success);
            
            if (!success) {
                eof_detected = true;
                break;
            }
            if (opcao < 0 || opcao > 6) {
                tui_print_warning("Opcao invalida. Escolha entre 0 e 6.\n");
            }
        }

        if (eof_detected) {
            set_free(A);
            set_free(B);
            break;
        }

        if (opcao == 0) {
            set_free(A);
            set_free(B);
            continuar = 'N';
            break;
        }

        tui_clear_screen();
        tui_print_header();
        tui_print_result_header(get_operation_name(opcao));

        Set *res = NULL;
        char buffer[MAX_INPUT_LEN];

        switch(opcao) {
            case 1:
                res = set_union(A, B);
                tui_print_set("A U B (Uniao)", res);
                set_free(res);
                break;
            case 2:
                res = set_intersection(A, B);
                tui_print_set("A intersecao B", res);
                set_free(res);
                break;
            case 3:
                res = set_difference(A, B);
                tui_print_set("A - B (Diferenca)", res);
                set_free(res);
                break;
            case 4:
                if (read_string_secure(ANSI_BOLD "Informe o valor textual para buscar em A: " ANSI_RESET, buffer, sizeof(buffer))) {
                    printf("\n");
                    if (set_contains(A, buffer)) {
                        tui_print_success("O elemento informado pertence a caixa A.");
                    } else {
                        tui_print_warning("O elemento informado nao esta na caixa A.");
                    }
                } else {
                    eof_detected = true;
                }
                break;
            case 5:
                if (set_is_subset(A, B)) {
                    tui_print_success("Verdadeiro. A caixa A esta contida na caixa B.");
                } else {
                    tui_print_warning("Falso. A caixa A nao esta totalmente contida na caixa B.");
                }
                break;
            case 6: {
                char msg[128];
                snprintf(msg, sizeof(msg), "A cardinalidade da caixa A e %zu.", set_cardinality(A));
                tui_print_success(msg);
                break;
            }
        }

        if (eof_detected) {
            set_free(A);
            set_free(B);
            break;
        }

        set_free(A);
        set_free(B);

        char input[64];
        printf("\n");
        if (!read_string_secure(ANSI_BOLD "Deseja realizar outra operacao com novas caixas? (S/N): " ANSI_RESET, input, sizeof(input))) {
            eof_detected = true;
            break;
        }
        continuar = input[0];

    } while (continuar == 'S' || continuar == 's');

    if (eof_detected) {
         tui_print_warning("Fim de entrada detectado (EOF). Encerrando a aplicacao com seguranca.");
    } else {
         tui_print_success("Encerrando o programa.");
    }
    
    return 0;
}