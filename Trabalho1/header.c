//
//  header.c
//  Trabalho1
//
//  Created by Léo Silva on 04/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

//conta quantos registros tem no arquivo de entrada para serem inseridos
int contaRegistrosEntrada(){
    
    FILE *entrada;
    
    abreArquivo(&entrada, "r", "insere.bin");
    
    char registro[TAM_REG_ENTRADA];
    int quant_registros = 0;
    
    //descobre quantos registros têm no arquivo para salvar na struct
    while (fread(&registro,sizeof(char),TAM_REG_ENTRADA,entrada))
    {
        quant_registros++;
    }
    
    fclose(entrada);
    
    return quant_registros;
}

//abre o arquivo com a flag desejada
void abreArquivo (FILE** arquivo, char *flag, char *nome_arquivo){
    
    unsigned long tam = strlen(nome_arquivo) + strlen("/Users/leo/Desktop/Faculdade/ED2 - 2019/EDII-T1/Trabalho1/");
    
    char caminho_arquivo[tam];
    
    sprintf(caminho_arquivo, "%s%s", "/Users/leo/Desktop/Faculdade/ED2 - 2019/EDII-T1/Trabalho1/", nome_arquivo);
    
    if ((*arquivo = fopen(caminho_arquivo, flag)) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
        exit(0);
    }
    
}

//faz o dump do arquivo
void dump (){
    
    FILE *saida;
    
    abreArquivo(&saida, "r+", "saida.bin");
    
    char value;
    
    while (!feof(saida)){
        value = fgetc(saida);
        
        if (isprint(value)){
            //%x imprime em hexadecimal
            printf("%x ", value);
        }
    }
    
    fclose(saida);
    
}

//insere registros no arquivo de saida
void insere(struct cadastro cadastro[], struct lista_espacos* lista){
    
    FILE *inserido;
    FILE *saida;
    abreArquivo(&inserido, "r+", "inserido.bin");
    abreArquivo(&saida, "r+", "saida.bin");
    
    //contador das posições da struct cadastro
    int i = 0;
    
    //char para guardar a informação lida de inserido.bin
    char cod_segurado_inserido[3];
    
    //enquanto puder ler em inserido.bin verifica se cadastro[i].cod_segurado[2] == cod_segurado_inserido[2]
    //se igual, avança i
    //se diferente, reinicia loop
    while(fread(cod_segurado_inserido, sizeof(cod_segurado_inserido), 1, inserido)){
        if (cadastro[i].cod_segurado[2] == cod_segurado_inserido[2]){
            //printf("Codigo %.3s ja existe, procurando novo codigo.\n", cadastro[i].cod_segurado);
            i++;
        }
    }
    
    
    
    /////////////////////////////////////////////////////////
    
    int j = 0;
    int k = 0;
    int quant_seek = 0;
    
    struct lista_espacos* atual = lista;
    
    //caso a lista esteja vazia não houve remoção e a inclusão deve ser feita no fim do arquivo
    if (atual->prox == NULL){
        
        while(k < i){
            quant_seek += cadastro[k].tamanho;
            k++;
        }
        
        //escreve em inserido.bin o cod_segurado para evitar que haja duplicidade posteriormente
        fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
        
        //garante que saida.bin esteja na posição correta e não sobrescreva outros registros
        fseek(saida, quant_seek, 1);
        
        //escreve em saida.bin os dados
        fprintf(saida, "%d", cadastro[i].tamanho);
        
        fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, saida);
        fputc(CHAR_DIVISOR, saida);
        
        //int j = 0;
        
        while(cadastro[i].nome_segurado[j] != '\0'){
            fputc(cadastro[i].nome_segurado[j], saida);
            j++;
        }
        
        fputc(CHAR_DIVISOR, saida);
        
        j = 0;
        
        while(cadastro[i].seguradora[j] != '\0'){
            fputc(cadastro[i].seguradora[j], saida);
            j++;
        }
        
        fputc(CHAR_DIVISOR, saida);
        
        j = 0;
        
        while(cadastro[i].tipo_seguro[j] != '\0'){
            fputc(cadastro[i].tipo_seguro[j], saida);
            j++;
        }
    }
    else{
        while(atual->prox != NULL){
            
            //verifica se o penúltimo nó disponível é suficiente
            if(atual->tamanho >= cadastro[i].tamanho){
                
                //escreve em inserido.bin o cod_segurado para evitar que haja duplicidade posteriormente
                fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
                
                //garante que saida.bin esteja na posição correta e não sobrescreva outros registros
                fseek(saida, atual->offset, 1);
                
                //escreve em saida.bin os dados
                fprintf(saida, "%d", cadastro[i].tamanho);
                
                fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, saida);
                fputc(CHAR_DIVISOR, saida);
                
                //int j = 0;
                
                while(cadastro[i].nome_segurado[j] != '\0'){
                    fputc(cadastro[i].nome_segurado[j], saida);
                    j++;
                }
                
                fputc(CHAR_DIVISOR, saida);
                
                j = 0;
                
                while(cadastro[i].seguradora[j] != '\0'){
                    fputc(cadastro[i].seguradora[j], saida);
                    j++;
                }
                
                fputc(CHAR_DIVISOR, saida);
                
                j = 0;
                
                while(cadastro[i].tipo_seguro[j] != '\0'){
                    fputc(cadastro[i].tipo_seguro[j], saida);
                    j++;
                }
            }
            atual = atual->prox;
        }
    }
    
    //verifica se o último nó disponível é suficiente
    if(atual->tamanho >= cadastro[i].tamanho){
        
        //escreve em inserido.bin o cod_segurado para evitar que haja duplicidade posteriormente
        fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
        
        //garante que saida.bin esteja na posição correta e não sobrescreva outros registros
        fseek(saida, atual->offset, 1);
        
        //escreve em saida.bin os dados
        fprintf(saida, "%d", cadastro[i].tamanho);
        
        fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, saida);
        fputc(CHAR_DIVISOR, saida);
        
        //int j = 0;
        
        while(cadastro[i].nome_segurado[j] != '\0'){
            fputc(cadastro[i].nome_segurado[j], saida);
            j++;
        }
        
        fputc(CHAR_DIVISOR, saida);
        
        j = 0;
        
        while(cadastro[i].seguradora[j] != '\0'){
            fputc(cadastro[i].seguradora[j], saida);
            j++;
        }
        
        fputc(CHAR_DIVISOR, saida);
        
        j = 0;
        
        while(cadastro[i].tipo_seguro[j] != '\0'){
            fputc(cadastro[i].tipo_seguro[j], saida);
            j++;
        }
    }
    
    /*
     while(atual->prox != NULL){
     atual = atual->prox;
     }
     
     atual->prox = malloc(sizeof(struct lista_espacos));
     atual->prox->tamanho = 0;
     atual->prox->prox = NULL;
     */
    
    
    
    
    ///////////////////////////////////////////////////////
    
    
    
    /*
    int k = 0;
    int quant_seek = 0;
    
    while(k < i){
        quant_seek += cadastro[k].tamanho;
        k++;
    }
    
    //escreve em inserido.bin o cod_segurado para evitar que haja duplicidade posteriormente
    fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
    
    //garante que saida.bin esteja na posição correta e não sobrescreva outros registros
    fseek(saida, quant_seek, 1);
    
    //escreve em saida.bin os dados
    fprintf(saida, "%d", cadastro[i].tamanho);
    
    fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, saida);
    fputc(CHAR_DIVISOR, saida);
    
    int j = 0;
    
    while(cadastro[i].nome_segurado[j] != '\0'){
        fputc(cadastro[i].nome_segurado[j], saida);
        j++;
    }
    
    fputc(CHAR_DIVISOR, saida);
    
    j = 0;
    
    while(cadastro[i].seguradora[j] != '\0'){
        fputc(cadastro[i].seguradora[j], saida);
        j++;
    }
    
    fputc(CHAR_DIVISOR, saida);
    
    j = 0;
    
    while(cadastro[i].tipo_seguro[j] != '\0'){
        fputc(cadastro[i].tipo_seguro[j], saida);
        j++;
    }
    */
    
    fclose(inserido);
    fclose(saida);
}


//função para ler o arquivo de entrada e passar os registros para memória (struct)
void leEntrada(FILE* arquivo, struct cadastro cadastro[]){
    
    //salva o tamanho total dos campos
    int total_caracteres = 0;
    
    //salva o registro do arquivo para memória principal
    char registro[TAM_REG_ENTRADA];
    
    //quantidade de registros no arquivo de entrada
    int quant_registros = sizeof(&cadastro);
    
    //percorre as posições do registro em memória principal
    int i = 0;
    
    //percorre as posições da struct
    int j = 0;
    
    //percorre as posições dos membros da struct
    int k = 0;
    
    //contador do tamanho dos campos
    int conta_caracteres = 0;
    
    //garante que arquivo esteja no começo
    rewind(arquivo);
    
    //limpa a struct preenchendo tudo '\0'
    memset(cadastro,(char)'\0',sizeof(struct cadastro));
    
    //enquanto houver registros
    while (j < quant_registros){
        
        //le o registro do arquivo e passa para memória principal para ser manipulado
        fread(&registro,sizeof(char),TAM_REG_ENTRADA,arquivo);
        
        //por alguma razão cadastro[j].cod_segurado[k] está salvando junto o nome_segurado
        while(conta_caracteres < TAM_COD_SEGURADO){
            if(registro[i] != '\0' && registro[i] >= '0' && registro[i] <= '9'){
                cadastro[j].cod_segurado[k] = registro[i];
                k++;
            }
            i++;
            conta_caracteres++;
        }
        
        total_caracteres += k;
        
        k = 0;
        
        conta_caracteres = 0;
        
        while(conta_caracteres < TAM_NOME_SEGURADO){
            if(registro[i] != '\0'){
                cadastro[j].nome_segurado[k] = registro[i];
                k++;
            }
            i++;
            conta_caracteres++;
        }
        
        total_caracteres += k;
        
        k = 0;
        
        conta_caracteres = 0;
        
        while(conta_caracteres < TAM_SEGURADORA){
            if(registro[i] != '\0'){
                cadastro[j].seguradora[k] = registro[i];
                k++;
            }
            i++;
            conta_caracteres++;
        }
        
        total_caracteres += k;
        
        k = 0;
        
        conta_caracteres = 0;
        
        while(conta_caracteres < TAM_TIPO_SEGURO){
            if(registro[i] != '\0'){
                cadastro[j].tipo_seguro[k] = registro[i];
                k++;
            }
            i++;
            conta_caracteres++;
        }
        
        total_caracteres += k;
        
        //+ 5 por conta dos 3 # e 2 bits do tamanho
        cadastro[j].tamanho = total_caracteres + 5;
        
        j++;
        i = 0;
        k = 0;
        conta_caracteres = 0;
        total_caracteres = 0;
    }
}
