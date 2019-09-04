//
//  main.c
//  Trabalho1
//
//  Created by Léo Silva on 30/08/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

int main(void)
{
    
    FILE *entrada;
    
    if ((entrada = fopen("/Users/leo/Desktop/Faculdade/ED2 - 2019/semana4/Trabalho1/Trabalho1/insere.bin", "r")) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
        return 0;
    }
    
    FILE *saida;
    
    if ((saida = fopen("/Users/leo/Desktop/Faculdade/ED2 - 2019/semana4/Trabalho1/Trabalho1/saida.bin", "r+")) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo.\n");
        return 0;
    }
    
    char registro[TAM_REG_ENTRADA];
    int quant_registros = 0;
    
    //descobre quantos registros têm no arquivo para salvar na struct
    while (fread(&registro,sizeof(char),TAM_REG_ENTRADA,entrada))
    {
        quant_registros++;
    }
    
    //declara struct agora já tem o tamanho
    struct cadastro cadastro[quant_registros];
    
    //seta todas as posições como '\0'
    memset(&cadastro,(char)'\0',sizeof(cadastro));
    
    leEntrada(entrada, cadastro);
    
    insere(cadastro);
    insere(cadastro);
    insere(cadastro);
    dump();
    
    fclose(entrada);
    
    return 0;
}
