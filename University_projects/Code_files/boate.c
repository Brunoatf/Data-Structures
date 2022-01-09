#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    int n;
    char profissao[26];
    char vacinado[13];
    struct no * prox;
} no;

typedef no * p_no;

p_no inserir(p_no fila, p_no * lanterna, int n, char profissao[26], char vacinado[13]) {
    p_no novo;
    novo = malloc(sizeof(no));
    novo->n = n;
    strcpy(novo->profissao,profissao);
    strcpy(novo->vacinado, vacinado);
    novo->prox = NULL;
    if(strcmp(novo->profissao,"coach") == 0 || strcmp(novo->profissao,"ex-bbb") == 0 || strcmp(novo->profissao,"youtuber-financas") == 0
    || strcmp(novo->profissao,"herdeiro") == 0 || strcmp(novo->profissao,"filho-politico") == 0 || fila == NULL) {
        novo->prox = fila; //a pessoa entra como primeira da fila
        *lanterna = novo; //a lanterna passa para a pessoa nova
        return novo; //ela é o primeiro elemento da fila
    }
    if (strcmp(novo->profissao,"funcionario") == 0) { 
        p_no no;
        for (no = fila; no->prox != NULL; no = no->prox);
        no->prox = novo; //o funcionário entra como última pessoa da fila
        *lanterna = novo; //a lanterna passa para a pessoa nova
        return fila; //porém o primeiro elemento da fila continua o mesmo
    }
    if (strcmp((*lanterna)->profissao,"fiscal") == 0 && strcmp(novo->vacinado,"nao-vacinado") == 0) {
        free(novo);
        return fila; //a pessoa nova não entra na fila
    }
    if (strcmp(novo->profissao, "fiscal") == 0 && (*lanterna)->prox != NULL) {
        p_no no;
        for (no = (*lanterna)->prox; strcmp(no->vacinado,"nao-vacinado") == 0; no = no->prox) {
            free(no);
        }
        novo->prox = no; //a pessoa atrás do fiscal deve ser uma vacinada
    }
    else novo->prox = (*lanterna)->prox; //a pessoa atrás da nova passa a ser aquela que estava atrás da que tinhaahr3ohth a lanterna
    (*lanterna)->prox = novo; //a pessoa nova entra após a pessoa que tinha a lanterna
    *lanterna = novo; //a lanterna passa para a pessoa nova
    return fila; //o primeiro elemento da fila continua o mesmo
}

void destruir_fila(p_no fila) {
    if (fila->prox != NULL) {
        destruir_fila(fila->prox);
    }
    free(fila);
}

void imprimir_fila(p_no fila) {
    while(fila->prox != NULL) {
        printf("%d %s %s\n", fila->n, fila->profissao, fila->vacinado);
        fila = fila->prox;
    }
    printf("%d %s %s\n", fila->n, fila->profissao, fila->vacinado);
}

int main() {
    p_no fila, *lanterna; //fila aponta sempre para o primeiro nó. Lanterna aponta para o p_no da pessoa com a lanterna.
    int n;
    char profissao[26], vacinado[13];
    lanterna = malloc(sizeof(p_no));
    fila = *lanterna = NULL; //a fila inicialmente está vazia, ou seja, fila e lanterna começam com valor nulo 
    while(scanf("%d %s %s", &n, profissao, vacinado)!= EOF) {
      fila = inserir(fila, lanterna, n, profissao, vacinado);
    }
    imprimir_fila(fila);
    destruir_fila(fila);
    free(lanterna);
    return 0;
}   