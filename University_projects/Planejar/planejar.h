typedef struct data {
    int dia, mes, ano;
} data;

typedef struct voo {
    int numero;
    char codigo_origem[4], codigo_destino[4];
    data data_voo;
    double preco;
} voo;

voo registrar(int numero, char codigo_origem[4], char codigo_destino[4], int dia, int mes, int ano, double preco);
void alterar(voo *voos, int numero, double novo_preco);
void planejar(voo *voos, char codigo_saida[4], data primeiro, data ultimo);
void cancelar(voo *voos, int numero);
data formatar(char data_nao_formatada[10]);
data gerar_data(int dia, int mes, int ano);