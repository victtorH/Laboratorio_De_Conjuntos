/**
 * @file main_exe2.c
 * @brief Orquestrador de inicialização e ciclo de vida do Robô Auxiliar de Matemática (exe2).
 * 
 * Conecta a entrada do usuário ao módulo lógico, avalia o estado de resposta
 * e delega o feedback à camada visual de terminal (TUI). 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "familias.h"
#include "terminal_ui.h"

/**
 * @brief Limpa o buffer de entrada do stdin de forma defensiva para evitar loop infinito em falhas do fgets.
 */
static void limpar_buffer_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Função principal e ponto de entrada (Entrypoint).
 * Mantém o loop interativo da aplicação conforme o contrato de interface.
 */
int main(void) {
    char buffer[256];
    char opcao = 'S';

    ui_print_header("Robô Auxiliar de Matemática (Parte 2)");

    do {
        ui_prompt("Digite um número (ex: 42, -0.5, 3/4, 0.333..., √2): ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n");
            ui_print_error("[Fim de Arquivo / Erro Crítico]. Encerrando.");
            break;
        }

        /* Defesa contra Buffer Overflow: entrada não possuía newline dentro da capacidade máxima */
        if (strchr(buffer, '\n') == NULL && strlen(buffer) == sizeof(buffer) - 1) {
            printf("\n");
            ui_print_error("A entrada excedeu o tamanho máximo permitido do buffer.");
            limpar_buffer_stdin();
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';
        ResultadoClassificacao res = classificar_numero(buffer);

        /* Orquestração dos retornos da matemática para a interface gráfica (TUI) */
        switch (res.status) {
            case ERR_EMPTY_INPUT:
                printf("\n");
                ui_print_warning("Nao foi possivel classificar: a entrada nao pode estar vazia.");
                break;
            case ERR_INVALID_FORMAT:
                printf("\n");
                ui_print_warning("Nao foi possivel classificar: expressao possui sintaxe incorreta ou caracteres nao suportados.");
                break;
            case ERR_DIV_ZERO:
                printf("\n");
                ui_print_error("Nao foi possivel classificar: divisao por zero nao e permitida.");
                break;
            case ERR_OVERFLOW:
                printf("\n");
                ui_print_warning("Nao foi possivel classificar: valor fora do intervalo numérico suportado.");
                break;
            case ERR_NON_REAL_ROOT:
                ui_print_non_real_root(buffer);
                break;
            case ERR_OK:
                ui_print_results(buffer, res);
                break;
            default:
                printf("\n");
                ui_print_error("Erro Desconhecido ocorrido no classificador matemático.");
        }

        ui_prompt("Deseja analisar outro número? (S/N): ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcao = buffer[0];
        } else {
            opcao = 'N';
        }

    } while (opcao == 'S' || opcao == 's');

    printf("\n");
    ui_print_info("Encerrando o Robô Auxiliar de Matemática. Até logo!\n");
    
    return EXIT_SUCCESS;
}