//Analisador Lexico Inicial 4ECR
//Referências: apostila de compiladores item 2.1.1
//Estrutura de dados: listas encadeadas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOK_NUM 0
#define TOK_OP 1
#define TOK_PONT 2
#define PARESQ 3
#define PARDIR 4

char *codigo;
int pos;
int tamanho;

typedef struct{
    int tipo; //Token
    int valor; //Lexema
}Token;

struct lista{
    int tipo; //Token
    int valor; //Lexema
    int linha;  //Ocorrências
    int coluna;
    struct lista* next;
};
typedef struct lista Tabela;

Tabela* t;

Tabela* cria(void){
    return NULL;
}

Tabela* add(Tabela* t,Token *tok,int linha,int coluna){
    Tabela* no = (Tabela*) malloc(sizeof(Tabela));
    no->tipo = tok->tipo;
    no->valor = tok->valor;
    no->linha = linha;
    no->coluna = coluna;
    no->next = t;
    return no;
}

char le_caractere(void){
    char c;

    if(pos<tamanho){
        c=codigo[pos];
        pos++;
    }else
        c=-1;

    return c;
}

void inicializaAnalise(char *entrada){
  codigo=entrada;
  tamanho = strlen(codigo);
  pos=0;
}

Token *proximo_token(Token *tok){
    char c;
    char valor[200];
    int vpos=0;
    char *ops="+-*/";

    c=le_caractere();

    while(isspace(c)){
        c=le_caractere();
    }

    if(isdigit(c)){
        tok->tipo = TOK_NUM;
        valor[vpos++]=c;
        c=le_caractere();
        while(isdigit(c)){
            valor[vpos++]=c;
            c=le_caractere();
        }
        valor[vpos]='\0';
        tok->valor = atoi(valor);
        t = add(t,tok,2,3);

    }else if(strchr(ops,c)!=NULL){
        tok->tipo = TOK_OP;
        tok->valor = c;
        t = add(t,tok,2,3);

    }else if(c=='(' || c==')'){
        tok->tipo = TOK_PONT;
        tok->valor = (c=='('?PARESQ:PARDIR);
        t = add(t,tok,2,3);

    }else
        return NULL;

    return tok;
}

void imprime_token(Token *tok,FILE *fptr2){

}

void imprime_tabela(Tabela* t,FILE *fptr3){

}

int main(void){
  FILE *fptr;
  FILE *fptr2;
  FILE *fptr3;

  if((fptr = fopen("entrada.txt","r"))==NULL){
    printf("Erro na abertura do arquivo\n");
    exit(1);
  }
  fptr2=fopen("fluxoTok.txt","w");
  fptr3=fopen("tabela.txt","w");

  char entrada[200];
  Token tok;

  t = cria();

  fgets(entrada,200,fptr);

  inicializaAnalise(entrada);

  while(proximo_token(&tok)!=NULL){
    imprime_token(&tok,fptr2);
    imprime_tabela(t,fptr3);
  }

  //t = add(t,tok,2,3);

  //t = add(t,tok,2,3);
}