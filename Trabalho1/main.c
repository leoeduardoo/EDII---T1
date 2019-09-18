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
    //declara a lista de espacos
    struct lista_espacos lista;
   
    lista.offset = -1;
    lista.prox = NULL;
    
    FILE *entrada;
    FILE *saida;
    FILE *remove;
    
    abreArquivo(&entrada, "r", "insere.bin");
    abreArquivo(&saida, "r+", "saida.bin");
    abreArquivo(&remove, "r", "remove.bin");
    
    //declara struct agora que já tem o tamanho
    int quant_registros_insere = contaRegistrosEntrada();
    struct cadastro cadastro[quant_registros_insere];
    
    //seta todas as posições como '\0'
    memset(&cadastro,(char)'\0',sizeof(cadastro));
    
    //declara struct contendo quais registros serão removidos
    int quant_registros_remove = contaRegistrosRemover();
    struct remocao remocao[quant_registros_remove];
    
    //seta todas as posições como '\0'
    memset(&remocao,(char)'\0',sizeof(remocao));
    
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
        fflush(stdin);
        scanf(" %d", &opcao);
        
        switch (opcao) {
            case 1:
                if (arquivoCarregado(remocao, cadastro, 1)){
                    insereRegistro(cadastro, &lista);
                }
                break;
                
            case 2:
                if (arquivoCarregado(remocao, cadastro, 2)){
                    removeRegistro(remocao, cadastro, &lista);
                }
                break;
                
            case 3:
                compacta(saida);
                break;
                
            case 4:
                dump();
                break;
                
            case 5:
                carregaArquivos(entrada, remove, cadastro, remocao, quant_registros_insere, quant_registros_remove);
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
