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
 * - 'nome': identificador legível (ex: "Ana", "Bia").
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
 * Imprime apenas os elementos formatados entre chaves: { item1, item2, item3 }
 */
void imprimir_elementos(const Conjunto *c) {
    printf("{ ");
    for (int i = 0; i < c->cardinalidade; i++) {
        printf("%s", c->itens[i]);
        if (i < c->cardinalidade - 1) {
            printf(", ");
        }
    }
    printf(" }");
}

/*
 * Imprime um usuário e seu conjunto na seção de Entrada
 * Formato: - Nome:  { item1, item2 } (N itens)
 */
void imprimir_usuario_entrada(const Conjunto *c) {
    printf("- %-5s { ", c->nome);
    for (int i = 0; i < c->cardinalidade; i++) {
        printf("%s", c->itens[i]);
        if (i < c->cardinalidade - 1) {
            printf(", ");
        }
    }
    printf(" } (%d itens)\n", c->cardinalidade);
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

    // 2. Insere os elementos de B (adicionar_elemento impede repetições)
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
 */
double calcular_jaccard(int card_intersecao, int card_uniao) {
    if (card_uniao == 0) {
        return 0.0;
    }
    return (double)card_intersecao / (double)card_uniao;
}

/*
 * Executa o cálculo de afinidade entre dois usuários e imprime no formato exato solicitado.
 */
double analisar_par(int numero_par, const Conjunto *A, const Conjunto *B) {
    Conjunto inter, uni;

    calcular_intersecao(A, B, &inter);
    calcular_uniao(A, B, &uni);

    double jaccard = calcular_jaccard(inter.cardinalidade, uni.cardinalidade);

    printf("Par %d: %s e %s\n", numero_par, A->nome, B->nome);
    printf("  Intersecao: ");
    imprimir_elementos(&inter);
    printf(" -> |A ∩ B| = %d\n", inter.cardinalidade);

    printf("  Uniao:      ");
    imprimir_elementos(&uni);
    printf(" -> |A ∪ B| = %d\n", uni.cardinalidade);

    printf("  Jaccard:    %d / %d = %.2f\n\n", inter.cardinalidade, uni.cardinalidade, jaccard);

    return jaccard;
}

int main(void) {
    printf("=== EXERCICIO 3: RECOMENDACAO POR INTERESSES (JACCARD) ===\n\n");

    // 1. Definição e inicialização dos conjuntos de teste do enunciado
    Conjunto ana, bia, caio;

    inicializar_conjunto(&ana, "Ana:");
    adicionar_elemento(&ana, "rock");
    adicionar_elemento(&ana, "jazz");
    adicionar_elemento(&ana, "cinema");
    adicionar_elemento(&ana, "games");

    inicializar_conjunto(&bia, "Bia:");
    adicionar_elemento(&bia, "jazz");
    adicionar_elemento(&bia, "cinema");
    adicionar_elemento(&bia, "series");
    adicionar_elemento(&bia, "viagens");

    inicializar_conjunto(&caio, "Caio:");
    adicionar_elemento(&caio, "games");
    adicionar_elemento(&caio, "tecnologia");
    adicionar_elemento(&caio, "cinema");

    // 2. Exibição das entradas
    printf("[Entrada - Interesses]\n");
    imprimir_usuario_entrada(&ana);
    imprimir_usuario_entrada(&bia);
    imprimir_usuario_entrada(&caio);

    // Ajusta nomes para a exibição dos cálculos dos pares
    strcpy(ana.nome, "Ana");
    strcpy(bia.nome, "Bia");
    strcpy(caio.nome, "Caio");

    printf("\n----------------------------------------------------------\n");
    printf("[Calculos de Afinidade]\n\n");

    // 3. Cálculos de cada par
    double jaccard_ana_bia  = analisar_par(1, &ana, &bia);
    double jaccard_ana_caio = analisar_par(2, &ana, &caio);
    double jaccard_bia_caio = analisar_par(3, &bia, &caio);
    (void)jaccard_bia_caio; // Uso implícito pelo enunciado

    // 4. Comparação e Resultado
    printf("----------------------------------------------------------\n");
    printf("[Resultado]\n");

    if (jaccard_ana_caio >= jaccard_ana_bia) {
        printf("Maior afinidade com a Ana: Caio (Indice: %.2f)\n", jaccard_ana_caio);
        printf("Recomendacao: Caio\n");
    } else {
        printf("Maior afinidade com a Ana: Bia (Indice: %.2f)\n", jaccard_ana_bia);
        printf("Recomendacao: Bia\n");
    }

    return 0;
}
