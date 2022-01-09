#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct no {
    char loja[20]; //supus que as lojas terão nomes de no máximo 19 chars, por isso loja comporta 19 chars + \0
    int codigo;
    double valor;
    struct no * esq;
    struct no * dir;
} no;

typedef no * p_no;

enum tem_oferta {TEM, NAOTEM}; //esse enum será usado para verificarmos se alguma oferta foi encontrada na faixa de preço

p_no inserir(p_no raiz, char loja[20], int codigo, double valor) { //insere na árvore de busca
    p_no novo;
    if (raiz == NULL) { //se a árvore for vazia
        novo = malloc(sizeof(no));
        novo->esq = novo->dir = NULL;
        novo->valor = valor;
        novo->codigo = codigo;
        strcpy(novo->loja, loja);
        return novo;
    }
    //o primeiro critério de ordenação na árvore será o código 
    else if (codigo < raiz->codigo)
        raiz->esq = inserir(raiz->esq, loja, codigo, valor);
    else if (codigo > raiz->codigo)
        raiz->dir = inserir(raiz->dir, loja, codigo, valor);
    //o segundo critério de ordenação na árvore será a ordem alfabética da loja
    else if (strcmp(loja,raiz->loja) < 0)
        raiz->esq = inserir(raiz->esq, loja, codigo, valor);
    else
        raiz->dir = inserir(raiz->dir, loja, codigo, valor);
    return raiz;
}

void consulta(p_no raiz, int codigo, double valor, enum tem_oferta *oferta) { //imprime as ofertas encontradas na árvore
    if (raiz != NULL) {
        if (raiz->codigo > codigo)
            consulta(raiz->esq, codigo, valor, oferta);
        else if (raiz->codigo < codigo)
            consulta(raiz->dir, codigo, valor, oferta);
        else { //se a raiz possuir o codigo procurado
            consulta(raiz->esq, codigo, valor, oferta); //primeiro, consultamos à esquerda da raiz
            if (raiz->valor <= 1.1*valor && raiz->codigo == codigo) { //depois, analisamos a raiz
                printf("%s %d %.2lf\n", raiz->loja, raiz->codigo, raiz->valor);
                *oferta = TEM; //assinalamos que pelo menos 1 oferta foi encontrada
            }
            consulta(raiz->dir, codigo, valor, oferta);//e, por fim, à direita da raiz
            //Essa ordem faz com que passemos pelos nós em ordem alfabética das lojas
        }
    }
}

void destruir_arvore(p_no raiz) {
    if (raiz != NULL) {
        destruir_arvore(raiz->esq);
        destruir_arvore(raiz->dir);
        free(raiz);
    }
}

int main() {
    int n, m, codigo;
    double valor;
    char loja[20];
    p_no raiz = NULL;
    enum tem_oferta oferta;
    scanf("%d", &n);
    for (int i=0; i<n; i++) {
        scanf("%s %d %lf", loja, &codigo, &valor);
        raiz = inserir(raiz, loja, codigo, valor);
    }
    scanf("%d", &m);
    for (int i=0; i<m; i++) {
        oferta = NAOTEM; //para cada consulta, inicialmente supomos que não há ofertas na faixa de preço desejada
        scanf("%d %lf", &codigo, &valor);
        printf("OFERTAS para %d:\n", codigo);
        consulta(raiz, codigo, valor, &oferta);
        if (oferta == NAOTEM) //se, após a consulta, oferta ainda tiver o valor de NAOTEM
            printf("nenhuma boa oferta foi encontrada\n");
        if (i != m-1) //esse if faz com que não pulemos uma linha a mais no final do código
            printf("\n");
    }
    destruir_arvore(raiz);
    return 0;
}