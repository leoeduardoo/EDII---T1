#include<stdio.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct livro {
        char codigo[4];
        char nome[50];
        char seguradora[50];
        char tipo_seg[30];
    } vet[8] = {{"001", "Joao", "Maritima", "Automovel"},
                {"002", "Paulo", "Porto Seguro", "Automovel"},
                {"003", "Maria", "Porto Seguro", "Automovel"},
                {"004", "Ana", "Zurich", "Vida"},
                {"005", "Marcos", "Bradesco", "Previdencia"},
                {"006", "Renan", "Zurich", "Residencial"},
                {"007", "Fernando", "Sompo", "Automovel"},
                {"008", "Paulo", "Sompo", "Residencial"}};
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct remove {
        char isbn[4];
    } vet_r[4] = {{"003"},
                  {"001"},
                  {"007"},
                  {"004"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

