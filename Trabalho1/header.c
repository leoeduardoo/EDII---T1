//
//  header.c
//  Trabalho1
//
//  Created by Léo Silva on 04/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#include "header.h"

//retorna o offset a ser inserido no arquivo saida.bin quando se remove um arquivo
int offsetListaDisponiveis(struct lista_espacos *lista){
    
    FILE *saida;
    
    abreArquivo(&saida, "r+", "saida.bin");
    
    struct lista_espacos *novo_no = (struct lista_espacos*)malloc(sizeof(struct lista_espacos));
    
    if(novo_no == NULL){
        fprintf(stderr, "Nao foi possivel criar o no auxiliar.\n");
        exit(-1);
    }
    
    //verifica se a lista está vazia
    if(lista->prox == NULL){
        printf("Lista vazia.\n");
    }else{
        //percorre a lista até prox->prox ser nulo
        struct lista_espacos *atual = lista;
        while(true){
            if(atual->prox->prox == NULL)
            {
                //retorna o offset para ser escrito
                return atual->offset;
            }
            atual = atual->prox;
        };
    }
    
    return 0;
}

int incluiNoListaDisponiveis(struct lista_espacos *lista, int quant_seek){
    
    struct lista_espacos *novo_no = (struct lista_espacos*)malloc(sizeof(struct lista_espacos));
    
    if(novo_no == NULL){
        fprintf(stderr, "Nao foi possivel criar o no auxiliar.\n");
        exit(-1);
    }
    
    novo_no->offset = quant_seek;
    novo_no->prox = NULL;
    
    //verifica se a lista está vazia
    if(lista->prox == NULL){
        lista->prox = novo_no;
    }else{
        //insere no fim
        struct lista_espacos *atual = lista;
        while (true) {
            if(atual->prox == NULL)
            {
                atual->prox = novo_no;
                break;
            }
            atual = atual->prox;
        };
    }
    return 0;
}

//preenche a lista de espaços disponíveis
void preencheListaDisponiveis(struct lista_espacos* lista){
    
    FILE *saida;
    abreArquivo(&saida, "r+", "saida.bin");
    
    rewind(saida);
    
    //armazena os offsets dos espacos disponveis
    char buffer[4];
    
    //limpa a struct preenchendo tudo '\0'
    memset(buffer,(char)'\0',sizeof(buffer));
    
    //quantidade de seek entre o inicio do arquivo e o proximo espaco disponivel
    int quant_seek = 0;
    
    char caractere = fgetc(saida);
    
    if(!(caractere == EOF)){
        
        //le de saida.bin e escreve em buffer
        while(buffer[0] != '-' && buffer[1] != '-' && buffer[2] != '-' && buffer[3] != '-'){
            
            //limpa a struct preenchendo tudo '\0'
            memset(buffer,(char)'\0',sizeof(buffer));
            
            //se o caractere lido for digito ou '-' insere no buffer
            for (int i = 0; i < 4; i++){
                if (isdigit(caractere) || caractere == '-'){
                    buffer[i] = caractere;
                }
                else{
                    buffer[i] = '\0';
                }
                caractere = fgetc(saida);
            }
            
            rewind(saida);
            
            sscanf(buffer, "%d", &quant_seek);
            
            //+4 por conta dos dois dígitos do tamanho e 2 *
            fseek(saida, quant_seek+4, 1);
            caractere = fgetc(saida);
            
            atualizaListaDisponiveis(lista, quant_seek);
        }
    }
    //atualizaListaDisponiveis(lista, quant_seek);
}

//verifica se já carregou os arquivos para a struct
bool arquivoCarregado(struct remocao *remocao, struct cadastro *cadastro, int p){
    //cadastro -> p = 1
    //remocao -> p = 2
    
    if (p == 1){
        if ((void*) strlen(cadastro[0].cod_segurado) == NULL){
            printf("Carregue os arquivos de entrada primeiro (opcao 5).\n");
            return false;
        }else{
            return true;
        }
    }else{
        if ((void*) strlen(remocao[0].cod_segurado) == NULL){
            printf("Carregue os arquivos de entrada primeiro (opcao 5).\n");
            return false;
        }else{
            return true;
        }
    }
}

//compacta arquivo
void compacta(){
    
    FILE *saida;
    FILE *arquivo_aux;
    
    abreArquivo(&saida, "r+", "saida.bin");
    abreArquivo(&arquivo_aux, "w+", "saida_aux.bin");
    
    rewind(saida);
    rewind(arquivo_aux);
    
    //armazena o caractere lido no arquivo
    char caractere = '\0';
    
    int i = 0;
    
    //cabeçalho inicial
    char cabecalho[] = "0-1";
    
    //verifica se o arquivo está vazio para inserir o cabeçalho
    
    fputc(cabecalho[0], arquivo_aux);
    fputc(cabecalho[1], arquivo_aux);
    fputc(cabecalho[2], arquivo_aux);
    fputc('\n', arquivo_aux);
    
    //pula o cabeçalho e o tamanho do primeiro registro
    fseek(saida, 4, 1);
    
    //armazena o tamanho do registro
    int tam = 0;
    
    while (caractere != EOF){
        
        fscanf(saida, "%2d", &tam);
        caractere = fgetc(saida);
        
        if(caractere != EOF){
            //se caractere for * significa que o registor foi excluido e deve pular tam - 3 (dois digitos do tamanho e 1 do * lido)
            if(caractere == '*'){
                fseek(saida, tam-3, 1);
            }
            else{
                fprintf(arquivo_aux, "%d", tam);
                
                i = 0;
                //ja leu 3 caracteres (dois digitos do tamanho e 1 do * lido) basta escrever o restante
                while(i < tam-3){
                    fputc(caractere, arquivo_aux);
                    caractere = fgetc(saida);
                    i++;
                }
                fputc(caractere, arquivo_aux);
            }
        }
    }
    
    rewind(arquivo_aux);
    
    //limpa o arquivo saida.bin
    fclose(fopen("/Users/leo/Desktop/Faculdade/ED2 - 2019/EDII-T1/Trabalho1/saida.bin", "w"));
    rewind(saida);
    
    //copia todos os caracteres de arquivo_aux para saida.bin
    while((caractere = fgetc(arquivo_aux)) != EOF ){
        fputc(caractere, saida);
    }
    
    int status = remove("/Users/leo/Desktop/Faculdade/ED2 - 2019/EDII-T1/Trabalho1/saida_aux.bin");
    
    if (status != 0){
        printf("Nao foi possivel deletar o arquivo auxiliar de compressao.\n");
        perror("O seguinte erro ocorrou: \n");
    }
    
    fclose(saida);
    fclose(arquivo_aux);
}

//atualiza a lista de espaços disponíveis
void atualizaListaDisponiveis(struct lista_espacos* lista, int quant_seek){
    
    //se lista estiver vazia
    if(lista->offset == 0){
        //se quant_seek == 0 então o arquivo saida.bin está vazio e deve ser iniciada a lista com -1
        if (quant_seek == 0){
            lista->offset = -1;
        }else{
            //se quant_seek != 0 então a lista está vazia porque o programa inicializou e está sendo preenchida
            lista->offset = quant_seek;
        }
    }else{
        //cria novo nó para inserir na lista
        struct lista_espacos lista_aux = *lista;
        
        lista->prox = malloc(sizeof(struct lista_espacos));
        
        lista->prox->prox = lista_aux.prox;
        lista->prox->offset = lista_aux.offset;
        
        lista->offset = quant_seek;
    }
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
    
    //armazena o cod_segurado anterior para comparar se houve um dado novo no buffer_saida
    char buffer_saida_anterior[4];
    
    //limpa o buffer preenchendo tudo '\0'
    memset(buffer_saida,(char)'\0',sizeof(buffer_saida));
    
    //limpa o buffer preenchendo tudo '\0'
    memset(buffer_saida_anterior,(char)'\0',sizeof(buffer_saida_anterior));
    
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
        
        //verifica se o que tem que ser removido (remocao[i].cod_segurado) é igual ao que foi lido do arquivo saida.bin (buffer_saida) e caso não efetua uma nova leitura do arquivo saida.bin
        for(int cont = 0; cont < sizeof(buffer_saida); cont++){
            buffer_saida_anterior[cont] = buffer_saida[cont];
        }
        if (buffer_saida[0] != remocao[i].cod_segurado[0]){
            fread(buffer_saida, sizeof(buffer_saida), 1, saida);
            //se buffer anterior for igual a buffer então não tem mais o que ler de saida.bin
            if (strcmp(buffer_saida_anterior, buffer_saida) == 0){
                break;
            }
        }
        else{
            if (buffer_saida[1] != remocao[i].cod_segurado[1]){
                fread(buffer_saida, sizeof(buffer_saida), 1, saida);
                //se buffer anterior for igual a buffer então não tem mais o que ler de saida.bin
                if (strcmp(buffer_saida_anterior, buffer_saida) == 0){
                    break;
                }
            }
            else{
                if (buffer_saida[2] != remocao[i].cod_segurado[2]){
                    fread(buffer_saida, sizeof(buffer_saida), 1, saida);
                    //se buffer anterior for igual a buffer então não tem mais o que ler de saida.bin
                    if (strcmp(buffer_saida_anterior, buffer_saida) == 0){
                        break;
                    }
                }
                else{
                    flag_achou= true;
                }
            }
        }
        
        k++;
    }
    
    if(flag_achou){
        
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
        incluiNoListaDisponiveis(lista, quant_seek+4);
        
        fputc(caractere_removido, saida);
        fputc(caractere_removido, saida);
        
        //insere em saida.bin o offset do proximo espaço disponível
        fprintf(saida, "%d", offsetListaDisponiveis(lista));
        
        //escreve em remocao.bin o cod_segurado para evitar que haja duplicidade posteriormente
        fwrite(remocao[i].cod_segurado, sizeof(remocao[i].cod_segurado), 1, removido);
        
        rewind(saida);
        
        //4 para pular os 3 digitos do offset + '\n'
        int cabecalho = 4 + quant_seek;
        
        fprintf(saida, "%03d", cabecalho);
    }
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
