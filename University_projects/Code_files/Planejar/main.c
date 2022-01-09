#include <stdio.h>
#include "planejar.h"

int main() {
    char operacao[10];
    voo voos[100];
    int contagem = 0;
    while (1) {
        scanf("%s", operacao);
        if (operacao[0] == 'r') {
            int numero, dia, mes, ano;
            char codigo_origem[4], codigo_destino[4];
            double preco;
            scanf("%d %s %s %d/%d/%d %lf", &numero, codigo_origem, codigo_destino, &dia, &mes, &ano, &preco);
            voos[contagem] = registrar(numero, codigo_origem, codigo_destino, dia, mes, ano, preco);
            contagem++;
        }
        else if (operacao[0] == 'a') {
            int numero;
            double novo_preco;
            scanf("%d %lf", &numero, &novo_preco);
            alterar(voos, numero, novo_preco);
        }
        else if (operacao[0] == 'c') {
            int numero;
            scanf("%d", &numero);
            cancelar(voos, numero);
        }
        else {
            char codigo_saida[4];
            int dia_1, dia_2, mes_1, mes_2, ano_1, ano_2;
            scanf("%s %d/%d/%d %d/%d/%d", codigo_saida, &dia_1, &mes_1, &ano_1, &dia_2, &mes_2, &ano_2);
            planejar(voos, codigo_saida, gerar_data(dia_1, mes_1, ano_1), gerar_data(dia_2, mes_2, ano_2));
            break;
        }
    }
}