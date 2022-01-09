#include <stdio.h>
#include <stdlib.h>

typedef struct vertice { //cada vértice do grafo representa uma cela, que possui nome e diferentes atributos mínimos
    char nome[51];
    int forca;
    int destreza;
    int constituicao;
    int inteligencia;
    int carisma;
    int sabedoria;
} vertice;

typedef struct grafo {
    int ** adj; //matriz que representa o conjunto de arestas
    vertice * l; //vetor que armazena o conjunto de vértices
} grafo; //Ressalta-se que o índice de um vértice no vetor e na matriz de adjacências será o mesmo

int existe_aresta(vertice A, vertice B) { //verifica se existe (A,B), ou seja, se dá para ir de A a B em 1 jogada
    //calcularemos o número de pontos necessários para alguém que começou em A conseguir ir a B
    int pontos_necessarios = 0, diferencas[6]; 
    diferencas[0] = B.forca - A.forca;
    diferencas[1] = B.constituicao - A.constituicao;
    diferencas[2] = B.destreza - A.destreza;
    diferencas[3] = B.inteligencia - A.inteligencia;
    diferencas[4] = B.sabedoria - A.sabedoria;
    diferencas[5] = B.carisma - A.carisma;
    for (int i=0; i<6; i++) {
        if (diferencas[i] > 0) {
            pontos_necessarios += diferencas[i];
        }
    }
    if (pontos_necessarios > 1) return 0; //se for necessário mais de 1 ponto, não há como ir de A a B em 1 jogada
    else return 1;
}

//A próxima função retorna o índice de um elemento de "lista" com os mesmos atributos mínimos de "cela" 
int encontrar_cela(vertice cela, vertice * lista, int numero_de_celas) {
    for (int i=0; i<numero_de_celas; i++) {
        if (cela.carisma == lista[i].carisma && cela.constituicao == lista[i].constituicao
        && cela.destreza == lista[i].destreza && cela.forca == lista[i].forca &&
        cela.inteligencia == lista[i].inteligencia && cela.sabedoria == lista[i].sabedoria) {
            return i; //poderia haver mais de 1 indice possível, mas tal não influenciaria o resultado do programa
        }
    }
    return -1; //se não encontramos nenhuma cela correspondente ao desejado, retornamos -1
}

//A próxima função verifica se existe um caminho, percorrível em 2 jogadas, entre os vértices de índices saída e chegada
int existe_caminho(int saida, int chegada, int ** matriz, int numero_de_celas) {
    if (matriz[saida][chegada]) return 1; //se existe uma aresta ligando tais vertices
    for (int j=0; j<numero_de_celas; j++) { //para cada vértice do grafo
        if (matriz[saida][j] && matriz[j][chegada]) {//se existe o caminho saida --> vertice --> chegada
            return 1;
        }
    }
    return 0;
}

int main() {
    int numero_de_celas, numero_de_jogadores, termina, terminou_ao_menos_1_vez = 0;
    vertice cela;
    grafo rpg;
    scanf("%d", &numero_de_celas);
    rpg.adj = malloc(numero_de_celas * (sizeof(int*)));
    rpg.l = malloc(numero_de_celas * sizeof(vertice));

    for (int i=0; i<numero_de_celas; i++) {
        rpg.adj[i] = calloc(numero_de_celas, sizeof(int));
        scanf("%s %d %d %d %d %d %d", cela.nome, &cela.forca, &cela.destreza, &cela.constituicao,
        &cela.inteligencia, &cela.sabedoria, &cela.carisma);
        rpg.l[i] = cela;
    }

    //Montaremos uma matriz de adjacências que indica se é possível começar em i e ir até j em 1 jogada:
    for (int i=0; i<numero_de_celas; i++) {
        for (int j=0; j<numero_de_celas; j++) {
            rpg.adj[i][j] = existe_aresta(rpg.l[i], rpg.l[j]);
        }
    }

    scanf("%d", &numero_de_jogadores);
    //indices_inicio guarda os índices dos vértices correspondentes às posições iniciais dos jogadores:
    int * indices_inicio = malloc(numero_de_jogadores * sizeof(int));
    for (int i=0; i<numero_de_jogadores; i++) {
        scanf("%*s %d %d %d %d %d %d", &cela.forca, &cela.destreza, &cela.constituicao,
        &cela.inteligencia, &cela.sabedoria, &cela.carisma);
        indices_inicio[i] = encontrar_cela(cela, rpg.l, numero_de_celas);
    }

    for (int j=0; j<numero_de_celas; j++) { //iteramos sobre cada vértice
        termina = 1; //"termina" indica se o jogo termina no vértice de índice j. Inicialmente supomos que sim
        for (int i=0; i<numero_de_jogadores; i++) { //para cada jogador
            if (indices_inicio[i] == -1 || !existe_caminho(indices_inicio[i], j, rpg.adj, numero_de_celas)) {
                //se o jogador começou de um vértice que não existe ou não há caminho entre sua posição inicial e j:
                termina = 0;
                break;
            }
        }
        if (termina) { //"termina" ainda será 1 se todos os jogadores conseguirem chegar em j em no máximo 2 jogadas
            printf("%s\n", rpg.l[j].nome);
            terminou_ao_menos_1_vez = 1; //essa variável indica que é possível terminar o jogo ao menos de 1 maneira
        }
    }
    if (!terminou_ao_menos_1_vez) { //se não há nenhuma maneira de os jogadores se encontrarem em no máximo 2 jogadas
        printf("Impossivel terminar em duas rodadas.\n");
    }

    for (int i=0; i<numero_de_celas; i++)
        free(rpg.adj[i]);
    free(rpg.adj);
    free(rpg.l);
    free(indices_inicio);
    return 0;
}