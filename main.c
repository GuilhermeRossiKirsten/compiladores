#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//analizador lexico
//Estrutura de dados: listas encadeadas

void analise(char *entrada) {
    char *it = entrada;
    int tamanho = strlen(it);
    int pos = 0;
}

int main(void) {

    FILE *file1;
    FILE *file2;
    FILE *file3;


    file1 = fopen("entrada.txt", "r");
    
    if (file1 == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    file2 = fopen("fluxoTok.txt", "w");
    file3 = fopen("tabela.txt", "w");

    char entrada[200];

    fgets(entrada, (sizeof(entrada)/sizeof(char)), file1);


    analize(&entrada);

    return 0;
}




// //Analisador Lexico Inicial 4ECR
// //Referências: apostila de compiladores item 2.1.1
// //Estrutura de dados: listas encadeadas

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// void inicializaAnalise(char *entrada){
//   char *codigo=entrada;
//   int tamanho = strlen(codigo);
//   int pos=0;
// }

// int main(void){
//   FILE *fptr;
//   FILE *fptr2;
//   FILE *fptr3;

//   if((fptr = fopen("entrada.txt","r"))==NULL){
//     printf("Erro na abertura do arquivo\n");
//     exit(1);
//   }
//   fptr2=fopen("fluxoTok.txt","w");
//   fptr3=fopen("tabela.txt","w");

//   char entrada[200];

//   fgets(entrada,200,fptr);

//   inicializaAnalise(entrada);
// }