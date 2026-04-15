#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

int main(int argc, char **argv){
    FILE *f;
    Endereco e;
    int qt, c;
    if(argc != 2){
        fprintf(stderr, "Uso: %s [CEP]", argv[0]);
        return 1;
    }

    c = 0;
    printf("Tamanho da Estrutura: %ld\n\n",sizeof(Endereco) );

    f = fopen("cep_ordenado.dat", "r"); // modo de leitura de texto

    if(!f){
        fprintf(stderr, "Arquivo %s não pode ser aberto para leitura \n", argv[1]);
        return 1;
    }

    fseek(f, 0, SEEK_END); //corta pro final, isso me diz o tamanho em bytes do arquivo

    long tamanhoBytes = ftell(f); // armazenando o tamanho do arquivo EM BYTES
    long tamanhoRegistros = tamanhoBytes/sizeof(Endereco); //Aqui eu pego o tamanho dos registros
    long inicio = 0;
    long fim = tamanhoRegistros-1; //-1 porque ao colocar no final eu passo do documento, o -1 faz com que eu volte pro arquivo
    long meio;
    int achou = 0;
    while(fim >= inicio){
        meio = (inicio + fim)/2;
        fseek(f, meio*sizeof(Endereco), SEEK_SET);
        qt = fread(&e, sizeof(Endereco), 1, f);
        long aux = strncmp(argv[1], e.cep, 8);
        if(aux == 0){
            printf("CEP encontrado!\n");
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
            achou = 1;
			break;
        }
        else{
            if(aux<0)//se for menor
                fim = meio-1;
            else//se for maior
                inicio = meio+1;
            
        }
    }
    if(achou==0){//ou !achou
        printf("CEP não encontrado");
    }
    //acho que caberia tirar essa barriga feia de if else com um switch case, mas preguiça :p
    fclose(f);
}