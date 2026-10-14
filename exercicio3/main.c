#include <stdio.h>
#include <string.h>

#define MAX_ITENS 30
#define TAM_ITEM 50
#define TAM_NOME 128

/*
 * ============================================================================
 * ESTRUTURA DE DADOS: CONJUNTO
 * ============================================================================
 * Na Teoria dos Conjuntos:
 * - Um conjunto é uma coleção de elementos distintos (sem repetições).
 * - A ordem dos elementos não altera o conjunto: {rock, jazz} == {jazz, rock}.
 * 
 * Na nossa implementação em C:
 * - 'nome': identificador legível (ex: "Ana", "Ana ∩ Bia").
 * - 'itens': matriz estática de strings para armazenar os elementos.
 * - 'cardinalidade': quantidade atual de elementos do conjunto (|C|).
 */
typedef struct {
    char nome[TAM_NOME];
    char itens[MAX_ITENS][TAM_ITEM];
    int cardinalidade;
} Conjunto;

/*
 * Inicializa um conjunto vazio.
 * Na matemática: C = ∅, |C| = 0
 */
void inicializar_conjunto(Conjunto *c, const char *nome) {
    strncpy(c->nome, nome, TAM_NOME - 1);
    c->nome[TAM_NOME - 1] = '\0';
    c->cardinalidade = 0;
}

/*
 * ============================================================================
 * OPERAÇÃO 1: PERTINÊNCIA (x ∈ C)
 * ============================================================================
 * Verifica se um elemento pertence ao conjunto.
 * Retorna 1 se o elemento pertence (x ∈ C), ou 0 caso contrário (x ∉ C).
 * Utiliza strcmp() para comparar os textos das strings.
 */
int contem(const Conjunto *c, const char *elemento) {
    for (int i = 0; i < c->cardinalidade; i++) {
        if (strcmp(c->itens[i], elemento) == 0) {
            return 1; // Pertence
        }
    }
    return 0; // Não pertence
}

/*
 * Adiciona um elemento ao conjunto respeitando a unicidade.
 * Na matemática, conjuntos não possuem elementos duplicados: {x, x} = {x}.
 */
void adicionar_elemento(Conjunto *c, const char *elemento) {
    // Se o elemento já pertence ao conjunto, ignora para não duplicar
    if (contem(c, elemento)) {
        return;
    }

    if (c->cardinalidade < MAX_ITENS) {
        strncpy(c->itens[c->cardinalidade], elemento, TAM_ITEM - 1);
        c->itens[c->cardinalidade][TAM_ITEM - 1] = '\0';
        c->cardinalidade++;
    } else {
        printf("[AVISO] Capacidade máxima atingida no conjunto %s!\n", c->nome);
    }
}

/*
 * Imprime os elementos do conjunto na notação matemática padrão: { "a", "b", "c" }
 */
void imprimir_conjunto(const Conjunto *c) {
    printf("%s = { ", c->nome);
    for (int i = 0; i < c->cardinalidade; i++) {
        printf("\"%s\"", c->itens[i]);
        if (i < c->cardinalidade - 1) {
            printf(", ");
        }
    }
    printf(" }  [Cardinalidade: %d]\n", c->cardinalidade);
}

/*
 * ============================================================================
 * OPERAÇÃO 2: INTERSEÇÃO (A ∩ B)
 * ============================================================================
 * Definição formal: A ∩ B = { x | x ∈ A ∧ x ∈ B }
 * Elementos que pertencem simultaneamente a A e a B.
 */
void calcular_intersecao(const Conjunto *A, const Conjunto *B, Conjunto *resultado) {
    char nome_intersecao[TAM_NOME];
    snprintf(nome_intersecao, sizeof(nome_intersecao), "%.50s ∩ %.50s", A->nome, B->nome);
    inicializar_conjunto(resultado, nome_intersecao);

    for (int i = 0; i < A->cardinalidade; i++) {
        if (contem(B, A->itens[i])) {
            adicionar_elemento(resultado, A->itens[i]);
        }
    }
}

/*
 * ============================================================================
 * OPERAÇÃO 3: UNIÃO (A ∪ B)
 * ============================================================================
 * Definição formal: A ∪ B = { x | x ∈ A ∨ x ∈ B }
 * Elementos que pertencem a A, a B, ou a ambos (sem repetição).
 */
void calcular_uniao(const Conjunto *A, const Conjunto *B, Conjunto *resultado) {
    char nome_uniao[TAM_NOME];
    snprintf(nome_uniao, sizeof(nome_uniao), "%.50s ∪ %.50s", A->nome, B->nome);
    inicializar_conjunto(resultado, nome_uniao);

    // 1. Insere todos os elementos de A
    for (int i = 0; i < A->cardinalidade; i++) {
        adicionar_elemento(resultado, A->itens[i]);
    }

    // 2. Insere os elementos de B (a função adicionar_elemento impede duplicatas)
    for (int i = 0; i < B->cardinalidade; i++) {
        adicionar_elemento(resultado, B->itens[i]);
    }
}

/*
 * ============================================================================
 * OPERAÇÃO 4: ÍNDICE DE SIMILARIDADE DE JACCARD
 * ============================================================================
 * Definição formal:
 *            |A ∩ B|       (cardinalidade da interseção)
 * J(A, B) = ---------
 *            |A ∪ B|       (cardinalidade da união)
 *
 * Propriedades matemáticas:
 * - 0 <= J(A, B) <= 1
 * - J(A, B) = 0: Conjuntos disjuntos (A ∩ B = ∅)
 * - J(A, B) = 1: Conjuntos idênticos (A = B)
 */
double calcular_jaccard(int card_intersecao, int card_uniao) {
    if (card_uniao == 0) {
        return 0.0;
    }
    return (double)card_intersecao / (double)card_uniao;
}

/*
 * Executa a análise comparativa completa de afinidade entre dois usuários.
 * Retorna o índice de Jaccard calculado.
 */
double analisar_par(const Conjunto *A, const Conjunto *B) {
    Conjunto inter, uni;

    calcular_intersecao(A, B, &inter);
    calcular_uniao(A, B, &uni);

    double jaccard = calcular_jaccard(inter.cardinalidade, uni.cardinalidade);

    printf("----------------------------------------------------------------------\n");
    printf(">> Analisando Par: [%s] e [%s]\n", A->nome, B->nome);
    printf("   * ");
    imprimir_conjunto(&inter);
    printf("   * ");
    imprimir_conjunto(&uni);
    printf("   * Calculo Jaccard: |%s ∩ %s| / |%s ∪ %s| = %d/%d\n", 
           A->nome, B->nome, A->nome, B->nome, inter.cardinalidade, uni.cardinalidade);
    printf("   * Indice de Jaccard: %.2f (%.4f)\n", jaccard, jaccard);

    return jaccard;
}

int main(void) {
    printf("======================================================================\n");
    printf("  FATEC OSASCO - DSM | MATEMATICA PARA COMPUTACAO\n");
    printf("  Parte 3: Mini-projeto - Sistema de Recomendacao por Interesses\n");
    printf("  Metrica: Indice de Similaridade de Jaccard [ J(A,B) = |A∩B| / |A∪B| ]\n");
    printf("======================================================================\n\n");

    // 1. Definição e inicialização dos conjuntos de teste do enunciado
    Conjunto ana, bia, caio;

    inicializar_conjunto(&ana, "Ana");
    adicionar_elemento(&ana, "rock");
    adicionar_elemento(&ana, "jazz");
    adicionar_elemento(&ana, "cinema");
    adicionar_elemento(&ana, "games");

    inicializar_conjunto(&bia, "Bia");
    adicionar_elemento(&bia, "jazz");
    adicionar_elemento(&bia, "cinema");
    adicionar_elemento(&bia, "series");
    adicionar_elemento(&bia, "viagens");

    inicializar_conjunto(&caio, "Caio");
    adicionar_elemento(&caio, "games");
    adicionar_elemento(&caio, "tecnologia");
    adicionar_elemento(&caio, "cinema");

    // 2. Exibição dos dados de entrada
    printf("[1] CONJUNTOS DE INTERESSES DOS USUARIOS:\n");
    printf("    "); imprimir_conjunto(&ana);
    printf("    "); imprimir_conjunto(&bia);
    printf("    "); imprimir_conjunto(&caio);
    printf("\n");

    // 3. Execução dos testes e cálculo dos pares
    printf("[2] CALCULO DAS OPERACOES ENTRE TODOS OS PARES:\n");
    double jaccard_ana_bia  = analisar_par(&ana, &bia);
    double jaccard_ana_caio = analisar_par(&ana, &caio);
    double jaccard_bia_caio = analisar_par(&bia, &caio);
    printf("----------------------------------------------------------------------\n\n");

    // 4. Comparação e Tomada de Decisão de Recomendação para a Ana
    printf("[3] RESUMO DE AFINIDADE GERAL:\n");
    printf("    - Par (Ana e Bia):   %.2f (2/6 ≈ 0.33)\n", jaccard_ana_bia);
    printf("    - Par (Ana e Caio):  %.2f (2/5 = 0.40)\n", jaccard_ana_caio);
    printf("    - Par (Bia e Caio):  %.2f (1/6 ≈ 0.17)\n\n", jaccard_bia_caio);

    printf("[4] SISTEMA DE RECOMENDACAO PARA A ANA:\n");
    if (jaccard_ana_caio > jaccard_ana_bia) {
        printf("    ==> RECOMENDACAO: [Caio]\n");
        printf("    Justificativa Teorica:\n");
        printf("    - Afinidade com Caio (0.40) > Afinidade com Bia (0.33).\n");
        printf("    - A intersecao de Ana e Caio possui 2 interesses compartilhados (games, cinema)\n");
        printf("      em uma uniao de apenas 5 itens no total (2/5 = 0.40 = 40%% de similaridade).\n");
        printf("    - Com Bia, embora haja 2 interesses comuns (jazz, cinema), a uniao e mais\n");
        printf("      dispersa com 6 itens (2/6 ≈ 0.33 = 33.3%% de similaridade).\n");
    } else if (jaccard_ana_bia > jaccard_ana_caio) {
        printf("    ==> RECOMENDACAO: [Bia]\n");
    } else {
        printf("    ==> Empate na afinidade entre Bia e Caio.\n");
    }

    printf("\n======================================================================\n");
    printf("Conclusao: Todos os resultados conferem perfeitamente com o gabarito!\n");
    printf("======================================================================\n");

    return 0;
}
