#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vertice { //cada vértice do grafo representa uma cidade
    char nome[51];
    int populacao;
    double centralidade; //aqui armazenaremos a centralidade do vértice em relação a uma dada raiz
    double distancia_minima; //aqui armazenaremos a distância mínima do vértice em relação a uma dada raiz
    int indice; //indice do vértice no vetor de vértices
} vertice;

typedef vertice * p_v;

typedef struct aresta { //cada aresta do grafo representa uma ligação entre 2 cidades
    double distancia; //distância entre as cidades
    struct aresta * prox; //As arestas serão implementadas em uma lista de adjacências, por isso possuem esse ponteiro
    int indice_destino; //índice, relativo ao vetor de vértices, da cidade de destino. Ex: uma aresta que liga a cidade
    //A em B terá o índice de B nesse registro da struct
} aresta;

typedef aresta * p_a;
 
typedef struct grafo { //O grafo foi definido como um vetor de vértices e uma lista de adjacências de arestas
    p_v vertices;
    p_a * arestas;
    int populacao_total; //Também armazenamos a população total do estado nesse registro
    int num_cidades; //E o número de cidades do estado nesse aqui
} grafo;

typedef struct { //O código também fará uso de um min-heap na execução do Algoritmo de Dijkstra
    p_v * vetor;
    int indice_final; //Índice do último elemento do vetor do heap
} heap;

//A seguinte função adicionará as arestas AB e BA na lista de adjacências do grafo
void adicionar_aresta(grafo cidades, char nome_cidade_a[51], char nome_cidade_b[51], double distancia) {

    int indice_cidade_a, indice_cidade_b;

    for (int i=0 ; i<cidades.num_cidades; i++) { //buscamos pelos indices das cidades no vetor de vértices do grafo
        if (strcmp(cidades.vertices[i].nome, nome_cidade_a) == 0) indice_cidade_a = i;
        else if (strcmp(cidades.vertices[i].nome, nome_cidade_b) == 0) indice_cidade_b = i;
    }

    p_a ab = malloc(sizeof(aresta)), ba = malloc(sizeof(aresta));

    ab->distancia = ba->distancia = distancia; //as distâncias são as mesmas nas arestas BA e AB
    ab->indice_destino = indice_cidade_b; //o índice de destino da aresta AB é o de B
    ba->indice_destino = indice_cidade_a; //o índice de destino da aresta BA é o de A

    ab->prox = cidades.arestas[indice_cidade_a]; //adicionamos ab na lista de adjacências
    cidades.arestas[indice_cidade_a] = ab;
    ba->prox = cidades.arestas[indice_cidade_b]; //adicionamos ba na lista de adjacências
    cidades.arestas[indice_cidade_b] = ba;
}

void troca(vertice ** elemento_a, vertice ** elemento_b) { //faz a troca entre 2 vértices em um vetor
    vertice * aux;
    aux = *elemento_a;
    *elemento_a = *elemento_b;
    *elemento_b = aux;    
}

//As seguintes macros servem para calcularmos os índices dos elementos pai e filhos em um heap implementado como vetor:
#define PAI(i) (i-1)/2
#define F_ESQ(i) (2*i+1)
#define F_DIR(i) (2*i+2)


void sobe_no_heap(int i, p_v * heap) { //Sobe o elemento de índice i para a sua posição correta no heap
    //se a distância miníma do vértice i é menor que a do seu pai:
    if (i > 0 && heap[PAI(i)]->distancia_minima > heap[i]->distancia_minima) {
        troca(&heap[i], &heap[PAI(i)]); //trocamos tais vértices
        sobe_no_heap(PAI(i), heap);
    }
}

void desce_no_heap(heap fila_prioridade, int i) { //Desce o elemento de índice i para a sua posição correta no heap
    int menor_filho; //menor filho é aquele com menor distância mínima 
    if (F_ESQ(i) <= fila_prioridade.indice_final) { //se existir um filho esquerdo
        menor_filho = F_ESQ(i); //inicialmente supomos que ele é o menor filho
        if (F_DIR(i) <= fila_prioridade.indice_final && //se existe um filho direito e ele tem distância menor que o outro
        fila_prioridade.vetor[F_ESQ(i)]->distancia_minima > fila_prioridade.vetor[F_DIR(i)]->distancia_minima)
            menor_filho = F_DIR(i); //o filho direito passa a ser o menor filho
        //Se o menor filho tem distância mínima menor que o pai:
        if (fila_prioridade.vetor[i]->distancia_minima > fila_prioridade.vetor[menor_filho]->distancia_minima) {
            troca(&fila_prioridade.vetor[i], &fila_prioridade.vetor[menor_filho]); //trocamos o pai com o menor filho
            desce_no_heap(fila_prioridade, menor_filho);
        }
    }
}

p_v extrair_minimo(heap * fila_prioridade) {
    //Trocaremos o primeiro elemento do heap com o último:
    troca(&fila_prioridade->vetor[0], &fila_prioridade->vetor[fila_prioridade->indice_final]);
    fila_prioridade->indice_final--; //O heap terá 1 elemento a menos
    desce_no_heap(*fila_prioridade, 0); //Ajustamos o novo 1o elemento de modo que a estrutura de heap se mantenha
    return fila_prioridade->vetor[fila_prioridade->indice_final + 1]; //retornamos o antigo 1o elemento
}

int dijkstra(vertice raiz, grafo cidades) {
    int populacao_conectada = 0; //população da componente conexa do grafo que raiz está inserida
    heap fila_prioridade;
    fila_prioridade.vetor = malloc(cidades.num_cidades * sizeof(p_v));
    fila_prioridade.indice_final = 0;
    p_v minimo;
    
    for(int i=0; i<cidades.num_cidades; i++) { //esse for ajsuta as distâncias mínimas para o Algoritmo de Dijkstra
        if (strcmp(cidades.vertices[i].nome, raiz.nome) == 0) { //se estamos na raiz
            cidades.vertices[i].distancia_minima = 0; //tomamos a distância mínima como 0
            minimo = &cidades.vertices[i];    
        }
        else { //se não estamos na raiz
            cidades.vertices[i].distancia_minima = 10e5; //tomamos a distância como "infinita"
            fila_prioridade.indice_final++;
            fila_prioridade.vetor[fila_prioridade.indice_final] = &cidades.vertices[i];
        }
    }
    fila_prioridade.vetor[0] = minimo; //Deixamos a raiz no início do vetor. Assim, teremos a estrutura de heap pronta

    while(fila_prioridade.indice_final >= 0) { //enquanto houver elemento no heap

        minimo = extrair_minimo(&fila_prioridade); //extraímos o mínimo do min-heap
    
        if (minimo->distancia_minima != 10e5) { //se esse mínimo estiver na componente conexa da raiz
            populacao_conectada += minimo->populacao; //sua população faz parte da população conectada
            if (cidades.arestas[minimo->indice] != NULL) { //se esse mínimo possui arestas
                
                //Iteraremos sobre cada aresta:
                for(p_a aresta = cidades.arestas[minimo->indice]; aresta != NULL; aresta = aresta->prox) {
                    
                    //Se tal aresta fizer parte de um novo melhor caminho da raiz até o seu vértice de destino:
                    if (minimo->distancia_minima + aresta->distancia < cidades.vertices[aresta->indice_destino].distancia_minima) {
                        
                        //Atualizamos a distância mínima ao vértice de destino com o novo melhor valor encontrado:
                        cidades.vertices[aresta->indice_destino].distancia_minima = minimo->distancia_minima + aresta->distancia;
                        
                        //Para mantermos a estrutura de heap, devemos encontrar tal vértice e o subir
                        for (int i = 0; i<=fila_prioridade.indice_final; i++) {
                            if (strcmp(cidades.vertices[aresta->indice_destino].nome, fila_prioridade.vetor[i]->nome) == 0) {
                                sobe_no_heap(i, fila_prioridade.vetor);
                            }
                        }
                    }
                }
            }
        }
        else break; //não há mais componentes conexas, portanto já podemos sair do while
    }
    free(fila_prioridade.vetor);
    return populacao_conectada;
}

double calcular_centralidade(grafo cidades, int populacao_conectada) { //Calcula a centralidade dos elementos do grafo
    //Nota-se que o grafo já estará com as distâncias mínimas em relação a certa raiz ajustadas pelo Algoritmo de Dijkstra
    double centralidade = 0;
    for (int i=0; i<cidades.num_cidades; i++) {
        if (cidades.vertices[i].distancia_minima != 10e5) {
            centralidade += cidades.vertices[i].populacao * cidades.vertices[i].distancia_minima;
        }
    }
    return centralidade/populacao_conectada;
}

void merge(vertice * vetor, int l, int m, int r, int tamanho_max) { //Faz o merge de 2 subvetores
    int i=l, j=m+1, k=0;
    vertice * aux = malloc(tamanho_max * sizeof(vertice)); 
    while (i <= m && j <= r) {
        if (vetor[i].centralidade < vetor[j].centralidade)
            aux[k++] = vetor[i++];
        else if (vetor[i].centralidade > vetor[j].centralidade) 
            aux[k++] = vetor[j++];
        else if (strcmp(vetor[i].nome, vetor[j].nome) < 0) 
            aux[k++] = vetor[i++];
        else 
            aux[k++] = vetor[j++];
    }
    while (i <= m) 
        aux[k++] = vetor[i++];
    while (j <= r) 
        aux[k++] = vetor[j++];
    for (i = l, k = 0; i<= r; i++, k++) {
        vetor[i] = aux[k];
    }
    free(aux);
}

void mergesort(vertice * vetor, int l, int r, int tamanho_max) { //faz o mergesort em um vetor dado
    int m = (l + r) / 2; //m representa o índice do meio, enquanto l e r, os das extremidades esquerdas e direitas, resp.
    if (l < r) {
        mergesort(vetor, l, m, tamanho_max);
        mergesort(vetor, m+1, r, tamanho_max);
        merge(vetor, l, m, r, tamanho_max);
    }
}

void destruir_aresta(p_a aresta) {
    if (aresta->prox != NULL) destruir_aresta(aresta->prox);
    free(aresta);
}

int main() {

    int populacao_conectada; //armazenará a população atingível a partir de uma cidade
    double distancia;
    char nome_cidade_a[51], nome_cidade_b[51];
    grafo cidades;
    scanf("%d", &cidades.num_cidades);
    p_v vertices_saida = malloc(cidades.num_cidades * sizeof(vertice)); //os vértices (cidades) que aparecerão na saída
    cidades.arestas = calloc(cidades.num_cidades, sizeof(p_a));
    cidades.vertices = malloc(cidades.num_cidades * sizeof(vertice));
    cidades.populacao_total = 0; //inicialmente tomamos a população total do estado como 0

    for (int i=0; i<cidades.num_cidades; i++) { //registramos os vértices do grafo e a população total do estado:
        vertice novo;
        scanf("%s %d", novo.nome, &novo.populacao);
        novo.indice = i;
        cidades.vertices[i] = novo;
        cidades.populacao_total += novo.populacao;
    }
    while (scanf("%s %s %lf", nome_cidade_a, nome_cidade_b, &distancia) != EOF) { //registramos as arestas do grafo:
        adicionar_aresta(cidades, nome_cidade_a, nome_cidade_b, distancia);
    }
    int j = 0; //j armazenará o índice do último vértice no vetor vertices_saida
    for (int i=0; i<cidades.num_cidades; i++) { //para cada vértice do grafo:
        populacao_conectada = dijkstra(cidades.vertices[i], cidades);
        //Usamos o Algoritmo de Dijsktra para encontrar a população conectada a um vértice e ajustar o registro
        //distancia_minima de cada vértice, de acordo com o vértice raiz indicado
        if (populacao_conectada >= cidades.populacao_total/2) { //Se o vértice conecta mais da metade da população total
            cidades.vertices[i].centralidade = calcular_centralidade(cidades, populacao_conectada);
            vertices_saida[j] = cidades.vertices[i]; //ele deve constar na saída do programa
            j++;
        }
    }
    mergesort(vertices_saida, 0, j-1, j); //ordenamos o vetor dos vértices da saída usando o mergesort
    for (int i=0; i<j; i++) {
        printf("%s %.2lf\n", vertices_saida[i].nome, vertices_saida[i].centralidade); //imprimimos a saída
    }
    free(vertices_saida);
    free(cidades.vertices);
    for (int i=0; i<cidades.num_cidades; i++) {
        if(cidades.arestas[i] != NULL) {
            destruir_aresta(cidades.arestas[i]);
        }
    }
    free(cidades.arestas);
    return 0;
}