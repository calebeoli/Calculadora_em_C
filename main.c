#include <stdio.h>
#include <string.h>
#include "expressao.h"

// Função para formatar as expressões da tabela para o padrão com espaços
void format_infix_expression(const char* input, char* output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; ++i) {
        if (strchr("+-*/%^()", input[i])) {
            output[j++] = ' ';
            output[j++] = input[i];
            output[j++] = ' ';
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

int main() {
    // Array com os testes da tabela do PDF 
    char *testesPosFixa[] = {
        "3 4 + 5 *",
        "7 2 * 4 +",
        "8 5 2 4 + * +",
        "6 2 / 3 + 4 *",
        "9 5 2 8 * 4 + * +",
        "2 3 + log 5 /",
        "10 log 3 ^ 2 +",
        "45 60 + 30 cos *",
        "45 sen 2 ^ 0.5 +"
    };

    char *testesInFixaRaw[] = {
        "(3+4)*5",
        "7*2+4",
        "8+(5*(2+4))",
        "(6/2+3)*4",
        "9+(5*(2+8*4))",
        "log(2+3)/5",
        "(log(10)^3)+2",
        "(45+60)*cos(30)",
        "sen(45)^2+0.5"
    };

    int num_testes = sizeof(testesPosFixa) / sizeof(testesPosFixa[0]);

    printf("--- AVALIADOR DE EXPRESSÕES NUMÉRICAS ---\n");

    for (int i = 0; i < num_testes; i++) {
        printf("\n----------------------------------------\n");
        printf("Teste %d\n", i + 1);

        // Formata a expressão infixa para ter espaços
        char testeInFixaFmt[512];
        format_infix_expression(testesInFixaRaw[i], testeInFixaFmt);

        // 1. Pós-fixa -> Infixa
        char* infixa_result = getFormaInFixa(testesPosFixa[i]);
        printf("Pós-fixa para Infixa: %s -> %s\n", testesPosFixa[i], infixa_result);

        // 2. Infixa -> Pós-fixa
        char* posfixa_result = getFormaPosFixa(testeInFixaFmt);
        printf("Infixa para Pós-fixa: %s -> %s\n", testesInFixaRaw[i], posfixa_result);
        
        // 3. Avaliar Pós-fixa
        float valor_posfixa = getValorPosFixa(testesPosFixa[i]);
        printf("Valor (Pós-fixa): %.2f\n", valor_posfixa);

        // 4. Avaliar Infixa
        float valor_infixa = getValorInFixa(testeInFixaFmt);
        printf("Valor (Infixa):   %.2f\n", valor_infixa);
    }
    printf("\n----------------------------------------\n");

    return 0;
}
