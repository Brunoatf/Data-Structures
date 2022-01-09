#include <stdio.h>
#include <stdlib.h>

int main() {
    char *DNA = malloc(26 * sizeof(char)), *RNA = malloc(26 * sizeof(char));
    int n;
    scanf("%s %s %d", DNA, RNA, &n);
    char *DNA_para_transcrever = malloc((n+1) * sizeof(char)), *RNA_transcrito = malloc((n+1) * sizeof(char));
    int *contagem = malloc(26 * sizeof(int)), *id_DNA = malloc(n * sizeof(int));
    scanf("%s", DNA_para_transcrever);

    //inicializando os vetores:
    for (int i=0; i<26; i++) contagem[i] = 0;
    for (int i=0; i<n; i++) id_DNA[i] = 0;

    for (int i=0; i<n; i++) {
        for (int j=0; j<26; j++) {
            if (DNA_para_transcrever[i] == DNA[j]) {
                if (contagem[j] == 0) {
                    contagem[j]++;
                }
                else {
                    id_DNA[i] = contagem[j]; //id nao poder ser inicializado com uma variavel?
                    contagem[j]++;
                }
                RNA_transcrito[i] = RNA[j];
                break;
            }
        }
    }

    //impressão da saída:
    printf("ID:\t");
    for (int i=0; i<n; i++) printf("%d\t", id_DNA[i]);
    printf("\nDNA:\t");
    for (int i=0; i<n; i++) printf("%c\t", DNA_para_transcrever[i]);
    printf("\n    \t");
    for (int i=0; i<n; i++) printf("|\t");
    printf("\nRNA:\t");
    for (int i=0; i<n; i++) printf("%c\t", RNA_transcrito[i]);

    //desalocação da memória:
    free(contagem);
    free(DNA_para_transcrever);
    free(RNA_transcrito);
    free(RNA);
    free(DNA);
    free(id_DNA);

    return 0;
}