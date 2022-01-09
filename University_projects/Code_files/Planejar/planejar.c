#include "planejar.h"
#include <stdio.h>
#include <string.h>

int verificar_duracao(data primeira, data segunda) { //verifica se primeira está 4 dias ou mais a frente de segunda
    if (primeira.ano > segunda.ano && (primeira.mes != 1 || segunda.mes != 12)) return 1;
    if (primeira.mes - segunda.mes == 1 || (primeira.mes == 1 && segunda.mes == 12)) {
        if (segunda.mes == 1 || segunda.mes == 3 || segunda.mes == 5 || segunda.mes == 7 || segunda.mes == 8
        || segunda.mes == 10 || segunda.mes == 12) {
            if (1 + primeira.dia + (31 - segunda.dia) >= 4) return 1;
            else return 0;
        }
        if (1 + primeira.dia + 30 - segunda.dia >= 4) return 1;
        return 0;
    }
    if (primeira.mes > segunda.mes) return 1;
    if (1 + primeira.dia - segunda.dia >= 4) return 1;
    return 0;
}

int compara_data(data primeira, data segunda) { //compara se a primeira vem antes da segunda
    if (primeira.ano < segunda.ano) return 1;
    else if (primeira.mes < segunda.mes) return 1;
    else if (primeira.dia <= segunda.dia) return 1;
    else return 0;
}

data gerar_data(int dia, int mes, int ano) {
    data retorno;
    retorno.dia = dia;
    retorno.mes = mes;
    retorno.ano = ano;
    return retorno;
}

voo registrar(int numero, char codigo_origem[4], char codigo_destino[4], int dia, int mes, int ano, double preco) {
    voo novo_voo;
    novo_voo.numero = numero;
    strcpy(novo_voo.codigo_destino, codigo_destino);
    strcpy(novo_voo.codigo_origem, codigo_origem);
    novo_voo.data_voo.dia = dia;
    novo_voo.data_voo.mes = mes;
    novo_voo.data_voo.ano = ano;
    novo_voo.preco = preco;
    return novo_voo;
}

void alterar(voo *voos, int numero, double novo_preco) {
    for (int i=0; i<100; i++) {
        if (voos[i].numero == numero) {
            voos[i].preco = novo_preco;
            break;
        }
    }
}

void cancelar(voo *voos, int numero) {
    int indice_cancelado;
    for (int i=0; i<100; i++){
        if (voos[i].numero == numero) {
            indice_cancelado = i;
            break;
        }
    }
    for (int i=indice_cancelado; i<99; i++) {
        voos[i] = voos[i+1];
    }
}

void planejar(voo *voos, char codigo_saida[4], data primeiro, data ultimo) {
    double melhor_preco = 100000000;
    int num_ida_resposta, num_volta_resposta;
    for (int i=0; i<100; i++) {
        if (strcmp(voos[i].codigo_origem, codigo_saida) == 0 && compara_data(primeiro, voos[i].data_voo)
        && compara_data(voos[i].data_voo, ultimo)) {
            for (int j=0; j<100; j++) {
                if (strcmp(voos[j].codigo_origem, voos[i].codigo_destino) == 0 && strcmp(voos[j].codigo_destino, codigo_saida) == 0
                && compara_data(voos[j].data_voo, ultimo)) {
                    if(verificar_duracao(voos[j].data_voo, voos[i].data_voo)) {
                        if (voos[i].preco + voos[j].preco < melhor_preco) {
                            melhor_preco = voos[i].preco + voos[j].preco;
                            num_ida_resposta = voos[i].numero;
                            num_volta_resposta = voos[j].numero;
                        }
                    }
                }
                else if (voos[j].codigo_origem[0] == '\0') break;
            }
        }
        if (voos[i].codigo_origem[0] == '\0') break;
    }
    printf("%d\n%d\n", num_ida_resposta, num_volta_resposta);
}