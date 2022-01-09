#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct navio{ //cada navio será tido como um nó de uma fila
    int quantidade; //quantidade de containers que precisam ser carregados/descarregados. Se quantidade = 0, o navio pode 
    //ir embora do porto.
    char nome[20];
    char mercadoria[20];
    char objetivo[11];
    struct navio * prox;
} navio;

typedef navio * p_n;

typedef struct container { //cada container será tido como o nó de uma pilha
    struct container * prox;
    char mercadoria[20];
} container;

typedef container * p_c;

typedef struct doca{ //cada doca possui uma pilha de containers
    int numero_containers;
    p_c topo; //ponteiro para o container que da o topo da pilha
} doca;

typedef doca * p_d;

p_n enfileirar(p_n fila) { //cria a fila inicial de navios, retornando o primeiro elemento dela
    p_n novo = malloc(sizeof(navio));
    scanf("%s %s %s %d", novo->nome, novo->objetivo, novo->mercadoria, &(novo->quantidade));
    if (fila == NULL) { //se a fila estiver vazia (estamos inserindo o primeiro navio)
        novo->prox = novo;
        return novo;
    }
    else { //caso contrário
        p_n elemento; //elemento apontará para o antigo último elemento da fila
        for (elemento = fila; elemento->prox != fila; elemento = elemento->prox);
        elemento->prox = novo; //o novo ficará atrás de elemento na fila
        novo->prox = fila; //atrás do novo teremos novamente o primeiro da fila, pois essa é circular
        return fila;
    }
}

p_c desempilhar(doca * pilha) { //tira o elemento do topo da pilha de containers de uma doca
    pilha->numero_containers--; //a doca perdeu um container
    p_c topo = pilha->topo->prox;
    free(pilha->topo);
    return topo; //o novo container do topo é retornado
}

p_c empilhar(doca * pilha, p_n fila) { //empilha um novo container na doca, vindo do primeiro elemento da fila
    pilha->numero_containers++;
    p_c novo = malloc(sizeof(container));
    strcpy(novo->mercadoria, fila->mercadoria);
    novo->prox = pilha->topo;
    return novo; //é retornado o novo topo da pilha
}

p_n remover_primeiro(p_n fila) { //remove o primeiro navio da fila, retornando o novo primeiro
    p_n ultimo;
    for (ultimo = fila; ultimo->prox != fila; ultimo = ultimo->prox);
    ultimo->prox = fila->prox;
    free(fila);
    return ultimo->prox;
}

p_n fimdafila(p_n fila) { //faz o primeiro elemento da fila ir para o final
    return fila->prox; //para isso basta retornarmos, como primeiro elemento da fia, o segundo
}

void desalocar_docas(p_d docas, int numero_docas) { //desaloca docas que ainda tem containers
    for (int i=0; i<numero_docas; i++) {
        while (docas[i].numero_containers != 0) {
            docas[i].topo = desempilhar(&docas[i]);
        }
    }
    free(docas);
}

int main() {
    int numero_docas, numero_navios, count, capacidade, sem_acao = 0;
    p_n fila = NULL;
    scanf("%d\n%d\n%d", &numero_docas, &capacidade, &numero_navios);
    p_d docas = malloc(sizeof(doca) * numero_docas);
    for (int i = 0 ; i<numero_docas; i++) {
        docas[i].topo = NULL;
        docas[i].numero_containers = 0;
    }
    for (int i = 0; i<numero_navios; i++) {
        fila = enfileirar(fila);
    }
    while (1) { //esse while roda a fila até os navios terminarem suas operações ou for constatado um alerta
        
        sem_acao++; //sem_acao representa o número de navios que, em sequência, passaram pelo porto e voltaram ao final da
        //fila sem conseguir realizar nenhuma ação de carga/descarga. Começamos supondo que o navio da vez é desse tipo.
        
        if (strcmp(fila->objetivo, "carrega") == 0) { //se queremos carregar um navio
            for (int i=0; i<numero_docas; i++) { //tentaremos a carga para todas as docas
                count = 0; //essa variavel guarda o número de conteiners transportados da doca ao navio

                while (docas[i].topo != NULL && strcmp(docas[i].topo->mercadoria, fila->mercadoria) == 0
                && fila->quantidade > 0) { //enquanto houver containers na doca, a mercadoria da doca for a desejada pelo
                //navio e a quantidade de containers que o navio precisa carregar for > 0

                    fila->quantidade--;
                    count++;
                    docas[i].topo = desempilhar(&docas[i]);
                }
                if (count > 0) { //se ocorreu uma operação de carga para a doca atual
                    printf("%s carrega %s doca: %d conteineres: %d\n", fila->nome, fila->mercadoria, i, count);
                    sem_acao = 0; //sem_acao volta a ser 0, pois o navio analisado não está formando uma sequência do tipo
                    //mencionado na linha 95 
                    break; //quebramos o laço que percorre as docas, pois já encontramos uma passível de prover carga
                }
            }
        }
        else { //se queremos descarregar um navio
            for (int i=0; i<numero_docas; i++) {
                count = 0;
                while (docas[i].numero_containers < capacidade && fila->quantidade > 0) { //enquanto a doca não estiver
                //lotada de containers e o primeiro navio da fila ainda tiver uma quantidade não nula para descarregar
                    fila->quantidade--; 
                    count++;
                    docas[i].topo = empilhar(&docas[i], fila);
                }
                if (count > 0) { //se ocorreu uma ação de descarga
                    printf("%s descarrega %s doca: %d conteineres: %d\n", fila->nome, fila->mercadoria, i, count);
                    sem_acao = 0;
                    break; //quebramos o laço que percorre as docas, pois já encontramos uma passível de receber carga
                }
            }
        }
        if (sem_acao == numero_navios) { //se a fila já rodou uma vez completa e nenhuma ação de carga/descarga aconteceu
            printf("ALERTA: impossivel esvaziar fila, restam %d navios.", numero_navios);
            break;
        }
        else if (fila->quantidade == 0) { //se o navio analisado cumpriu seu objetivo
            if (fila->prox == fila) //se a fila só possui um navio
                break;
            fila = remover_primeiro(fila); //removemos o navio analisado da fila
            numero_navios--; //existe um navio a menos na fila
        }
        else
            fila = fimdafila(fila); //o navio ainda precisa carregar/descarregar containers, então volta pro final da fila
    }
    free(fila); //liberamos o primeiro navio
    desalocar_docas(docas, numero_docas); //desalocamos o que sobrou nas docas, além do vetor "docas"
    return 0;
}