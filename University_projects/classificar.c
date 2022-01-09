#include <stdio.h>
#include <string.h>
#define NMAX 100

typedef struct pessoa {
    char nomecompleto[30];
    char nome[28];
} pessoa;

void ordenar_nome_lexicograficamente(pessoa* pessoas, int num_de_linhas) {
    //tal funcao age como uma espécie de bubble sort, deixando pessoas em ordem lexicografica
    pessoa aux;
    for (int i=num_de_linhas; i>0; i--) {
        for (int j=0; j<i-1; j++) {
            if (strcmp(pessoas[j].nomecompleto, pessoas[j+1].nomecompleto) > 0) {
                aux = pessoas[j];
                pessoas[j] = pessoas[j+1];
                pessoas[j+1] = aux;
            }
        }
    }
}

void homonimos(int n) {
    pessoa pessoas[NMAX];
    for (int i=0; i<n; i++) { //para cada pessoa
        scanf("%s", pessoas[i].nomecompleto); //pegamos o nome completo
        for (int j=0; j<30; j++) { //para cada caractere do nome completo
            if (pessoas[i].nomecompleto[j] == '_'){ //se o caractere for _
                pessoas[i].nome[j] = '\000'; //não há mais caracteres a se colocar no nome
                break;
            }
            else pessoas[i].nome[j] = pessoas[i].nomecompleto[j];
        }
    }
    ordenar_nome_lexicograficamente(pessoas, n); //ordenamos o vetor pessoas
    for (int i=0; i<n; i++) { //cada nome
        for (int j=0; j<n; j++) { //será comparado com os demais nomes
            if (j != i && strcmp(pessoas[i].nome, pessoas[j].nome) == 0) { //se o nome for igual a um outro nome diferente de si
                printf("%s\n", pessoas[i].nomecompleto);
                break;
            }
            else if (pessoas[i].nome[0] < pessoas[j].nome[0]) {//se nome[j] estiver entre os nomes que vem após nome[i]
                break; //quebra-se o for, pois não será possível encontrar um homonimo de nome[i] nos próximos nome[j]
            }
        }
    }
}

int main() {
    int n;
    char palavra[10];
    scanf("%d %s", &n, palavra);
    if (palavra[0] == 'h') homonimos(n); //chamamos a função homonimos
    //poderiamos também implementar uma funcao para printar parentes, chamando-a em um else aqui
    return 0;
}