#include <stdio.h>
#include <stdlib.h>

int altura_valida(int altura, int **tabuleiro, int i, int j, int n) {
    //visão pela esquerda:
    int maior_altura = 0, predios_avistados = 0;
    for (int coluna=1; coluna<j; coluna++) { //esse for calcula quantos prédios dá pra ver pela esquerda
        if (tabuleiro[i][coluna] == altura) //também já aproveitamos e verificamos se a altura dada não é repetida
            return 0;
        if (tabuleiro[i][coluna] > maior_altura) {
            maior_altura = tabuleiro[i][coluna];
            predios_avistados++;
        }
    }
    if (n-j+1 == tabuleiro[i][0]-predios_avistados && altura <= maior_altura)
        return 0; //ij não seria visível pela esquerda em um contexto onde ele precisaria ser
    //visão pela direita:
    maior_altura = altura;
    predios_avistados = 1; //inicialmente, as casas do tabuleiro a direita de ij estão vazias, portanto ij é visível
    for (int coluna=j-1; coluna>=1; coluna--) { //calcularemos o número atual de prédios visíveis pela direita
        if (tabuleiro[i][coluna] > maior_altura) {
            maior_altura = tabuleiro[i][coluna];
            predios_avistados++;
        }
    }
    if (n == j && tabuleiro[i][n+1] != predios_avistados) {
        return 0; //estamos na última coluna da linha e não há mais como colocar novos prédios a direita
    }
    else if (n-j == tabuleiro[i][n+1]-predios_avistados && altura-1 < tabuleiro[i][n+1]-predios_avistados) 
        return 0; //todos os prédios a direita de ij devem ser visíveis pela direita, mas não há alturas suficientes para tal
    //visão por cima:
    maior_altura = predios_avistados = 0;
    for (int linha=1; linha<i; linha++) { //esse for calcula quantos prédios dá pra ver por cima
        if (tabuleiro[linha][j] == altura) //também já aproveitamos e vemos se não há alturas repetidas na coluna
            return 0;
        if (tabuleiro[linha][j] > maior_altura) {
            maior_altura = tabuleiro[linha][j];
            predios_avistados++;
        }
    }
    if (n-i+1 == tabuleiro[0][j]-predios_avistados && altura <= maior_altura)
        return 0; //ij não seria visível em um contexto onde precisaria ser
    //visao por baixo:
    maior_altura = altura;
    predios_avistados = 1;
    for (int linha=i-1; linha>=1; linha--) { //calcularemos o número atual de prédios visíveis por baixo
        if (tabuleiro[linha][j] > maior_altura) {
            maior_altura = tabuleiro[linha][j];
            predios_avistados++;
        }
    }
    if (n == i && tabuleiro[n+1][j] != predios_avistados) {
        return 0; //estamos na última linha da coluna e não há mais como colocar novos prédios abaixo
    }
    else if (n-i == tabuleiro[n+1][j]-predios_avistados && altura-1 < tabuleiro[n+1][j]-predios_avistados)
        return 0; //todos os predios abaixo de ij devem ser visíveis por baixo, mas não há alturas suficientes para tal 
    return 1; //a altura é válida
}

void proxima_posicao(int *i, int *j, int n) {
    if (*j == n) { //se estamos na ultima casa de uma linha
        (*i)++; //avancamos uma linha
        *j = 1; //voltamos a primeira coluna
    }
    else (*j)++; //caso contrário, apenas avançamos uma coluna
}

void retornar_posicao(int *i, int *j, int n) {
    if (*j == 1) { //se estamos na primeira casa de um linha
        (*i)--; //voltamos a linha anterior
        *j = n; //e para a ultima casa preenchivel do tabuleiro
    }
    else (*j)--; //caso contrário, apenas voltamos uma coluna
}

int resolver(int **tabuleiro, int linha, int coluna, int n) {
    int i = linha, j = coluna;
    if (linha == n+1) { //chegamos ao final das casas preenchíveis do tabuleiro
        return 1;
    }
    else {
        for (int altura=1; altura<=n; altura++) {
            if (altura_valida(altura, tabuleiro, i, j, n)) { //se a altura for válida
                tabuleiro[i][j] = altura; //a casa do tabuleiro recebe a altura
                proxima_posicao(&i, &j, n); //avançamos uma posição
                if(resolver(tabuleiro, i, j, n)) //se for possível resolver o resto do tabuleiro
                    return 1;
                retornar_posicao(&i, &j, n); //não foi possível resolver o resto, voltamos uma posição e continuamos o for
            }
        }
        return 0; //nenhuma altura é válida, ou não é possível resolver o restante do tabuleiro
    }
}

int main() {
    int n, **tabuleiro;
    scanf("%d", &n);
    tabuleiro = malloc((n+2) * sizeof(int*));
    for(int i=0; i<n+2; i++) {
        tabuleiro[i] = malloc((n+2) * sizeof(int));
        for (int j=0; j<n+2; j++) {
            scanf("%d", &tabuleiro[i][j]);
        }
    }
    resolver(tabuleiro, 1, 1, n);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            printf("%d ", tabuleiro[1+i][1+j]);
        }
        printf("\n");
    }
    for (int i=0; i<n+2; i++)
        free(tabuleiro[i]);
    free(tabuleiro);
    return 0;    
}