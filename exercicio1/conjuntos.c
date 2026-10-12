#include "conjuntos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria uma copia dinamica (heap) de uma string fornecida.
 *        Solucao ISO C pura para substituir a dependencia da funcao POSIX strdup().
 * @param src Ponteiro constante contendo a string de origem.
 * @return Retorna o ponteiro para a nova string alocada, ou NULL em falha de memoria.
 */
static char* duplicate_string(const char *src) {
    if (!src) return NULL;
    size_t len = strlen(src);
    char *dst = (char*) malloc(len + 1);
    if (!dst) return NULL;
    memcpy(dst, src, len + 1);
    return dst;
}

Set* set_create(size_t capacity) {
    Set *s = (Set*) malloc(sizeof(Set));
    if (!s) return NULL;
    
    s->capacity = capacity;
    s->size = 0;
    
    if (capacity > 0) {
        s->elements = (char**) malloc(capacity * sizeof(char*));
        if (!s->elements) {
            free(s);
            return NULL;
        }
    } else {
        s->elements = NULL;
    }
    
    return s;
}

void set_free(Set *s) {
    if (s) {
        if (s->elements) {
            for (size_t i = 0; i < s->size; i++) {
                free(s->elements[i]);
                s->elements[i] = NULL;
            }
            free(s->elements);
            s->elements = NULL;
        }
        free(s);
    }
}

bool set_contains(const Set *s, const char *value) {
    if (!s || s->size == 0 || !value) return false;
    for (size_t i = 0; i < s->size; i++) {
        if (strcmp(s->elements[i], value) == 0) return true;
    }
    return false;
}

bool set_add(Set *s, const char *value) {
    if (!s || !value) return false;
    if (set_contains(s, value)) return false; /* Impede duplicatas num mesmo conjunto */
    
    if (s->size >= s->capacity) {
        size_t new_cap = (s->capacity == 0) ? 4 : s->capacity * 2;
        char **new_elems = (char**) realloc(s->elements, new_cap * sizeof(char*));
        if (!new_elems) return false;
        s->elements = new_elems;
        s->capacity = new_cap;
    }
    
    char *new_str = duplicate_string(value);
    if (!new_str) return false;
    
    s->elements[s->size++] = new_str;
    return true;
}

Set* set_union(const Set *A, const Set *B) {
    Set *result = set_create(A->size + B->size);
    if (!result) return NULL;
    
    for (size_t i = 0; i < A->size; i++) {
        set_add(result, A->elements[i]);
    }
    for (size_t i = 0; i < B->size; i++) {
        if (!set_contains(result, B->elements[i])) {
            set_add(result, B->elements[i]);
        }
    }
    return result;
}

Set* set_intersection(const Set *A, const Set *B) {
    size_t min_size = (A->size < B->size) ? A->size : B->size;
    Set *result = set_create(min_size);
    if (!result) return NULL;
    
    for (size_t i = 0; i < A->size; i++) {
        if (set_contains(B, A->elements[i])) {
            set_add(result, A->elements[i]);
        }
    }
    return result;
}

Set* set_difference(const Set *A, const Set *B) {
    Set *result = set_create(A->size);
    if (!result) return NULL;
    
    for (size_t i = 0; i < A->size; i++) {
        if (!set_contains(B, A->elements[i])) {
            set_add(result, A->elements[i]);
        }
    }
    return result;
}

bool set_is_subset(const Set *A, const Set *B) {
    if (A->size == 0) return true;
    for (size_t i = 0; i < A->size; i++) {
        if (!set_contains(B, A->elements[i])) {
            return false;
        }
    }
    return true;
}

size_t set_cardinality(const Set *s) {
    if (!s) return 0;
    return s->size;
}

void set_print(const Set *s) {
    if (!s || s->size == 0) {
        printf("{}\n");
        return;
    }
    printf("{");
    for (size_t i = 0; i < s->size; i++) {
        printf("\"%s\"", s->elements[i]);
        if (i < s->size - 1) printf(", ");
    }
    printf("}\n");
}