#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Definição de M_PI para portabilidade, caso não exista em math.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- FUNÇÕES AUXILIARES ---

// Retorna a precedência de um operador
int precedencia(char op) {
    switch (op) {
        case '^': return 3;
        case '*':
        case '/':
        case '%': return 2;
        case '+':
        case '-': return 1;
        default: return 0; // Para parênteses
    }
}

// Verifica se o token é um operador unário (função)
int is_function(const char *token) {
    return strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
           strcmp(token, "tg") == 0  || strcmp(token, "log") == 0 ||
           strcmp(token, "raiz") == 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES PRINCIPAIS ---

/**
 * Converte uma expressão Infixa para Pós-fixa.
 * Utiliza o algoritmo Shunting-yard.
 */
char *getFormaPosFixa(char *StrInFixa) {
    static char posFixa[512];
    char pilha[512][16]; // Pilha para operadores e funções
    int topo = -1;
    int j = 0;
    char *token;

    // Uma cópia é criada para não modificar a string original com strtok
    char inFixaCopy[512];
    strcpy(inFixaCopy, StrInFixa);
    
    token = strtok(inFixaCopy, " ");

    while (token != NULL) {
        // Se for um número, adiciona à saída
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1])) || (token[0] == '-' && isdigit(token[1]))) {
            j += sprintf(posFixa + j, "%s ", token);
        }
        // Se for uma função, empilha
        else if (is_function(token)) {
            strcpy(pilha[++topo], token);
        }
        // Se for um operador
        else if (strchr("+-*/%^", token[0])) {
            while (topo > -1 && precedencia(pilha[topo][0]) >= precedencia(token[0])) {
                j += sprintf(posFixa + j, "%s ", pilha[topo--]);
            }
            strcpy(pilha[++topo], token);
        }
        // Se for parêntese de abertura
        else if (token[0] == '(') {
            strcpy(pilha[++topo], token);
        }
        // Se for parêntese de fechamento
        else if (token[0] == ')') {
            while (topo > -1 && pilha[topo][0] != '(') {
                j += sprintf(posFixa + j, "%s ", pilha[topo--]);
            }
            topo--; // Descarta o '('
            if (topo > -1 && is_function(pilha[topo])) {
                j += sprintf(posFixa + j, "%s ", pilha[topo--]);
            }
        }
        token = strtok(NULL, " ");
    }

    // Desempilha operadores restantes
    while (topo > -1) {
        j += sprintf(posFixa + j, "%s ", pilha[topo--]);
    }

    if (j > 0) posFixa[j - 1] = '\0'; // Remove o último espaço
    else posFixa[0] = '\0';

    return posFixa;
}

/**
 * Converte uma expressão Pós-fixa para Infixa.
 */
char *getFormaInFixa(char *StrPosFixa) {
    static char inFixa[512];
    char pilha[256][512];
    int topo = -1;
    char *token;
    
    char posFixaCopy[512];
    strcpy(posFixaCopy, StrPosFixa);

    token = strtok(posFixaCopy, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (strlen(token) > 1 && token[0] == '-')) { // É um número
            strcpy(pilha[++topo], token);
        } else if (is_function(token)) { // É operador unário 
            if (topo < 0) return "Erro: Faltando operando para função";
            char operando[512];
            strcpy(operando, pilha[topo--]);
            sprintf(pilha[++topo], "%s(%s)", token, operando);
        } else { // É operador binário 
            if (topo < 1) return "Erro: Faltando operandos para operador";
            char op2[512], op1[512];
            strcpy(op2, pilha[topo--]);
            strcpy(op1, pilha[topo--]);
            sprintf(pilha[++topo], "(%s %s %s)", op1, token, op2);
        }
        token = strtok(NULL, " ");
    }

    if (topo == 0) {
        strcpy(inFixa, pilha[0]);
        return inFixa;
    }

    return "Erro: Expressão Pós-fixa inválida";
}


/**
 * Calcula o valor de uma expressão Pós-fixa.
 */
float getValorPosFixa(char *StrPosFixa) {
    float pilha[256];
    int topo = -1;
    char *token;

    char posFixaCopy[512];
    strcpy(posFixaCopy, StrPosFixa);

    token = strtok(posFixaCopy, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1])) || (strlen(token) > 1 && token[0] == '-')) {
            pilha[++topo] = atof(token);
        } else {
            // Operadores unários
            if (is_function(token)) {
                if (topo < 0) return NAN; // Erro
                float operando = pilha[topo--];
                if (strcmp(token, "sen") == 0) pilha[++topo] = sin(operando * M_PI / 180.0);
                else if (strcmp(token, "cos") == 0) pilha[++topo] = cos(operando * M_PI / 180.0);
                else if (strcmp(token, "tg") == 0) pilha[++topo] = tan(operando * M_PI / 180.0);
                else if (strcmp(token, "log") == 0) pilha[++topo] = log10(operando);
                else if (strcmp(token, "raiz") == 0) pilha[++topo] = sqrt(operando);
            }
            // Operadores binários
            else {
                if (topo < 1) return NAN; // Erro
                float op2 = pilha[topo--];
                float op1 = pilha[topo--];

                if (strcmp(token, "+") == 0) pilha[++topo] = op1 + op2;
                else if (strcmp(token, "-") == 0) pilha[++topo] = op1 - op2;
                else if (strcmp(token, "*") == 0) pilha[++topo] = op1 * op2;
                else if (strcmp(token, "/") == 0) pilha[++topo] = op1 / op2;
                else if (strcmp(token, "%") == 0) pilha[++topo] = fmod(op1, op2);
                else if (strcmp(token, "^") == 0) pilha[++topo] = pow(op1, op2);
            }
        }
        token = strtok(NULL, " ");
    }
    return (topo == 0) ? pilha[topo] : NAN; // Retorna o resultado ou NAN em caso de erro
}

/**
 * Calcula o valor de uma expressão Infixa.
 */
float getValorInFixa(char *StrInFixa) {
    // Reutiliza as funções já criadas: Infixa -> Pós-fixa -> Valor
    char *posFixa = getFormaPosFixa(StrInFixa);
    return getValorPosFixa(posFixa);
}
