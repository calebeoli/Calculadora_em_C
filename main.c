#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct
{
    char posFixa[512];
    char inFixa[512];
    float Valor;
} Expressao;

// Expressão na forma pos-fixa, como 3 12 4 + *
// Expressão na forma infixa, como 3 * (12 + 4)
// Valor numérico da expressão
char *getFormaInFixa(char *Str);         // Retorna a forma inFixa de Str (posFixa)
char *getFormaPosFixa(char *Str);        // Retorna a forma posFixa de Str (inFixa)
float getValorPosFixa(char *StrPosFixa); // Calcula o valor de Str (na forma posFixa)
float getValorInFixa(char *StrInFixa);   // Calcula o valor de Str (na forma inFixa)



int main(){
    getFormaInFixa("9 + (5 * (2 + 8 * 4))");

return 0;
}

char *getFormaInFixa(char *Str) {
    static char posFixa[512];
    char operadores[512];
    int topo = -1, j = 0;

    for (int i = 0; Str[i] != '\0'; i++) {
        if (Str[i] == ' ') continue;
        if (Str[i] >= '0' && Str[i] <= '9') {
            posFixa[j++] = Str[i];
            posFixa[j++] = ' ';
        } else if (Str[i] == '+' || Str[i] == '-' || Str[i] == '*' || Str[i] == '/') {
           while (topo >= 0) {
                posFixa[j++] = operadores[topo--];
                posFixa[j++] = ' ';
            }
            operadores[++topo] = Str[i];
        }
    }
    while (topo >= 0) {
        posFixa[j++] = operadores[topo--];
        posFixa[j++] = ' ';
        printf("Parcial: %s\n", posFixa); // imprime a string parcial
    }
    if (j > 0) posFixa[j-1] = '\0';
    else posFixa[0] = '\0';
    return Str;
}
