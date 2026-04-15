#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Endereco Endereco;
typedef struct _Indice Indice;

//O que eu preciso fazer, colocar um indice, dividir o arquivo pelo indice criado, depois ordenar os indices, por fim fazer a buscabinaria pelo indice
struct _Endereco{
    char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

struct _Indice{
	char cep[8];
	int ind;
};

int compara(const void *a, const void *b);

int main(int argc, char**argv){

    FILE *file, *saida;
	Endereco e;

	file = fopen("cep.dat", "rb");

	if(!file){
		fprintf(stderr, "Erro ao abrir arquivo\n");
		return 1;
	}

	fseek(file, 0, SEEK_END);
	long tamanho = ftell(file);
	int total = tamanho / sizeof(Endereco);
	rewind(file);

	Indice *in = malloc(sizeof(Indice)*total); // vetor da struct indices
	if(in == NULL){
		fprintf(stderr, "Erro!\n");
		return 1;
	}
	int i = 0;
	while(fread(&e, sizeof(Endereco), 1, file) == 1){//seria melhor for pq eu sei o tamanho total, mas eu quero fazer com for
		strncpy(in[i].cep,e.cep, 8);
		in[i].ind = i;
		i++;
	}

	qsort(in, total, sizeof(Indice), compara);
	saida = fopen("indice_ordenado.dat", "wb");
	if(!saida){
		printf("Erro ao abrir");
		return 1;
	}
	fwrite(in, sizeof(Indice), total, saida);

	//Aqui começa a parte da busca binária, vou tentar colocar como uma função para facilitar minha vida

    long inicio = 0;
    long fim = total -1;
    long meio;
    int achou = 0;


    while(fim >= inicio){
        meio = (inicio + fim)/2;
        long aux = strncmp(argv[1], in[meio].cep, 8);
        if(aux == 0){
            fseek(file, (long)in[meio].ind * sizeof(Endereco), SEEK_SET);
            fread(&e, sizeof(Endereco), 1, file);
            printf("CEP encontrado!\n");
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
            achou = 1;
			break;
        }
        else{
            if(aux<0)//se for menor
                fim = meio - 1;
            else//se for maior
                inicio = meio+1;
            
        }
    }
    if(achou==0){//ou !achou
        printf("CEP não encontrado");
    }


	fclose(file);
	fclose(saida);
    free(in);
    return 0;
}

int compara(const void *a, const void *b){
	return strncmp(((Indice*)a)->cep, ((Indice*)b)->cep, 8);
}

/*


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



*/