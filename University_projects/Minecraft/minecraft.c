#include <stdio.h>
#include <stdlib.h>
#include "minecraft.h"

int **calcularAltitudes(int m, int n, int seed) {
    int **matriz = malloc(m * sizeof(int*));
    for (int x=0; x<m; x++) {
        matriz[x] = malloc(n * sizeof(int));
        for (int z=0; z<n; z++)
            matriz[x][z] = (seed*(202+x+z) + 12345 + x + z)%256;
    }
    return matriz;
}

Bloco ***criarMundo(int m, int n, int **altitudes, int seed) {
    Bloco ***matriz_blocos = malloc(m * sizeof(Bloco**));
    for (int x=0; x<m; x++) {
        matriz_blocos[x] = malloc(n * sizeof(Bloco*));
        for (int z=0; z<n; z++) {
            matriz_blocos[x][z] = malloc(256 * sizeof(Bloco));
            for (int y=0; y<256; y++) {
                if (y > altitudes[x][z])
                    matriz_blocos[x][z][y].valor = 21;
                else
                    matriz_blocos[x][z][y].valor = (seed*(202+x+y+z)+x+y+z)%33;
            }
        }
    }
    return matriz_blocos;
}

double explorarMundo(Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco,
int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos) {
    for (int x=0; x<m; x++) {
        for (int z=0; z<n; z++) {
            for (int y=0; y<=altitudes[x][z]; y++) {
                if (mundo[x][z][y].valor == 0) (*qtdDiamante)++;
                else if (mundo[x][z][y].valor < 3) (*qtdOuro)++;
                else if (mundo[x][z][y].valor < 6) (*qtdFerro)++;
                if (mundo[x][z][y].valor < 21) (*qtdBlocos)++;
            }
        }
    }
    return *qtdBlocos*tempoPorBloco;
}

int main() {
    int m, n, seed, qtdDiamante=0, qtdOuro=0, qtdFerro=0, qtdBlocos=0;
    double tempoPorBloco, tempoTotal;
    scanf("%d %d %d %lf", &m, &n, &seed, &tempoPorBloco);
    int **altitudes = calcularAltitudes(m, n, seed);
    Bloco ***mundo = criarMundo(m, n, altitudes, seed);
    tempoTotal = explorarMundo(mundo, m, n, altitudes, tempoPorBloco, &qtdDiamante, &qtdOuro, &qtdFerro, &qtdBlocos);
    printf("Total de Blocos: %d\nTempo total: %.2lfs\n", qtdBlocos, tempoTotal);
    printf("Diamantes: %d\nOuros: %d\nFerros: %d\n", qtdDiamante, qtdOuro, qtdFerro);
    for (int x=0; x<m; x++) {
        for (int z=0; z<n; z++) {
            free(mundo[x][z]);
        }
        free(mundo[x]);
        free(altitudes[x]);
    }
    free(altitudes);
    free(mundo);
    return 0;
}