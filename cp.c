#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    FILE *arquivoEnt, *arquivoSai;
    char numeros[20];

    arquivoEnt = fopen("arquivo.txt", "r");
    arquivoSai = fopen("arquivosaida.txt", "w");
    
    if (arquivoEnt == NULL) {
        return 1;
    }

    

    while (fgets(numeros, sizeof(numeros), arquivoEnt) != NULL) {
        for (int i = 0; i < strlen(numeros); i++) {
            if (numeros[i] >= '0' && numeros[i] <= '9') {
                printf("%i", numeros[i] - '0');
            }
        }
        printf("%s", "\n");
        
        for (int i = 0; i < strlen(numeros); i++) {
            if (numeros[i] >= '0' && numeros[i] <= '9') {
                int digito = numeros[i] - '0';    
                int resultado = digito * 2;       
                printf("%i", resultado);
                fprintf(arquivoSai, "%d", resultado);
            }
        }        
        printf("%s", "\n");
        fprintf(arquivoSai, "%s", "\n"); 
    }    
    
    fclose(arquivoEnt);
    fclose(arquivoSai);
}