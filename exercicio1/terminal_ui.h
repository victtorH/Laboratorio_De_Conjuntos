#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "conjuntos.h"

/* Dicionario de Cores ANSI */
#define ANSI_RESET   "\x1b[0m"
#define ANSI_BLACK   "\x1b[30m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_WHITE   "\x1b[37m"

/* Estilos de Fonte */
#define ANSI_BOLD    "\x1b[1m"
#define ANSI_UNDERLINE "\x1b[4m"

/* Controles de Tela e Cursor */
#define CLEAR_SCREEN() printf("\x1b[2J\x1b[H")
#define HIDE_CURSOR()  printf("\x1b[?25l")
#define SHOW_CURSOR()  printf("\x1b[?25h")

/**
 * @brief Limpa o terminal e reposiciona o cursor.
 */
void tui_clear_screen(void);

/**
 * @brief Imprime o cabecalho oficial da aplicacao em formato de banner.
 */
void tui_print_header(void);

/**
 * @brief Apresenta o menu de operacoes matematicas suportadas.
 */
void tui_print_menu(void);

/**
 * @brief Renderiza um conjunto num cartao visual com sua cardinalidade e itens.
 * @param name Identificador do conjunto (Ex: "A", "B", "A U B").
 * @param s Ponteiro para a estrutura Set.
 */
void tui_print_set(const char *name, const Set *s);

/**
 * @brief Imprime o banner indicador de tela de resultado de operacao.
 * @param operation O nome da operacao recem-executada.
 */
void tui_print_result_header(const char *operation);

/**
 * @brief Dispara uma mensagem visual e colorida de sucesso no padrao do sistema.
 */
void tui_print_success(const char *msg);

/**
 * @brief Dispara uma mensagem visual e colorida de erro critico.
 */
void tui_print_error(const char *msg);

/**
 * @brief Dispara uma mensagem visual e colorida de alerta / validacao defensiva.
 */
void tui_print_warning(const char *msg);

#endif