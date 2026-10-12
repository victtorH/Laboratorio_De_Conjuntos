#include "terminal_ui.h"
#include <stdio.h>
#include <string.h>

void tui_clear_screen(void) {
    CLEAR_SCREEN();
}

void tui_print_header(void) {
    printf(ANSI_CYAN ANSI_BOLD);
    printf("┌──────────────────────────────────────────────────────────┐\n");
    printf("│                 ROBO ARRUMADOR DE CAIXAS                 │\n");
    printf("│                   TEORIA DOS CONJUNTOS                   │\n");
    printf("├──────────────────────────────────────────────────────────┤\n");
    printf("│           Manipulacao interativa de conjuntos            │\n");
    printf("└──────────────────────────────────────────────────────────┘\n");
    printf(ANSI_RESET "\n");
}

void tui_print_menu(void) {
    printf(ANSI_CYAN "┌──────────────────────────────────────────────────────────┐\n");
    printf("│" ANSI_RESET ANSI_BOLD "                      MENU PRINCIPAL                      " ANSI_RESET ANSI_CYAN "│\n");
    printf("├──────────────────────────────────────────────────────────┤\n");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "1. Uniao (A U B)");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "2. Intersecao (A intersecao B)");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "3. Diferenca (A - B)");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "4. Pertinencia (x em A)");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "5. Inclusao (A contido em B)");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "6. Cardinalidade (|A|)");
    printf("│" ANSI_RESET " %-56s " ANSI_CYAN "│\n", "0. Sair");
    printf("└──────────────────────────────────────────────────────────┘\n" ANSI_RESET);
}

void tui_print_set(const char *name, const Set *s) {
    printf(ANSI_CYAN "┌──────────────────────────────────────────────────────────┐\n" ANSI_RESET);
    printf(ANSI_CYAN "│" ANSI_RESET ANSI_BOLD " CAIXA %-50s " ANSI_RESET ANSI_CYAN "│\n" ANSI_RESET, name);
    printf(ANSI_CYAN "│" ANSI_RESET " Cardinalidade: %-41zu " ANSI_CYAN "│\n" ANSI_RESET, s ? s->size : 0);
    printf(ANSI_CYAN "├──────────────────────────────────────────────────────────┤\n" ANSI_RESET);
    
    printf(ANSI_CYAN "│ " ANSI_RESET);
    
    if (!s || s->size == 0) {
        printf("%-56s", "{ } (Vazia)");
        printf(ANSI_CYAN " │\n" ANSI_RESET);
    } else {
        /* Sistema basico de quebra de linhas dinamica para elementos textuais */
        int line_len = 2; // Inicia com o tamanho de "{ "
        printf("{ ");
        for (size_t i = 0; i < s->size; i++) {
            char item[256];
            snprintf(item, sizeof(item), "\"%s\"%s", s->elements[i], (i < s->size - 1) ? ", " : " }");
            int item_len = (int)strlen(item);
            
            if (line_len + item_len > 56) {
                while (line_len < 56) { printf(" "); line_len++; }
                printf(ANSI_CYAN " │\n│ " ANSI_RESET);
                printf("  "); 
                line_len = 2;
            }
            printf("%s", item);
            line_len += item_len;
        }
        while (line_len < 56) { printf(" "); line_len++; }
        printf(ANSI_CYAN " │\n" ANSI_RESET);
    }
    printf(ANSI_CYAN "└──────────────────────────────────────────────────────────┘\n\n" ANSI_RESET);
}

void tui_print_result_header(const char *operation) {
    printf(ANSI_CYAN "┌──────────────────────────────────────────────────────────┐\n");
    printf("│" ANSI_RESET ANSI_BOLD "                        RESULTADO                         " ANSI_RESET ANSI_CYAN "│\n");
    printf("├──────────────────────────────────────────────────────────┤\n");
    printf("│" ANSI_RESET " Operacao: %-45s " ANSI_CYAN "│\n", operation);
    printf("└──────────────────────────────────────────────────────────┘\n\n" ANSI_RESET);
}

void tui_print_success(const char *msg) {
    printf(ANSI_GREEN ANSI_BOLD "[Sucesso]" ANSI_RESET ANSI_GREEN " %s\n" ANSI_RESET, msg);
}

void tui_print_error(const char *msg) {
    printf(ANSI_RED ANSI_BOLD "[Erro]" ANSI_RESET ANSI_RED " %s\n" ANSI_RESET, msg);
}

void tui_print_warning(const char *msg) {
    printf(ANSI_YELLOW ANSI_BOLD "[Aviso]" ANSI_RESET ANSI_YELLOW " %s\n" ANSI_RESET, msg);
}