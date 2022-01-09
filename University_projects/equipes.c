#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int altura; //altura da pessoa
    struct no * prox; //proximo elemento da fila
    struct no * anterior; //elemeto anterior da fila
} no;

typedef no * p_no;

p_no inserir(int altura, p_no primeiro_amigos) { //insere uma pessoa no final do circulo de amigos
    p_no novo;
    novo = malloc(sizeof(no));
    novo->altura = altura;    
    if (primeiro_amigos == NULL) { //se a fila de amigos não tiver nenhuma pessoa
        novo->prox = novo;
        novo->anterior = novo;
        return novo; 
    }
    novo->anterior = primeiro_amigos->anterior; // a pessoa anterior à nova será a antiga última da fila
    primeiro_amigos->anterior->prox = novo; // as linhas 22-24 concretizam o dito no último comentário
    novo->prox = primeiro_amigos;
    primeiro_amigos->anterior = novo;
    return primeiro_amigos; //retornamos a primeira, pois ela continua sendo a primeira
}

p_no sortear(int dado, p_no * primeiro_amigos, char sentido) { //retorna a pessoa sorteada
    p_no sorteado;
    sorteado = *primeiro_amigos; //de início supomos que o sorteado é o com o dado
    if (sentido == 'e') { //sorteamos para a esquerda no círculo de amigos
        for (int i=0; i<dado-1; sorteado = sorteado->anterior, i++);
    }
    else { //sorteamos para a direita no círculo de amigos
        for (int i=0; i<dado-1; sorteado = sorteado->prox, i++);
    }
    sorteado->anterior->prox = sorteado->prox; //as linhas 37 e 38 "cortam" a pessoa sorteada do círculo de amigos
    sorteado->prox->anterior = sorteado->anterior;
    if (sorteado == *primeiro_amigos) { //se o sorteado é o primeiro da fila
        *primeiro_amigos = (*primeiro_amigos)->prox; //o primeiro passa a ser a pessoa à sua direita
    }
    return sorteado;
}

p_no inserir_ordenado(p_no novo, p_no primeiro) {
    if (primeiro == NULL) { //se a lista estiver vazia, ou seja, o primeiro elemento não estiver definido
        novo->prox = novo;
        novo->anterior = novo;
        return novo; //tomamos a nova pessoa como primeiro elemento da lista
    }
    novo->prox = primeiro; //inicialmente consideramos que a altura da nova pessoa é a menor
    while (novo->altura > novo->prox->altura) { //enquanto a altura da nova pessoa for maior que a da próxima
        if (novo->prox->prox == primeiro) { //se o próximo elemento for o último da fila circular
            novo->anterior = novo->prox; //as linhas 54-57 colocam o elemento novo como o último da fila circular
            novo->anterior->prox = novo;
            novo->prox = primeiro;
            primeiro->anterior = novo;
            return primeiro;
        } 
        novo->prox = novo->prox->prox; //a próxima passa a ser a próxima da próxima
    }
    novo->anterior = novo->prox->anterior; //o elemento incialmente anterior a novo->prox se torna o anterior do novo
    novo->anterior->prox = novo; //novo é o elemento posterior a novo->anterior
    novo->prox->anterior = novo; //o anterior de novo->prox é, evidentemente, o novo
    if (novo->prox == primeiro) return novo; //se o elemento novo for o novo primeiro elemento da fila
    return primeiro;
}

void saida(p_no elemento) { //imprime a saída relativa à fila circular duplamente ligada na qual "elemento" está 
    p_no primeiro_elemento;
    primeiro_elemento = elemento;
    while (elemento->prox != primeiro_elemento) {
        printf("%d ", elemento->altura);
        elemento = elemento->prox;
        free(elemento->anterior);
    }
    printf("%d\n", elemento->altura);
    free(elemento);
}

int main() {
    int m, altura, dado;
    p_no primeiro_amigos = NULL, primeiro_time_a = NULL, primeiro_time_b = NULL, sorteado = NULL;
    scanf("%d", &m);
    for (int i=0; i<m; i++) {
        scanf("%d", &altura);
        primeiro_amigos = inserir(altura, primeiro_amigos);
    }
    for (int i=0; i<m; i++) {
        scanf("%d", &dado);
        if (i % 2 == 0) {
            sorteado = sortear(dado, &primeiro_amigos, 'e'); //'e' indica que estamos sorteando para a esquerda
            primeiro_time_a = inserir_ordenado(sorteado, primeiro_time_a);
        }
        else {
            sorteado = sortear(dado, &primeiro_amigos, 'd'); //'d' indica que estamos sorteando para a direita
            primeiro_time_b = inserir_ordenado(sorteado, primeiro_time_b);
        }
    }
    saida(primeiro_time_a); //nota-se que a função saida já faz o free da memória alocada
    saida(primeiro_time_b);
}