#include <stdio.h>
#include <stdlib.h>

//As seguintes macros ajudam a acessar elementos úteis do heap:
#define PAI(i) ((i-1)/2)
#define F_ESQ(i) (2*i+1)
#define F_DIR(i) (2*i+2)

typedef struct { //essa struct, chamada item, representa cada nó do maxheap que será empregado
    int id;
    int prioridade;
    int incremento;
} item;

typedef struct { //aqui definimos a fila de prioridade, que é um maxheap implementado como o vetor v
    item *v;
    int n, tamanho; //tamanho é o tamanho do vetor, enquanto n é o número de elementos atualmente no heap
} fp;

typedef fp * p_fp;

p_fp criar_heap(int tam) { //cria um heap do tamanho tam
    p_fp heap = malloc(sizeof(fp));
    heap->v = malloc(tam * sizeof(item));
    heap->n = 0;
    heap->tamanho = tam;
    return heap;
}

void troca(p_fp heap, int indice1, int indice2) { //faz a troca entre os elementos do heap com os indices indicados
    item aux = heap->v[indice1];
    heap->v[indice1] = heap->v[indice2];
    heap->v[indice2] = aux;
}

void sobe_no_heap(p_fp heap, int k) { //ajusta o heap, de baixo para cima e a partir do elemento de índice k
    if (k > 0 && heap->v[PAI(k)].prioridade < heap->v[k].prioridade) {
        troca(heap, k, PAI(k));
        sobe_no_heap(heap, PAI(k));
    }
}

void desce_no_heap(p_fp heap, int k) { //ajusta o heap, de cima para baixo e a partir do elemento de indice k
    int maior_filho; //primeiro, iremos descobrir qual dos filhos é o de maior prioridade
    if (F_ESQ(k) < heap->n) { //se o filho esquerdo existir
        maior_filho = F_ESQ(k); //supomos que ele é o de maior prioridade
        if (F_DIR(k) < heap->n &&
        heap->v[F_ESQ(k)].prioridade < heap->v[F_DIR(k)].prioridade) //se o direito existir e tiver + prioridade que o esquerdo
        maior_filho = F_DIR(k);
        if (heap->v[k].prioridade < heap->v[maior_filho].prioridade) { //se o filho de maior prioridade tiver + prioridade que o pai
            troca(heap, k, maior_filho);
            desce_no_heap(heap, maior_filho);
        }
    }
}

void inserir(p_fp heap, item elemento) { //insere um elemento no final do heap, e depois o sobe para a sua posição adequada
    heap->v[heap->n] = elemento;
    heap->n++;
    sobe_no_heap(heap, heap->n - 1);
}

void remover_raiz(p_fp heap) { //remove a raiz do heap. Tal raiz é sempre o elemento de maior prioridade
    troca(heap, 0, heap->n - 1);
    heap->n--;
    desce_no_heap(heap, 0);
}

void incrementar_prioridades(p_fp heap) { //faz o incremento das prioridades, que ocorre após cada tick
    for (int i=0; i<heap->n; i++) {
        heap->v[i].prioridade += heap->v[i].incremento;
        sobe_no_heap(heap, i);
    }
}

void encaminhar(p_fp heap, int k) { //faz o encaminhamento dos pacotes após um tick
    int i = 0;
    while (i < k && heap->n > 0) { //enquanto houver pacotes a serem enviados e não estourarmos o limite k
        printf("%d %d %d\n", heap->v[0].id, heap->v[0].prioridade, heap->v[0].incremento);
        remover_raiz(heap);
        i++;
    }
    printf("\n");
    incrementar_prioridades(heap); //fazemos o incremento de prioridades que ocorre após cada encaminhamento
}

int main() {
    int k, m, id, prioridade, incremento, numero_tick = 1;
    item novo;
    scanf("%d %d", &k, &m);
    p_fp heap = criar_heap(m);
    while (scanf("%d %d %d", &id, &prioridade, &incremento) != EOF) {
        if (id == 0 && prioridade == 0 && incremento == 0) { //se o roteador emite um tick
            printf("TICK %d\n", numero_tick++);
            encaminhar(heap, k);
        }
        else if (heap->n < m) { //se o roteador está recebendo pacotes a serem colocados na fila de prioridade
            novo.id = id;
            novo.prioridade = prioridade;
            novo.incremento = incremento;
            inserir(heap, novo);
        }
    }
    free(heap->v);
    free(heap);
    return 0;
}