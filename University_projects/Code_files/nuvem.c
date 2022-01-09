#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct { //as informações de cada palavra serão armazenadas em "nós" contendo a string e a sua frequência
    char chave[50];
    int frequencia;
} no;

typedef no * p_no;

int hashing_function(char palavra[50], int tamanho_hashtable) { //calcula o hashing da string
    int n = 0;
    for (int i=0; i < strlen(palavra); i++) { 

        //multiplicamos o numero ascii de cada caracter por uma potencia de 256, somando os resultados e obtendo um
        //número em base 256. O resto de tal número dividido pelo tamanho da hashtable será o resultado do hash. A
        //expressão abaixo faz tal cálculo de maneira eficiente: 

        n = (256 * n + palavra[i]) % tamanho_hashtable;

    }
    return n;
}

void inserir_hash(p_no hash, int estimativa, char palavra[50]) {
    int indice = hashing_function(palavra, estimativa);
    //A tabela de hashing será feita com endereçamento aberto. A linha abaixo trata do caso de colisões nesse contexto:
    while (strcmp(hash[indice].chave,"\0") != 0 && strcmp(hash[indice].chave, palavra) != 0) {
        indice = (indice + 1) % estimativa;
    }
    //Se encontramos uma chave igual a desejada:
    if (strcmp(hash[indice].chave,"\0") != 0 && strcmp(hash[indice].chave, palavra) == 0) {
        hash[indice].frequencia++;
    }
    //Caso contrário, adicionamos a chave na tabela de hashing:
    else {
        strcpy(hash[indice].chave, palavra);
        hash[indice].frequencia = 1;
    }
}

//A função a seguir verifica se a palavra dada é uma stop word:
int verificar(char palavra[50], char ** stop_words, int numero_stop_words) {
    for (int i = 0; i < numero_stop_words; i++) {
        if (strcmp(palavra, stop_words[i]) == 0)
            return 1;
    }
    return 0;
}

void troca(p_no no1, p_no no2) { //faz a troca do conteúdo de 2 nos
    no aux;
    strcpy(aux.chave, no1->chave);
    aux.frequencia = no1->frequencia;
    strcpy(no1->chave, no2->chave);
    no1->frequencia = no2->frequencia;
    strcpy(no2->chave, aux.chave);
    no2->frequencia = aux.frequencia;
}

//A seguinte função particiona um vetor ao redor de determinado pivô:
int partition(p_no vetor, int esquerda, int direita) {
    int pos = direita+1;
    no pivo = vetor[esquerda];
    for (int i = direita; i >= esquerda; i--) {

        //A próxima condicional será true se vetor[i] dever ficar após o pivô. Isso acontece caso vetor[i] seja um no nulo,
        //se seja um nó de frequência menor que a do pivô ou se seja um elemento de mesma frequência que o pivô, mas
        //lexicograficamente maior.

        if (strcmp(vetor[i].chave, "\0") == 0 || vetor[i].frequencia < pivo.frequencia
        || (strcmp(pivo.chave, "\0") != 0 && vetor[i].frequencia == pivo.frequencia
        && strcmp(vetor[i].chave, pivo.chave) >= 0)) {
            pos--;
            if (pos != i) //caso a condicional acima seja verdadeira para todas as iterações, teremos pos == i na última
            //e isso geraria um erro na strcpy usada na função troca
            troca(&vetor[i], &vetor[pos]);
        }
    }
    return pos;
}

void quicksort(p_no vetor, int esquerda, int direita) {
    //Ordenaremos o vetor de modo que os nós vazios fiquem ao final, ao passo que os primeiros nos sejam aqueles com maior
    //frequência, ou, para frequências iguais, menores lexicograficamente.
    int i = 0;
    if (esquerda >= direita) return; //o caso base do quicksort é quando o vetor fica de tamanho nulo
    i = partition(vetor, esquerda, direita);
    //outro caso base é se tivermos um vetor apenas de nos vazios (que já está "ordenado"):
    if (i == esquerda && strcmp(vetor[i].chave, "\0") == 0) return;
    quicksort(vetor, esquerda, i-1);
    quicksort(vetor, i+1, direita);
}

void formatarpalavra(char palavra[50]) { //remove caracteres não alfanuméricos e deixa a palavra dada minúscula
    char formatada[50];
    int j = 0;
    for (int i=0; i<strlen(palavra); i++) { //para cada caracter da string
        if (isalpha(palavra[i])) { //se for alfabético
            formatada[j] = tolower(palavra[i]); //deixamos tal caracter minúsculo e adicionamos em formatada
            j++;
        }
    }
    formatada[j] = '\0';
    strcpy(palavra, formatada);
}

int main() {
    int estimativa, numero_stop_words;
    char palavra[50], ** stop_words;
    scanf("%d", &estimativa);
    p_no hash = malloc(estimativa * sizeof(no));
    for (int i=0; i<estimativa; i++) {
        hash[i].chave[0] = '\0';
        hash[i].frequencia = 0;
    }
    scanf("%d", &numero_stop_words);
    stop_words = malloc(numero_stop_words * sizeof(char*));
    for (int i=0; i<numero_stop_words; i++) { //para cada stopword:
        stop_words[i] = malloc(sizeof(palavra));
        scanf("%s", stop_words[i]);
    }
    while (scanf("%s", palavra) != EOF) {
        //A seguir, garantiremos que a palavra nada fique minúscula e não contenha caracteres especiais:
        formatarpalavra(palavra);
        //A próxima condicional verifica se a palavra tem mais de uma letra e não é stop word
        if (strlen(palavra) != 1 && !verificar(palavra, stop_words, numero_stop_words)) {
            inserir_hash(hash, estimativa, palavra); //inserimos na hash table
        }
    }
    quicksort(hash, 0, estimativa-1); //a hash table foi feita sem encadeamento. Logo, é um vetor passível de ordenação
    for (int i=0; i<50 && strcmp(hash[i].chave, "\0") != 0; i++) { //iteramos sobre as 50 chaves mais frequentes
        printf("%s %d\n", hash[i].chave, hash[i].frequencia);
    }
    for (int i=0; i<numero_stop_words; i++) {
        free(stop_words[i]);
    }
    free(stop_words);
    free(hash);
    return 0;
}