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
    FILE *saida;
    
    abreArquivo(&entrada, "r", "insere.bin");
    abreArquivo(&saida, "r+", "saida.bin");
    
    //declara struct agora já tem o tamanho
    struct cadastro cadastro[contaRegistrosEntrada()];
    
    //seta todas as posições como '\0'
    memset(&cadastro,(char)'\0',sizeof(cadastro));
    
    int opcao = 0;
    
    do {
        printf("\nCadastro de Seguradora:\n");
        printf("\n\t1. Insercao");
        printf("\n\t2. Remocao");
        printf("\n\t3. Compactacao");
        printf("\n\t4. Dump Arquivo");
        printf("\n\t5. Carrega Arquivos");
        printf("\n\t6. Sair\n");
        printf("\nDigite uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                insere(cadastro);
                break;
                
            case 2:
                printf("\nRemocao\n");
                break;
                
            case 3:
                printf("\nCompactacao\n");
                break;
                
            case 4:
                dump();
                break;
                
            case 5:
                leEntrada(entrada, cadastro);
                break;
                
            case 6:
                break;
                
            default:
                printf("\nOpcao invalida.\n");
        }
        
    } while (opcao != 6);
    
    fclose(entrada);
    fclose(saida);
    
    return 0;
}
