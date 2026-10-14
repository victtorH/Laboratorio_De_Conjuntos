/**
 * @file terminal_ui.c
 * @brief Implementação da interface visual de terminal (TUI).
 * 
 * Contém a formatação ANSI e as lógicas de impressão na tela.
 * Este módulo não realiza cálculos, apenas reflete o estado matemático recebido.
 */

#include <stdio.h>
#include "terminal_ui.h"

/* Constantes visuais restritas à implementação gráfica */
#define UI_RESET   "\x1b[0m"
#define UI_CYAN    "\x1b[36m"
#define UI_GREEN   "\x1b[32m"
#define UI_YELLOW  "\x1b[33m"
#define UI_RED     "\x1b[31m"
#define UI_BOLD    "\x1b[1m"

void ui_print_header(const char* title) {
    printf("\n%s==================================================%s\n", UI_CYAN, UI_RESET);
    printf("%s%s  %s%s\n", UI_CYAN, UI_BOLD, title, UI_RESET);
    printf("%s==================================================%s\n", UI_CYAN, UI_RESET);
}

void ui_print_separator(void) {
    printf("%s--------------------------------------------------%s\n", UI_CYAN, UI_RESET);
}

void ui_print_info(const char* msg) {
    printf("%s[i]%s %s\n", UI_CYAN, UI_RESET, msg);
}

void ui_print_warning(const char* msg) {
    printf("%s[!]%s %s\n", UI_YELLOW, UI_RESET, msg);
}

void ui_print_error(const char* msg) {
    printf("%s[x]%s %s\n", UI_RED, UI_RESET, msg);
}

void ui_prompt(const char* prompt_msg) {
    printf("\n%s%s%s", UI_BOLD, prompt_msg, UI_RESET);
    fflush(stdout);
}

void ui_print_non_real_root(const char* input) {
    char buffer[256];
    ui_print_separator();
    snprintf(buffer, sizeof(buffer), "Número analisado: %s\n", input);
    printf("%s", buffer);
    ui_print_error("Nao pertence a N, Z, Q, I ou R.");
    ui_print_info("Motivo: a raiz quadrada de um numero negativo nao possui resultado no conjunto dos reais.");
    ui_print_separator();
}

void ui_print_results(const char* input, ResultadoClassificacao res) {
    char msg_buffer[256];
    
    ui_print_separator();
    if (res.tipo == TIPO_RAIZ_EXATA) {
        snprintf(msg_buffer, sizeof(msg_buffer), "Número analisado: %s = %lld", input, res.valor_inteiro);
    } else {
        snprintf(msg_buffer, sizeof(msg_buffer), "Número analisado: %s", input);
    }
    ui_print_info(msg_buffer);
    printf("\n");

    /* ====================== PERTENCE ====================== */
    printf("%sPertence a:%s\n", UI_GREEN, UI_RESET);
    if (res.familias & FAM_N) {
        printf("  %s[+]%s [N] Naturais: ", UI_GREEN, UI_RESET);
        if (res.tipo == TIPO_RAIZ_EXATA) printf("o resultado e um inteiro nao negativo.\n");
        else printf("e um numero inteiro nao negativo.\n");
    }
    if (res.familias & FAM_Z) {
        printf("  %s[+]%s [Z] Inteiros: ", UI_GREEN, UI_RESET);
        if (res.tipo == TIPO_RAIZ_EXATA) printf("o resultado nao possui parte decimal.\n");
        else if (res.tipo == TIPO_INTEIRO_NEGATIVO) printf("e um numero inteiro negativo.\n");
        else printf("nao possui parte decimal.\n");
    }
    if (res.familias & FAM_Q) {
        printf("  %s[+]%s [Q] Racionais: ", UI_GREEN, UI_RESET);
        if (res.tipo == TIPO_NATURAL || res.tipo == TIPO_INTEIRO_NEGATIVO || res.tipo == TIPO_RAIZ_EXATA) {
            printf("pode ser escrito como %lld/1.\n", res.valor_inteiro);
        } else if (res.tipo == TIPO_DECIMAL_EXATO) {
            printf("e um decimal exato, equivalente a %s.\n", res.fracao_geratriz);
        } else if (res.tipo == TIPO_DIZIMA) {
            printf("e uma dizima periodica com fracao geratriz %s.\n", res.fracao_geratriz);
        } else {
            printf("e uma fracao exata equivalente a %s.\n", res.fracao_geratriz);
        }
    }
    if (res.familias & FAM_I) {
        printf("  %s[+]%s [I] Irracionais: sua raiz nao e exata e nao pode ser expressa como fracao de inteiros.\n", UI_GREEN, UI_RESET);
    }
    if (res.familias & FAM_R) {
        printf("  %s[+]%s [R] Reais: ", UI_GREEN, UI_RESET);
        if (res.familias & FAM_Q) printf("todo numero racional e real.\n");
        else printf("e um numero real irracional.\n");
    }

    /* ==================== NÃO PERTENCE ==================== */
    printf("\n%sNao pertence a:%s\n", UI_RED, UI_RESET);
    if (!(res.familias & FAM_N)) {
        printf("  %s[-]%s [N] Naturais: ", UI_RED, UI_RESET);
        if (res.tipo == TIPO_INTEIRO_NEGATIVO) printf("e negativo.\n");
        else if (res.tipo == TIPO_IRRACIONAL) printf("nao possui valor inteiro.\n");
        else printf("nao e um inteiro nao negativo.\n");
    }
    if (!(res.familias & FAM_Z)) {
        printf("  %s[-]%s [Z] Inteiros: ", UI_RED, UI_RESET);
        if (res.tipo == TIPO_IRRACIONAL) printf("sua raiz nao resulta em um inteiro.\n");
        else printf("possui parte fracionaria.\n");
    }
    if (!(res.familias & FAM_Q)) {
        printf("  %s[-]%s [Q] Racionais: ", UI_RED, UI_RESET);
        if (res.tipo == TIPO_IRRACIONAL) printf("nao pode ser representado por uma fracao exata de inteiros.\n");
    }
    if (!(res.familias & FAM_I)) {
        printf("  %s[-]%s [I] Irracionais: ", UI_RED, UI_RESET);
        if (res.tipo == TIPO_RAIZ_EXATA) printf("a raiz e exata e resulta em um numero inteiro.\n");
        else printf("pode ser representado por uma fracao de inteiros.\n");
    }
    ui_print_separator();
}