#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//fiz isso pra garantir ser genérico, mas estou assumindo no código que seja inteiros
typedef int tipo;


int compara(const void *e1, const void *e2);
void processarBlocos(FILE *file, long totalBytes, int k);
void gerenciaIntercala(int k);
void intercala(char *a1, char *b1, char *saida1);

int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr, "Erro, não tem parametros");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if(!file){
        fprintf(stderr, "Erro ao abrir arquivo");
        return 1;
    }

    fseek(file, 0, SEEK_END);

    long tamanho = ftell(file);
    rewind(file);
    int k;
    printf("Por quantos blocos deseja dividir(preferencialmente coloque uma potencia de 2)\n");
    scanf("%d", &k);
    processarBlocos(file, tamanho, k);
    fclose(file);

    gerenciaIntercala(k);

    return 0;
}

 int compara(const void *e1, const void *e2){
    if (*(tipo*)e1 < *(tipo*)e2) 
        return -1;
    if (*(tipo*)e1 > *(tipo*)e2) 
        return 1;
    return 0;
 }

 void processarBlocos(FILE *file, long totalBytes, int k){
    long totalElementos = totalBytes/sizeof(tipo);
    long elementosBloco = totalElementos/k;

    long tamanhoMax = elementosBloco + (totalElementos % k); //para caso seja um numero quebrado tipo 13/4, iria sobrar 1

    tipo *buffer = malloc(tamanhoMax*sizeof(tipo));

    long lidos;
    char nome[100];
    for(int i = 0; i < k; i++){
        if(i == k-1)
            lidos = totalElementos - (i*elementosBloco);
        else
            lidos = elementosBloco;
        
        fread(buffer, sizeof(tipo), lidos, file);
        qsort(buffer, lidos, sizeof(tipo), compara);
        sprintf(nome, "bloco%d.dat", i);

        FILE *temp = fopen(nome, "wb");
        if(temp){
            fwrite(buffer, sizeof(tipo), lidos, temp);
            fclose(temp);
        }
    }
    free(buffer);
}

void gerenciaIntercala(int k){
    char nomes[200][100];
    int totalArquivos = k;
    int atual = 0;

    for(int i = 0; i < k; i++)
        sprintf(nomes[i], "bloco%d.dat", i);
    
    while(totalArquivos - atual > 1){
        char *arq1 = nomes[atual++];
        char *arq2 = nomes[atual++];
        
        char nomeSaida[100];

        sprintf(nomeSaida, "temporario%d.dat", totalArquivos);
        strcpy(nomes[totalArquivos], nomeSaida);

        intercala(arq1, arq2, nomeSaida);

        remove(arq1);
        remove(arq2);

        totalArquivos++;
    }

    printf("Arquivo final ordenado: %s\n", nomes[totalArquivos - 1]);

}

void intercala(char *a1, char *b1, char *saida1){
    
    FILE *a = fopen(a1, "rb");
    FILE *b = fopen(b1, "rb");
    FILE *saida = fopen(saida1, "wb");
    tipo ta, tb;

    //não coloquei ainda o fopen pq não sei como fazer para ler
    int leuA = fread(&ta, sizeof(tipo), 1, a);
    int leuB = fread(&tb, sizeof(tipo), 1, b);

    while(leuA && leuB){
        if(compara(&ta, &tb) < 0){
            fwrite(&ta, sizeof(tipo), 1, saida);
            leuA = fread(&ta, sizeof(tipo), 1, a);
        }
        else{
            fwrite(&tb, sizeof(tipo), 1, saida);
            leuB = fread(&tb, sizeof(tipo), 1, b);
        }
    }
    
    while(leuA){
        fwrite(&ta, sizeof(tipo), 1, saida);
        leuA = fread(&ta, sizeof(tipo), 1, a); 
    }

    while(leuB){
        fwrite(&tb, sizeof(tipo), 1, saida);
        leuB = fread(&tb, sizeof(tipo), 1, b);
    }

    fclose(a);
    fclose(b);
    fclose(saida);

}