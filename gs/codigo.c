#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Variáveis globais para armazenar o código fonte, tamanho, posição atual, linha e coluna
char *codigo;
int tamanho, pos;
int linha = 1, coluna = 1;

// Strings com operadores e pontuações válidas
const char *ops = "+-*/=<>!";
const char *pontuacoes = "(){},;[]";

// Estrutura para representar um token
typedef struct {
    int tipo;
    char valor[100];
} Token;

// Estrutura para a tabela de símbolos (lista ligada)
typedef struct lista {
    int tipo;
    char valor[100];
    int linha;
    int coluna;
    struct lista* next;
} Tabela;

Tabela* t = NULL; // Ponteiro para o início da tabela

#define TRUE 1
#define FALSE 0

// Definições dos tipos de tokens
#define TOK_NUM 0
#define TOK_OP 1
#define TOK_PONT 2
#define TOK_ID 3
#define TOK_KEY 4
#define TOK_COMMENT 5
#define TOK_DIRECTIVE 6
#define TOK_LITERAL 7

// Lista de palavras-chave reconhecidas
const char *keywords[] = {
    "int", "return", "for", "if", "else", "main", "printf", "sizeof", "#include"
};
int keywords_count = 9;

// Função para adicionar um novo token à tabela de símbolos
Tabela* add(Tabela* t, Token *tok, int linha, int coluna) {
    Tabela* no = (Tabela*) malloc(sizeof(Tabela)); // Aloca novo nó
    strcpy(no->valor, tok->valor); // Copia valor do token
    no->tipo = tok->tipo; // Copia tipo do token
    no->linha = linha; // Salva linha de ocorrência
    no->coluna = coluna; // Salva coluna de ocorrência
    no->next = t; // Liga ao início da lista
    return no; // Retorna novo início da lista
}

// Inicializa as variáveis globais para análise léxica
void inicializa_analise(char *prog) {
    codigo = prog; 
    tamanho = strlen(codigo);
    pos = 0;
    linha = 1;
    coluna = 1;
}

// Lê o próximo caractere do código fonte, atualizando linha e coluna
char le_caractere(void) {
    if (pos < tamanho) {
        char c = codigo[pos++];
        if (c == '\n') {
            linha++; // Atualiza linha se for quebra de linha
            coluna = 1;
        } else {
            coluna++; // Incrementa coluna
        }
        return c;
    }
    return -1; // Fim do código
}

// Volta um caractere na leitura (caso precise "desler")
void volta_caractere(void) {
    if (pos > 0) {
        pos--;
        coluna--;
        if (codigo[pos] == '\n') {
            linha--;
        }
    }
}

// Verifica se uma string é palavra-chave
int eh_keyword(const char *str) {
    for (int i = 0; i < keywords_count; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

// Função principal do analisador léxico: retorna o próximo token encontrado
Token *proximo_token(Token *tok) {
    char c;
    char buffer[100];
    int bpos = 0;

    c = le_caractere();

    // Ignora espaços em branco
    while (isspace(c)) {
        c = le_caractere();
    }

    if (c == -1) return NULL; // Fim do código

    // Reconhece comentários de linha (//)
    if (c == '/' && codigo[pos] == '/') {
        pos++; // Avança para depois do segundo '/'
        tok->tipo = TOK_COMMENT;
        int vpos = 0;
        char valor[200];

        c = le_caractere();
        while (c != '\n' && c != -1) {
            valor[vpos++] = c; // Armazena comentário
            c = le_caractere();
        }
        valor[vpos] = '\0';
        strcpy(tok->valor, valor);

        t = add(t, tok, linha, coluna); // Adiciona à tabela
        return tok;
    }

    // Reconhece números inteiros
    if (isdigit(c)) {
        tok->tipo = TOK_NUM;
        buffer[bpos++] = c;
        c = le_caractere();
        while (isdigit(c)) {
            buffer[bpos++] = c;
            c = le_caractere();
        }
        volta_caractere(); // Retorna um caractere se não for dígito
        buffer[bpos] = '\0';
        strcpy(tok->valor, buffer);
        t = add(t, tok, linha, coluna);
        return tok;
    }

    // Reconhece identificadores e palavras-chave
    if (isalpha(c) || c == '_') {
        tok->tipo = TOK_ID;
        buffer[bpos++] = c;
        c = le_caractere();
        while (isalnum(c) || c == '_') {
            buffer[bpos++] = c;
            c = le_caractere();
        }
        volta_caractere();
        buffer[bpos] = '\0';
        strcpy(tok->valor, buffer);

        if (eh_keyword(buffer)) {
            tok->tipo = TOK_KEY;
        }
        t = add(t, tok, linha, coluna);
        return tok;
    }

    // Reconhece operadores (simples e duplos, como ==, !=, <=, >=)
    if (strchr(ops, c) != NULL) {
        tok->tipo = TOK_OP;
        buffer[0] = c;
        buffer[1] = '\0';

        // Verifica operadores duplos
        if (codigo[pos] == '=' && (c == '=' || c == '!' || c == '<' || c == '>')) {
            buffer[1] = '=';
            buffer[2] = '\0';
            le_caractere();
        }
        strcpy(tok->valor, buffer);
        t = add(t, tok, linha, coluna);
        return tok;
    }

    // Reconhece pontuações (parênteses, chaves, etc)
    if (strchr(pontuacoes, c) != NULL) {
        tok->tipo = TOK_PONT;
        buffer[0] = c;
        buffer[1] = '\0';
        strcpy(tok->valor, buffer);
        t = add(t, tok, linha, coluna);
        return tok;
    }

    // Reconhece literais de string
    if (c == '\"') {
        tok->tipo = TOK_LITERAL;
        buffer[bpos++] = c;
        c = le_caractere();
        while (c != '\"' && c != -1) {
            buffer[bpos++] = c;
            c = le_caractere();
        }
        buffer[bpos++] = '\"';
        buffer[bpos] = '\0';
    
        strcpy(tok->valor, buffer);
        add(t, tok, linha, coluna);
        return tok;
    }

    // Reconhece diretivas do pré-processador (ex: #include)
    if (c == '#') {
        tok->tipo = TOK_DIRECTIVE;
        int vpos = 0;
        char valor[200];
        valor[vpos++] = c;

        c = le_caractere();
        while (c != '\n' && c != -1) {
            valor[vpos++] = c;
            c = le_caractere();
        }
        valor[vpos] = '\0';
        strcpy(tok->valor, valor);

        t = add(t, tok, linha, coluna);
        return tok;
    }

    return NULL; // Caso não reconheça nenhum token
}

// Imprime o token encontrado em um arquivo de saída
void imprime_token(Token *tok, FILE *fptr2) {
    switch (tok->tipo) {
        case TOK_NUM:
            fprintf(fptr2, "Numero\t\t - Valor: %s\n", tok->valor);
            break;
        case TOK_OP:
            fprintf(fptr2, "Operador\t - Valor: %s\n", tok->valor);
            break;
        case TOK_PONT:
            fprintf(fptr2, "Pontuacao\t - Valor: %s\n", tok->valor);
            break;
        case TOK_ID:
            fprintf(fptr2, "Identificador\t - Valor: %s\n", tok->valor);
            break;
        case TOK_KEY:
            fprintf(fptr2, "Keyword\t\t - Valor: %s\n", tok->valor);
            break;
        case TOK_COMMENT:
            fprintf(fptr2, "Comentario\t - Valor: %s\n", tok->valor);
            break;
        case TOK_DIRECTIVE:
            fprintf(fptr2, "Diretiva\t - Valor: %s\n", tok->valor);
            break;
        case TOK_LITERAL:
            fprintf(fptr2, "Literal\t - Valor: %s\n", tok->valor);
            break;
        default:
            fprintf(fptr2, "Token desconhecido\n");
    }
}

// Imprime a tabela de símbolos em um arquivo
void print_tabela(Tabela *t, FILE *fptr3) {
    Tabela *p;
    int e = 0;
    for (p = t; p != NULL; p = p->next) {
        e++;
        fprintf(fptr3, "\nE%d\n", e);
        switch (p->tipo) {
            case TOK_NUM:
                fprintf(fptr3, "Tok: Num\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            case TOK_OP:
                fprintf(fptr3, "Tok: Op\t\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            case TOK_PONT:
                fprintf(fptr3, "Tok: Pont\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            case TOK_ID:
                fprintf(fptr3, "Tok: Id\t\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            case TOK_KEY:
                fprintf(fptr3, "Tok: Key\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            case TOK_COMMENT:
                fprintf(fptr3, "Tok: Coment\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            case TOK_DIRECTIVE:
                fprintf(fptr3, "Tok: Direct\tLex: %s\tOcor: L%dC%d\n", p->valor, p->linha, p->coluna);
                break;
            default:
                fprintf(fptr3, "Token desconhecido\n");
        }
    }
}

// Função principal
int main(void) {
    FILE *fptr, *fptr2, *fptr3;

    // Abre o arquivo de entrada (código fonte)
    if ((fptr = fopen("entrada.txt", "r")) == NULL) {
        puts("Não foi possível abrir o arquivo\n");
        exit(1);
    }

    // Abre arquivos de saída para tokens e tabela de símbolos
    fptr2 = fopen("saida.txt", "w");
    fptr3 = fopen("tabela.txt", "w");

    printf("Analisador Léxico\n");
    printf("Lendo Código-Fonte...\n");

    // Lê todo o código fonte para uma string
    char entrada[1000] = "";
    char linha_aux[200];

    while (fgets(linha_aux, 200, fptr) != NULL) {
        strcat(entrada, linha_aux);
    }

    Token tok;
    inicializa_analise(entrada);

    // Executa o analisador léxico até o fim do código
    while (proximo_token(&tok) != NULL) {
        imprime_token(&tok, fptr2);
    }

    // Imprime a tabela de símbolos
    print_tabela(t, fptr3);

    // Fecha arquivos
    fclose(fptr);
    fclose(fptr2);
    fclose(fptr3);

    printf("\nAnálise Léxica finalizada com sucesso\n");
    return 0;
}
