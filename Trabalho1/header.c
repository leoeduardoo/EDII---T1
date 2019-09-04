//
//  header.c
//  Trabalho1
//
//  Created by Léo Silva on 04/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

//faz o dump do arquivo
void dump (){
    
    FILE *arquivo;
    
    if ((arquivo = fopen("/Users/leo/Desktop/Faculdade/ED2 - 2019/semana4/Trabalho1/Trabalho1/saida.bin", "r+")) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
    }
    
    //rewind(arquivo);
    
    char value;
    
    while (!feof(arquivo)){
        value = fgetc(arquivo);
        
        if (isprint(value)){
            //%x imprime em hexadecimal
            printf("%x ", value);
        }
    }
}

//insere registros no arquivo de saida
void insere(struct cadastro cadastro[]){
    
    FILE *inserido;
    
    if ((inserido = fopen("/Users/leo/Desktop/Faculdade/ED2 - 2019/semana4/Trabalho1/Trabalho1/inserido.bin", "r+")) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
    }
    
    FILE *saida;
    
    if ((saida = fopen("/Users/leo/Desktop/Faculdade/ED2 - 2019/semana4/Trabalho1/Trabalho1/saida.bin", "r+")) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
    }
    
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
    
    int k = 0;
    int quant_seek = 0;
    
    while(k < i){
        quant_seek += cadastro[k].tamanho;
        k++;
    }
    
    //escreve em inserido.bin o cod_segurado ja inserido
    fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
    
    //garante que saida.bin esteja na posição correta e não sobrescreva nada
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
