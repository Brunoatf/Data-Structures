#include <stdio.h>
int paginas[10000], n;

int encontrar_indice(int numero) {
    int indice = 0;
    for (int i=1; i<n; i++) if (numero == paginas[i]) indice = i;
    return indice;
}

void conversar(int a, int b) {
    int ponta_esquerda_do_grupo = encontrar_indice(a);
    int ponta_direita_do_grupo = encontrar_indice(b);
    while (ponta_esquerda_do_grupo < ponta_direita_do_grupo) {
        int aux = paginas[ponta_esquerda_do_grupo];
        paginas[ponta_esquerda_do_grupo] = paginas[ponta_direita_do_grupo]; paginas[ponta_direita_do_grupo] = aux;
        ponta_esquerda_do_grupo++; ponta_direita_do_grupo--;
    }
}

void esperar(int a, int b) {
    int indice_a = encontrar_indice(a), indice_b = encontrar_indice(b);
    int aux = paginas[indice_a];
    for (int i=indice_a; i<indice_b; i++) paginas[i] = paginas[i+1];
    paginas[indice_b] = aux;
}

int main() {
    int m, identificador_do_movimento, a, b;
    scanf("%d %d", &n, &m);
    for (int i=0; i<n; i++) paginas[i] = i+1;
    for (int i=0; i<m; i++) {
        scanf("%d %d %d", &identificador_do_movimento, &a, &b);
        if (identificador_do_movimento == 1) conversar(a, b);
        else esperar(a, b);
    }
    for (int i=0; i<n; i++) printf("%d ", paginas[i]);
}