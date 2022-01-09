#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct no {
    double dinheiro; //dinheiro da pessoa associada ao nó
    char nome[21]; //nome da pessoa
    struct no * esq, * dir, * pai, * prox; //cada pessoa leva um ponteiro para o no esquerdo, direito e pai da árvore
    //temos também um ponteiro "prox" que será usado quando o nó estiver em uma fila, para o percurso em largura da árvore
    int nivel; //indica o nível da árvore que o nó está
} no;

typedef no * p_no;

typedef struct fila{ //essa fila será usada para imprimirmos os elementos da árvore em um percurso em largura
    struct no * ini, * fim;
} fila;

typedef fila * p_fila;

p_no criar_arvore(double dinheiro, char nome[21], p_no pai, p_no esq, p_no dir) { //cria uma árvore de 1 nó
    p_no r = malloc(sizeof(no));
    r->dinheiro = dinheiro;
    strcpy(r->nome, nome);
    r->pai = pai;
    r->esq = esq;
    r->dir = dir;
    if (pai == NULL)
        r->nivel = 1;
    else 
        r->nivel = r->pai->nivel + 1;
    return r;
}

p_no procurar_no(p_no raiz, char nome[21]) { //retorna o no com o nome desejado
    p_no esq;
    if (raiz == NULL || strcmp(raiz->nome, nome) == 0)
        return raiz;
    esq = procurar_no(raiz->esq, nome);
    if (esq != NULL)
        return esq;
    return procurar_no(raiz->dir, nome);
}

void remuneracao(p_no pagador, double taxa) { //faz a remuneracao para os níveis acima do pagador, que inicialmente será
    //a pessoa que conseguiu recrutar 2 novos membros para a pirâmide
    if (taxa > 0 && pagador->pai != NULL) {
        pagador->pai->dinheiro += (pagador->dinheiro)*taxa;
        pagador->dinheiro -= taxa*pagador->dinheiro;
        remuneracao(pagador->pai, taxa-0.01); //a taxa do próximo nível sera 0.01 a menos que a atual
    }
}

p_fila criar_fila() {
    p_fila f;
    f = malloc(sizeof(fila));
    f->ini = NULL;
    f->fim = NULL;
    return f;
}

void enfileirar(p_fila f, p_no novo) {
    if (novo != NULL) {
        novo->prox = NULL;
        if (f->ini == NULL)
            f->ini = novo;
        else
            f->fim->prox = novo;
        f->fim = novo;
    }
}

p_no desenfileirar(p_fila f) {
    p_no primeiro = f->ini;
    f->ini = f->ini->prox;
    if (f->ini == NULL)
        f->fim = NULL;
    return primeiro;
}

void destruir_arvore(p_no raiz){ //faz a desalocacao dos nós utilizados no programa
    if (raiz->esq != NULL) {
        destruir_arvore(raiz->esq);
    }
    if (raiz->dir != NULL) {
        destruir_arvore(raiz->dir);
    }
    free(raiz);
}

void percurso_largura(p_no raiz, p_fila f){
    int nivel = 1; //inicialmente, o percurso começa na raiz
    printf("\n\nNivel 1:");
    enfileirar(f, raiz); //adicionamos a raiz na fila
    while(f->ini != NULL && f->fim != NULL) { //enquanto a fila nao estiver vazia
        raiz = desenfileirar(f); //desenfileramos o primeiro elemento
        if (raiz != NULL) { //se ele for um elemento não nulo
            enfileirar(f, raiz->esq); //enfileiramos os seus nós esquerdo e direito
            enfileirar(f, raiz->dir);
            printf(" [%s %.2lf]", raiz->nome, raiz->dinheiro); //printamos as informações do ex-primeiro elemento
        }
        if (f->ini != NULL && f->ini->nivel > nivel) { //se o novo primeiro elemento for do próximo nível
            nivel++; //assim, os próximos printfs estarão de acordo com o nível dos elementos printados
            printf("\nNivel %d:", nivel);
        }
    }
}

int main() {
    double dinheiro;
    char recrutador[21], recrutado[21];
    p_no raiz, no_recrutador;
    p_fila f = criar_fila();
    scanf("Topo %s %lf", recrutador, &dinheiro); //escaneamos a raiz da pirâmide
    raiz = criar_arvore(dinheiro, recrutador, NULL, NULL, NULL); //criamos uma árvore binária a partir dessa raiz
    printf("Nivel 1: [%s %.2lf]", recrutador, dinheiro); //printamos a árvore no estado inicial
    while(scanf("%s recruta %s %lf", recrutador, recrutado, &dinheiro) != EOF) { //enquanto houver recrutamentos
        no_recrutador = procurar_no(raiz, recrutador); //buscamos o nó do recrutador na árvore
        no_recrutador->dinheiro -= dinheiro*(0.1); //o recrutador perde 10% do capital do recrutado
        if (no_recrutador->esq == NULL) { //se é a primeira recrutação do recrutador
            no_recrutador->esq = criar_arvore(1.1*dinheiro, recrutado, no_recrutador, NULL, NULL);
        } //na linha acima, criamos a subárvore relativa ao recrutado, que ganha uma bonificação de 10% no seu capital
        else { //se é a segunda recrutação do recrutador
            no_recrutador->dir = criar_arvore(1.1*dinheiro, recrutado, no_recrutador, NULL, NULL);
            //as proximas 4 linhas fazem a remuneração dos níveis superiores:
            no_recrutador->dinheiro += 0.06*(no_recrutador->esq->dinheiro + no_recrutador->dir->dinheiro);
            no_recrutador->esq->dinheiro *= 0.94;
            no_recrutador->dir->dinheiro *= 0.94;
            remuneracao(no_recrutador, 0.05);
        }
        percurso_largura(raiz, f); //fazemos a impressão da árvore, que mostrará os efeitos do último recrutamento
    }
    free(f); //liberamos o ponteiro relativo à fila
    destruir_arvore(raiz); //liberamos os ponteiros relativos aos nós da árvore
    return 0;
}