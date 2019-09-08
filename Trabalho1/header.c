//
//  header.c
//  Trabalho1
//
//  Created by Léo Silva on 04/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

//atualiza a lista de espaços disponíveis
void atualizaLista(struct lista_espacos* lista, int quant_seek){
    
    struct lista_espacos lista_aux = *lista;
    
    lista->prox = malloc(sizeof(struct lista_espacos));
    
    lista->prox->prox = lista_aux.prox;
    lista->prox->offset = lista_aux.offset;
    
    lista->offset = 4 + quant_seek;
}

//conta somente os caracteres no buffer
int contaCharBuffer(char * buffer){
    
    int i = 0;
    int total_caracteres = 0;
    
    while(i < TAM_REG_ENTRADA){
        if(buffer[i] != '\0'){
            total_caracteres++;
        }
        i++;
    }
    return total_caracteres;
}


//remove registros no arquivo de saida
void removeRegistro(struct remocao *remocao, struct cadastro *cadastro, struct lista_espacos* lista){
    
    FILE *saida;
    FILE *removido;
    abreArquivo(&removido, "r+", "removido.bin");
    abreArquivo(&saida, "r+", "saida.bin");
    
    //caractere que indica registro removido
    char caractere_removido = '*';
    
    //contador
    int k = 0;
    
    //quantidade de seek
    int quant_seek = 0;
    
    //flag muda para true quando encontrar no arquivo o registro que será removido
    bool flag_achou = false;
    
    //garante que arquivo esteja no começo
    rewind(saida);
    
    //pula o cabeçalho e o tamanho do primeiro registro
    fseek(saida, 6, 1);
    
    //armazena o cod_segurado lido de saida.bin
    char buffer_saida[3];
    
    //limpa o buffer preenchendo tudo '\0'
    memset(buffer_saida,(char)'\0',sizeof(buffer_saida));
    
    //contador para a posição da struct cadastro
    int i = 0;
    
    //char para guardar a informação lida de removido.bin
    char buffer_removido[4];
    
    //limpa o buffer preenchendo tudo '\0'
    memset(buffer_removido,(char)'\0',sizeof(buffer_removido));
    
    //enquanto puder ler em inserido.bin significa que á foi feita remoção e encontra código válido
    while(fread(buffer_removido, sizeof(buffer_removido), 1, removido)){
        if (remocao[i].cod_segurado[2] == buffer_removido[2]){
            //printf("Codigo %c ja existe, procurando novo codigo.\n", remocao[i].cod_segurado[2]);
            i++;
        }
    }
    
    fread(buffer_saida, sizeof(buffer_saida), 1, saida);
    
    while(!flag_achou){
        
        rewind(saida);
        
        //pula o cabeçalho e o tamanho do primeiro registro
        fseek(saida, 6, 1);
        
        //armazena a quantidade de seek até o registro procurado
        quant_seek += strlen(cadastro[k].cod_segurado);
        quant_seek += strlen(cadastro[k].nome_segurado);
        quant_seek += strlen(cadastro[k].seguradora);
        quant_seek += strlen(cadastro[k].tipo_seguro);
        quant_seek += 5;
        
        fseek(saida, quant_seek, 1);
        
        if (buffer_saida[0] != remocao[i].cod_segurado[0]){
            fread(buffer_saida, sizeof(buffer_saida), 1, saida);
        }
        else{
            if (buffer_saida[1] != remocao[i].cod_segurado[1]){
                fread(buffer_saida, sizeof(buffer_saida), 1, saida);
            }
            else{
                if (buffer_saida[2] != remocao[i].cod_segurado[2]){
                    fread(buffer_saida, sizeof(buffer_saida), 1, saida);
                }
                else{
                    flag_achou= true;
                }
            }
        }
        
        k++;
    }
    
    //remove a quantidade de seek a mais que foi lida
    quant_seek -= strlen(cadastro[k-1].cod_segurado);
    quant_seek -= strlen(cadastro[k-1].nome_segurado);
    quant_seek -= strlen(cadastro[k-1].seguradora);
    quant_seek -= strlen(cadastro[k-1].tipo_seguro);
    quant_seek -= 5;
    
    rewind(saida);
    
    //pula o cabeçalho e o tamanho do primeiro registro
    fseek(saida, 6 + quant_seek, 1);
    
    //atualiza a lista de espaços disponíveis
    atualizaLista(lista, quant_seek);
    
    fputc(caractere_removido, saida);
    fputc(caractere_removido, saida);
    fprintf(saida, "%d", lista->prox->offset);
    
    //escreve em remocao.bin o cod_segurado para evitar que haja duplicidade posteriormente
    fwrite(remocao[i].cod_segurado, sizeof(remocao[i].cod_segurado), 1, removido);
    
    rewind(saida);
    
    //4 para pular os 3 digitos do offset + '\n'
    int teste = 4 + quant_seek;
    
    fprintf(saida, "%03d", teste);
    
    fclose(saida);
    fclose(removido);
}

//conta quantos registros tem no arquivo de entrada para serem inseridos
int contaRegistrosRemover(){
    
    FILE *remove;
    
    abreArquivo(&remove, "r", "remove.bin");
    
    char registro[TAM_REG_REMOVE];
    int quant_registros = 0;
    
    //descobre quantos registros têm no arquivo para salvar na struct
    while (fread(&registro,sizeof(char),TAM_REG_REMOVE,remove))
    {
        quant_registros++;
    }
    
    fclose(remove);
    
    return quant_registros;
}

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
void insereRegistro(struct cadastro *cadastro, struct lista_espacos* lista){
    
    FILE *inserido;
    FILE *saida;
    
    abreArquivo(&inserido, "r+", "inserido.bin");
    abreArquivo(&saida, "r+", "saida.bin");
    
    //contador das posições da struct cadastro
    int i = 0;
    
    //contador
    int k = 0;
    
    //quantidade de seek
    int quant_seek = 0;
    
    //char para guardar a informação lida de inserido.bin
    char cod_segurado_inserido[TAM_COD_SEGURADO];
    
    //escreve o tamanho em saida.bin
    char buffer[TAM_REG_ENTRADA];
    
    //limpa a struct preenchendo tudo '\0'
    memset(buffer,(char)'\0',sizeof(buffer));
    
    //cabeçalho inicial
    char cabecalho[] = "0-1";
    
    //verifica se o arquivo está vazio para inserir o cabeçalho
    if(fgetc(saida) == EOF){
        fputc(cabecalho[0], saida);
        fputc(cabecalho[1], saida);
        fputc(cabecalho[2], saida);
        fputc('\n', saida);
    }
    
    rewind(saida);
    
    //pula o header e '\n'
    fseek(saida, 4, 1);
    
    //enquanto puder ler em inserido.bin significa que já foi feita inserção e encontra código válido
    while(fread(cod_segurado_inserido, sizeof(cod_segurado_inserido), 1, inserido)){
        if (cadastro[i].cod_segurado[2] == cod_segurado_inserido[2]){
            //printf("Codigo %.3s ja existe, procurando novo codigo.\n", cadastro[i].cod_segurado);
            i++;
        }
    }
    
    //soma todos os offsets desde o primeiro registro até cadastro[i]
    while(k < i){
        
        quant_seek += strlen(cadastro[k].cod_segurado);
        quant_seek += strlen(cadastro[k].nome_segurado);
        quant_seek += strlen(cadastro[k].seguradora);
        quant_seek += strlen(cadastro[k].tipo_seguro);
        k++;
        
        //incrementa 5: 3 '#' e 2 digitos do tamanho
        if(k >= 1){
            quant_seek += 5;
        }
    }
    
    //escreve em inserido.bin o cod_segurado ja inserido
    fwrite(cadastro[i].cod_segurado, sizeof(cadastro[i].cod_segurado), 1, inserido);
    
    //garante que saida.bin esteja na posição correta e não sobrescreva nada
    fseek(saida, quant_seek, 1);
    
    memcpy(buffer, cadastro[k].cod_segurado, sizeof(cadastro[k].cod_segurado));
    memcpy(buffer + sizeof(cadastro[k].cod_segurado), cadastro[k].nome_segurado, sizeof(cadastro[k].nome_segurado));
    memcpy(buffer + sizeof(cadastro[k].cod_segurado) + sizeof(cadastro[k].nome_segurado), cadastro[k].seguradora, sizeof(cadastro[k].seguradora));
    memcpy(buffer + sizeof(cadastro[k].cod_segurado) + sizeof(cadastro[k].nome_segurado) + sizeof(cadastro[k].tipo_seguro), cadastro[k].tipo_seguro, sizeof(cadastro[k].tipo_seguro));
    
    fprintf(saida, "%d", contaCharBuffer(buffer) + INC);
    
    //posiciona os '#'
    bool flag = true;
    for(int l = 0; l < TAM_REG_ENTRADA; l++){
        
        if(buffer[l] != '\0' && buffer[l-1] == '\0'){
            fputc('#', saida);
        }
        
        if(buffer[l] != '\0'){
            fputc(buffer[l], saida);
        }
        
        if(flag){
            fseek(saida, -2, 1);
            fputc(buffer[l], saida);
            flag = false;
        }
    }
    
    fclose(inserido);
    fclose(saida);
}

//função para ler o arquivo de entrada e passar os registros para memória (struct)
void carregaArquivos(FILE* arquivo_insere, FILE* arquivo_remove, struct cadastro *cadastro, struct remocao *remocao, int quant_registros_insere, int quant_registros_remove){
    
    //salva o registro do arquivo para memória principal
    char buffer_insere[TAM_REG_ENTRADA];
    
    //salva o registro do arquivo para memória principal
    char buffer_remove[TAM_REG_REMOVE];
    
    //percorre as posições da struct
    int j = 0;
    
    //percorre as posições do buffer_insere
    int i = 0;
    
    //garante que arquivo esteja no começo
    rewind(arquivo_insere);
    
    //limpa a struct preenchendo tudo '\0'
    memset(cadastro,(char)'\0',sizeof(struct cadastro));
    
    //enquanto houver registros
    while (j < quant_registros_insere){
        
        //le o registro do arquivo e passa para memória principal para ser manipulado
        fgets(buffer_insere,TAM_REG_ENTRADA+1,arquivo_insere);
        
        strcpy(cadastro[j].cod_segurado, &buffer_insere[0]);
        strcpy(cadastro[j].nome_segurado, &buffer_insere[4]);
        strcpy(cadastro[j].seguradora, &buffer_insere[54]);
        strcpy(cadastro[j].tipo_seguro, &buffer_insere[104]);
        
        i = 0;
        j++;
    }
    
    j = 0;
    
    //enquanto houver registros
    while (j < quant_registros_remove){
        
        //le o registro do arquivo e passa para memória principal para ser manipulado
        fread(&buffer_remove,sizeof(char),TAM_REG_REMOVE,arquivo_remove);
        
        strcpy(remocao[j].cod_segurado, &buffer_remove[0]);
        
        j++;
    }
}
