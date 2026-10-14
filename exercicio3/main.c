#include <stdio.h>
#include <string.h>

#define MAX_ITENS 30
#define TAM_ITEM 50
#define TAM_NOME 128

// Estrutura para representar o conjunto com itens estaticos e cardinalidade
typedef struct {
    char nome[TAM_NOME];
    char itens[MAX_ITENS][TAM_ITEM];
    int cardinalidade;
} Conjunto;

// Inicializa conjunto vazio (cardinalidade 0)
void inicializar_conjunto(Conjunto *c, const char *nome) {
    strncpy(c->nome, nome, TAM_NOME - 1);
    c->nome[TAM_NOME - 1] = '\0';
    c->cardinalidade = 0;
}

// Verifica pertinencia: retorna 1 se o elemento pertence, 0 se nao pertence
int contem(const Conjunto *c, const char *elemento) {
    for (int i = 0; i < c->cardinalidade; i++) {
        if (strcmp(c->itens[i], elemento) == 0) {
            return 1;
        }
    }
    return 0;
}

// Insere elemento garantindo unicidade (sem itens repetidos)
void adicionar_elemento(Conjunto *c, const char *elemento) {
    if (contem(c, elemento)) {
        return; // Ja existe no conjunto
    }

    if (c->cardinalidade < MAX_ITENS) {
        strncpy(c->itens[c->cardinalidade], elemento, TAM_ITEM - 1);
        c->itens[c->cardinalidade][TAM_ITEM - 1] = '\0';
        c->cardinalidade++;
    }
}

// Imprime elementos formatados: { item1, item2 }
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

// Exibe usuario e itens na secao de entrada
void imprimir_usuario_entrada(const Conjunto *c) {
    printf("- %-5s ", c->nome);
    imprimir_elementos(c);
    printf(" (%d itens)\n", c->cardinalidade);
}

// Intersecao (A ∩ B): elementos que estao em A e tambem em B
void calcular_intersecao(const Conjunto *A, const Conjunto *B, Conjunto *resultado) {
    inicializar_conjunto(resultado, "Intersecao");

    for (int i = 0; i < A->cardinalidade; i++) {
        if (contem(B, A->itens[i])) {
            adicionar_elemento(resultado, A->itens[i]);
        }
    }
}

// Uniao (A ∪ B): junta elementos de A e B sem duplicar
void calcular_uniao(const Conjunto *A, const Conjunto *B, Conjunto *resultado) {
    inicializar_conjunto(resultado, "Uniao");

    for (int i = 0; i < A->cardinalidade; i++) {
        adicionar_elemento(resultado, A->itens[i]);
    }
    for (int i = 0; i < B->cardinalidade; i++) {
        adicionar_elemento(resultado, B->itens[i]);
    }
}

// Calculo do Indice de Jaccard: |A ∩ B| / |A ∪ B|
double calcular_jaccard(int card_intersecao, int card_uniao) {
    if (card_uniao == 0) return 0.0;
    return (double)card_intersecao / (double)card_uniao;
}

// Faz o calculo do par e imprime os resultados
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

    // Entrada
    printf("[Entrada - Interesses]\n");
    imprimir_usuario_entrada(&ana);
    imprimir_usuario_entrada(&bia);
    imprimir_usuario_entrada(&caio);

    printf("\n----------------------------------------------------------\n");
    printf("[Calculos de Afinidade]\n\n");

    // Calculo dos pares
    double jaccard_ana_bia  = analisar_par(1, &ana, &bia);
    double jaccard_ana_caio = analisar_par(2, &ana, &caio);
    double jaccard_bia_caio = analisar_par(3, &bia, &caio);
    (void)jaccard_bia_caio;

    // Decisao de recomendacao
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
