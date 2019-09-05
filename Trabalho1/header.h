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
#define TAM_COD_SEGURADO 3
#define TAM_NOME_SEGURADO 50
#define TAM_SEGURADORA 50
#define TAM_TIPO_SEGURO 30
#define CHAR_DIVISOR '#'

//struct para armazenar as inforções lidas do arquivo
struct cadastro {
    char cod_segurado[3];
    char nome_segurado[50];
    char seguradora[50];
    char tipo_seguro[30];
    char tamanho;
};

//faz o dump do arquivo
void dump (void);

//insere registros no arquivo de saida
void insere(struct cadastro cadastro[]);

//função para ler o arquivo de entrada e passar os registros para memória (struct)
void leEntrada(FILE* arquivo, struct cadastro cadastro[]);

//abre o arquivo com a flag desejada
void abreArquivo (FILE** arquivo, char *flag, char *nome_arquivo);

//conta quantos registros tem no arquivo para serem inseridos
int contaRegistrosEntrada(void);

#endif /* header_h */
