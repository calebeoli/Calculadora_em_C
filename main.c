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



int main() {
    printf("Posfixa: %s\n", getFormaInFixa("sen(45) ^2 + 0,5"));
    return 0;
}

char *getFormaInFixa(char *Str) {
    static char posFixa[512];
    char operadores[512][16]; // Agora cada operador pode ser uma string (para funções)
    int topo = -1, j = 0;
    int i = 0;

    while (Str[i] != '\0') {
        if (Str[i] == ' ') {
            i++;
            continue;
        }

        // Lê números (mais de um dígito)
        if (Str[i] >= '0' && Str[i] <= '9') {
            while (Str[i] >= '0' && Str[i] <= '9') {
                posFixa[j++] = Str[i++];
            }
            posFixa[j++] = ' ';
            continue;
        }

        // Lê funções (ex: cos, sin, log)
        if ((Str[i] >= 'a' && Str[i] <= 'z') || (Str[i] >= 'A' && Str[i] <= 'Z')) {
            int k = 0;
            char func[16] = {0};
            while ((Str[i] >= 'a' && Str[i] <= 'z') || (Str[i] >= 'A' && Str[i] <= 'Z')) {
                func[k++] = Str[i++];
            }
            func[k] = '\0';
            strcpy(operadores[++topo], func); // Empilha o nome da função
            continue;
        }

        // Operadores aritméticos (agora inclui ^)
        if (Str[i] == '+' || Str[i] == '-' || Str[i] == '*' || Str[i] == '/' || Str[i] == '^') {
            while (topo >= 0 && operadores[topo][0] != '(' &&
                (
                    // Prioridade de ^ (associativo à direita)
                    ((operadores[topo][0] == '^') && (Str[i] != '^')) ||
                    // Prioridade de * e /
                    ((operadores[topo][0] == '*' || operadores[topo][0] == '/') && (Str[i] != '^' && Str[i] != '*' && Str[i] != '/')) ||
                    // Prioridade de + e -
                    ((operadores[topo][0] == '+' || operadores[topo][0] == '-') && (Str[i] == '+' || Str[i] == '-'))
                )
            ) {
                int k = 0;
                while (operadores[topo][k] != '\0') posFixa[j++] = operadores[topo][k++];
                posFixa[j++] = ' ';
                topo--;
            }
            operadores[++topo][0] = Str[i];
            operadores[topo][1] = '\0';
            i++;
            continue;
        }

        // Parêntese de abertura
        if (Str[i] == '(') {
            operadores[++topo][0] = '(';
            operadores[topo][1] = '\0';
            i++;
            continue;
        }

        // Parêntese de fechamento
        if (Str[i] == ')') {
            while (topo >= 0 && operadores[topo][0] != '(') {
                int k = 0;
                while (operadores[topo][k] != '\0') posFixa[j++] = operadores[topo][k++];
                posFixa[j++] = ' ';
                topo--;
            }
            if (topo >= 0 && operadores[topo][0] == '(') topo--; // Remove '('

            // Se o topo agora for uma função, desempilha ela também
            if (topo >= 0 && ((operadores[topo][0] >= 'a' && operadores[topo][0] <= 'z') ||
                              (operadores[topo][0] >= 'A' && operadores[topo][0] <= 'Z'))) {
                int k = 0;
                while (operadores[topo][k] != '\0') posFixa[j++] = operadores[topo][k++];
                posFixa[j++] = ' ';
                topo--;
            }
            i++;
            continue;
        }

        // Se não reconheceu, só avança
        i++;
    }

    // Desempilha o que sobrou
    while (topo >= 0) {
        int k = 0;
        while (operadores[topo][k] != '\0') posFixa[j++] = operadores[topo][k++];
        posFixa[j++] = ' ';
        topo--;
    }
    if (j > 0) posFixa[j-1] = '\0';
    else posFixa[0] = '\0';
    return posFixa;
}
