#include <stdio.h>
#include <stdlib.h>

double calcular_fonte(double rendimento) {
    if (rendimento <= 1499.15) return 0;
    else if (rendimento <= 2246.75) return 0.075*(rendimento-1499.15);
    else if (rendimento <= 2995.70) return 0.15*(rendimento-2246.75)+0.075*(2246.75-1499.15);
    else if (rendimento <= 3743.19) return 0.225*(rendimento-2995.7)+0.15*(2995.7-2246.75)+0.075*(2246.75-1499.15);
    else return 0.275*(rendimento-3743.19)+0.225*(3743.19-2995.7)+0.15*(2995.7-2246.75)+0.075*(2246.75-1499.15);
}

void calcular_ajuste_anual(double abatimento, double rendimento_anual_abatido, double **matriz, int i) {
    double imposto_anual = 0;
    if (rendimento_anual_abatido <= 18000) imposto_anual += 0;
    else if (rendimento_anual_abatido <= 26400) imposto_anual += 0.075*(rendimento_anual_abatido-18000); 
    else if (rendimento_anual_abatido <= 36000) imposto_anual += 0.15*(rendimento_anual_abatido-26400)+0.075*(26400-18000);
    else if (rendimento_anual_abatido <= 44400) imposto_anual += 0.225*(rendimento_anual_abatido-36000)+0.15*(36000-26400)+0.075*(26400-18000);
    else imposto_anual += 0.275*(rendimento_anual_abatido-44400)+0.225*(44400-36000)+0.15*(36000-26400)+0.075*(26400-18000);
    matriz[i][13] = imposto_anual;
    matriz[i][14] = imposto_anual - matriz[i][12];
}

int main() { //dúvida: precisa de & no scanf para um elemento de vetor ou matriz? v[i] = *(v+i)
    int n;
    double **matriz, abatimento, *rendimento_anual, rendimento, retido_anual; 
    scanf("%d", &n);
    matriz = (double **) malloc(n * sizeof(double *));
    rendimento_anual = (double *) malloc(n * sizeof(double));
    for (int i=0; i<n; i++) {
        retido_anual = 0;
        matriz[i] = (double *) malloc(15 * sizeof(double));
        for (int j=0; j<12; j++) {
            scanf("%lf", &rendimento);
            rendimento_anual[i] += rendimento;
            matriz[i][j] = calcular_fonte(rendimento); 
            retido_anual += matriz[i][j];
        }
        matriz[i][12] = retido_anual;
    }
    for (int i=0; i<n; i++) {scanf("%lf", &abatimento); calcular_ajuste_anual(abatimento, rendimento_anual[i]-abatimento, matriz, i);}
    printf("Jan\tFev\tMar\tAbr\tMai\tJun\tJul\tAgo\tSet\tOut\tNov\tDez\tRetido\tDevido\tAjuste\n");
    for (int i=0; i<n; i++) {for (int j=0; j<15; j++) printf("%.2lf\t", matriz[i][j]); printf("\n"); free(matriz[i]);}
    free(matriz); free(rendimento_anual);
    return 0;
}