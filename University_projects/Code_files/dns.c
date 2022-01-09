// O presente código faz uso de 2 árvores de busca rubro-negras esquerdistas. A primeira armazena os domínios e seus
// respectivos IPs, enquanto a segunda, as informações dos diferentes usuários.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Cor {VERMELHO, PRETO};

typedef struct no { //nó da árvore rubro-negra esquerdista, servindo para as 2 citadas
    char dominio[101], ip[16]; //a string domínio só será efetivamente usada na árvore dos domínios
    int consultas; //numero de consultas realizadas. Só será efetivamente usada na árvore dos usuários
    struct no * dir, * esq; //nós direitos e esquerdos
    enum Cor cor; //cor do nó
} no;

typedef no * p_no;

int ehvermelho(p_no raiz) { //verifica se o nó raiz é vermelho
    if (raiz == NULL)
        return 0;
    return raiz->cor == VERMELHO;
}

int ehpreto(p_no raiz) { //verifica se o nó raiz é preto
    if (raiz == NULL)
        return 0;
    return raiz->cor == PRETO;
}

p_no rotaciona_para_esquerda(p_no raiz) { //faz a operação de rotação para a esquerda
    p_no x = raiz->dir; //o nó x corresponde ao inicialmente armazenado em raiz->dir
    raiz->dir = x->esq; //x->esq é o nó inicialmente armazenado em raiz->dir->esq. Logo, deve ficar à direita da raiz
    x->esq = raiz; //como a raiz é maior que x, ela deve se manter a esquerda, porém agora como filha e não mãe
    x->cor = raiz->cor; //x assume a cor da raiz
    raiz->cor = VERMELHO; //a raiz fica vermelha
    return x; //x será a nova raiz. Essa rotação evita o problema de termos um nó filho vermelho na direita.
}

p_no rotaciona_para_direita(p_no raiz) { //faz a operação de rotação para a direita
    p_no x = raiz->esq; //o nó x corresponde ao inicialmente armazenado em raiz->esq
    raiz->esq = x->dir; //x->dir é o nó armazenado inicialmente em raiz->esq->dir. Logo, deve ficar à esquerda da raiz
    x->dir = raiz; //como a raiz é maior que x, ela deve ficar a direita de x, porém agora como filha e não mãe
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x; //x será a nova raiz. Essa rotação ajuda a evitar o problema de termos nós vermelhos com filhos vermelhos
    //se a raiz for preta, tal problema é resolvido. Caso ela seja vermelha, ainda precisaremos "subir" a cor vermelha
}

void sobevermelho(p_no raiz) { //faz a operação de subir vermelhos
    raiz->dir->cor = raiz->esq->cor = PRETO; //os nós filhos ficam pretos
    raiz->cor = VERMELHO; //a raiz fica vermelha
} //essa função resolve o problema de 2 filhos serem vermelhos

p_no inserir(p_no raiz, char dominio[101], char ip[16]) { //insere um novo nó na árvore e retorna o nó da raiz 
    if (raiz == NULL) { //se chegamos em um nó nulo, iremos preenchê-lo
        p_no novo = malloc(sizeof(no));
        novo->esq = novo->dir = NULL;
        novo->consultas = 1;
        novo->cor = VERMELHO;
        strcpy(novo->ip, ip);
        strcpy(novo->dominio, dominio);
        return novo;
    }

    if (dominio[0] != '\0') { //se o dominio não for uma string nula, então estamos inserindo na árvore dos domínios
        if (strcmp(dominio, raiz->dominio) < 0) //nessa árvore, inserimos ordenando pela ordem alfabética do domínio
            raiz->esq = inserir(raiz->esq, dominio, ip);
        else
            raiz->dir = inserir(raiz->dir, dominio, ip);
    }
    else { //se o domínio for nulo, então estamos na árvore dos usuários
        if (strcmp(ip, raiz->ip) < 0) //nessa árvore, inserimos com base na string que representa o ip do usuário
            raiz->esq = inserir(raiz->esq, dominio, ip);
        else
            raiz->dir = inserir(raiz->dir, dominio, ip);
    }
    //As próximas condicionais garantem que a árvore permaneça com as propriedades de rubro-negra esquerdista:
    
    if (ehvermelho(raiz->dir))
        raiz = rotaciona_para_esquerda(raiz); //evita filhos vermelhos à direita em relação a raiz atual
    if (ehvermelho(raiz->esq) && ehvermelho(raiz->esq->esq))
        raiz = rotaciona_para_direita(raiz); //evita filhos vermelhos com filhos vermelhos em relação a raiz atual
    if (ehvermelho(raiz->dir) && ehvermelho(raiz->esq)) 
        sobevermelho(raiz); //evita o caso de os 2 filhos serem vermelhos
    //As funções dessas condicionais deixam a raiz analisada vermelha. Caso isso seja problemático, será corrigido na
    //chamada recursiva do pai dessa raiz. 
    return raiz;
}

p_no busca_usuario(p_no raiz, char ip[16]) { //retorna o nó do usuário com o ip dado, ou NULL se ele não for encontrado
    if (raiz != NULL) {
        if (strcmp(ip, raiz->ip) < 0)
            raiz = busca_usuario(raiz->esq, ip);
        else if (strcmp(ip, raiz->ip) > 0)
            raiz = busca_usuario(raiz->dir, ip);
    }
    return raiz;
}

p_no busca_dominio(p_no raiz, char dominio[101]) { //retorna o nó com o domínio dado, ou NULL se ele não for encontrado
    if (raiz != NULL) {
        if (strcmp(dominio, raiz->dominio) < 0)
            raiz = busca_dominio(raiz->esq, dominio);
        else if (strcmp(dominio, raiz->dominio) > 0)
            raiz = busca_dominio(raiz->dir, dominio);
    }
    return raiz;
}

void destruir_arvore(p_no raiz) {
    if (raiz->dir != NULL) destruir_arvore(raiz->dir);
    if (raiz->esq != NULL) destruir_arvore(raiz->esq);
    free(raiz);
}

int main() {
    int u, n, m;
    char dominio[101], ip[16], get[4], from[5];
    scanf("%d %d", &u, &n);
    p_no raiz_dominios = NULL, raiz_usuarios = NULL;
    for (int i=0; i<n; i++) { //iremos inserir os dados na árvore dos dominios
        scanf("%s %s", dominio, ip);
        raiz_dominios = inserir(raiz_dominios, dominio, ip);
    }
    scanf("%d", &m);
    for (int i=0; i<m; i++) { //agora iremos inserir e trabalhar na árvore dos usuários
        scanf("%s %s %s %s", get, dominio, from, ip);
        p_no usuario = busca_usuario(raiz_usuarios, ip), dominio_desejado = busca_dominio(raiz_dominios, dominio);
        if (usuario == NULL) //se o usuário nunca fez uma consulta
            raiz_usuarios = inserir(raiz_usuarios, "\0", ip); //criamos um nó na árvore para esse usuário
        else if (usuario->consultas++ >= u) {//incrementa-se o número de consultas, que se for maior que u...
            printf("FORBIDDEN %s FROM %s\n", dominio, ip); // ... gera uma mensagem de bloqueio
            continue;
        }
        if (dominio_desejado == NULL) { //se o domínio não for encontrado
            printf("NOTFOUND %s FROM %s\n", dominio, ip);
            continue;
        }
        printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, ip, dominio_desejado->ip);
    }
    destruir_arvore(raiz_usuarios);
    destruir_arvore(raiz_dominios);
    return 0;
}