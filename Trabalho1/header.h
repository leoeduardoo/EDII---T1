//
//  header.h
//  Trabalho1
//
//  Created by Léo Silva on 04/09/19.
//  Copyright © 2019 Léo Silva. All rights reserved.
//

#ifndef header_h
#define header_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#define TAM_REG_ENTRADA 134
#define TAM_REG_REMOVE 4
#define TAM_COD_SEGURADO 4
#define TAM_NOME_SEGURADO 50
#define TAM_SEGURADORA 50
#define TAM_TIPO_SEGURO 30
#define INC 5 //2 inteiros + 4 #
#define CHAR_DIVISOR '#'

//lista de espacos disponiveis
struct lista_espacos {
    int offset;
    struct lista_espacos *prox;
};

//struct para armazenar as inforções lidas do arquivo de inserção
struct cadastro {
    char cod_segurado[4];
    char nome_segurado[51];
    char seguradora[51];
    char tipo_seguro[31];
    //char tamanho;
    //int int_tamanho;
};

//struct para armazenar as inforções lidas do arquivo de remoção
struct remocao {
    char cod_segurado[4];
    //char tamanho;
    //int int_tamanho;
};

//faz o dump do arquivo
void dump (void);

//insere registros no arquivo de saida
void insereRegistro(struct cadastro *cadastro, struct lista_espacos* lista);

//função para ler o arquivo de entrada e passar os registros para memória (struct)
void carregaArquivos(FILE* arquivo_insere, FILE* arquivo_remove, struct cadastro *cadastro, struct remocao *remocao, int quant_registros_insere, int quant_registros_remove);

//função para ler o arquivo de remoção e passar os registros para memória (struct)
void leRemove(FILE* arquivo, struct remocao *remocao);

//abre o arquivo com a flag desejada
void abreArquivo (FILE** arquivo, char *flag, char *nome_arquivo);

//conta quantos registros tem no arquivo para serem inseridos
int contaRegistrosEntrada(void);

//conta quantos registros tem no arquivo para serem removidos
int contaRegistrosRemover(void);

//remove registros no arquivo de saida
void removeRegistro(struct remocao *remocao, struct cadastro *cadastro, struct lista_espacos* lista);

//conta somente os caracteres no buffer
int contaCharBuffer(char * buffer);

//atualiza a lista de espaços disponíveis
void atualizaListaDisponiveis(struct lista_espacos* lista, int quant_seek);

//compacta arquivo
void compacta(FILE * arquivo);

//verifica se já carregou os arquivos para a struct
bool arquivoCarregado(struct remocao *remocao, struct cadastro *cadastro, int p);

//preenche a lista de espaços disponíveis
void preencheListaDisponiveis(struct lista_espacos* lista);

//retorna o offset a ser inserido no arquivo saida.bin quando se remove um arquivo
int offsetListaDisponiveis(struct lista_espacos *lista);

//inclui o novo nó na lista de disponíveis
int incluiNoListaDisponiveis(struct lista_espacos *lista, int quant_seek);

#endif /* header_h */
