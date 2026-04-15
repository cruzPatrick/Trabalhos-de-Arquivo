#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//fiz isso pra garantir ser genérico, mas estou assumindo no código que seja inteiros
typedef int tipo;

 int compara(const void *e1, const void *e2){
    return (*(int*)e1) - (*(int*)e2);
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
            lidos = totalElementos;
        
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

void intercala(FILE *a, FILE *b, FILE *saida){
    tipo ta, tb;

    //não coloquei ainda o fopen pq não sei como fazer para ler
    fread(&ta, sizeof(tipo), 1, a);
    fread(&tb, sizeof(tipo), 1, b);

    while(!feof(a) && !feof(b)){
        if(compara(feof(a), feof(b)) < 0){
            fwrite(&ta, sizeof(tipo), 1, saida);
            fread(&ta, sizeof(tipo), 1, a);
        }
        else{
            fwrite(&tb, sizeof(tipo), 1, saida);
            fread(&tb, sizeof(tipo), 1, b);
        }
    }
    
    while(!feof(a)){
        fwrite(&ta, sizeof(tipo), 1, saida);
        fread(&ta, sizeof(tipo), 1, a); 
    }

    while(!feof(b)){
        fwrite(&tb, sizeof(tipo), 1, saida);
        fread(&tb, sizeof(tipo), 1, b);
    }

    fclose(a);
    fclose(b);
    fclose(saida);

}

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

    for(int i = 0; i < k; i++){
        FILE *aux1 = fopen("bloco%d.dat", "rb+");
        FILE *aux2 = fopen("bloco%d+1.dat", "rb+");
        FILE *final;
        intercala(aux1, aux2, final);
    }

    fclose(file);


    return 0;
}