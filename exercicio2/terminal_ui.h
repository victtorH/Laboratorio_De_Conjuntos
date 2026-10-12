/**
 * @file terminal_ui.h
 * @brief Contratos da interface visual de terminal (TUI).
 * 
 * Centraliza e padroniza a exibição de resultados, erros e interações.
 * Mantém o orquestrador principal e a lógica matemática livres de formatações ANSI.
 */

#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "familias.h"

/**
 * @brief Exibe o cabeçalho principal e único da aplicação.
 * @param title Nome do aplicativo a ser destacado.
 */
void ui_print_header(const char* title);

/**
 * @brief Imprime uma linha separadora para delimitar blocos de informação.
 */
void ui_print_separator(void);

/**
 * @brief Exibe uma mensagem informativa formatada com o prefixo [i].
 * @param msg Texto informativo a ser apresentado.
 */
void ui_print_info(const char* msg);

/**
 * @brief Exibe um alerta não bloqueante com o prefixo [!].
 * @param msg Texto do aviso a ser apresentado.
 */
void ui_print_warning(const char* msg);

/**
 * @brief Exibe uma mensagem de falha severa com o prefixo [x].
 * @param msg Texto do erro a ser apresentado.
 */
void ui_print_error(const char* msg);

/**
 * @brief Apresenta o prompt de entrada e descarrega o buffer de saída (fflush).
 * @param prompt_msg Texto orientativo exibido antes de aguardar a digitação.
 */
void ui_prompt(const char* prompt_msg);

/**
 * @brief Formata e exibe os resultados da análise matemática e suas respectivas justificativas.
 * 
 * @param input A string original digitada pelo usuário.
 * @param res A estrutura de classificação preenchida pelo motor matemático.
 */
void ui_print_results(const char* input, ResultadoClassificacao res);

/**
 * @brief Exibe formatação específica e padronizada para rejeição de raízes com radicandos negativos.
 * @param input A string original digitada pelo usuário.
 */
void ui_print_non_real_root(const char* input);

#endif /* TERMINAL_UI_H */