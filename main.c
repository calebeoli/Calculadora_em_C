#include <stdio.h>
#include <stdlib.h>

typedef struct Item{
    float dados;
    struct Item *Anterior;
} Item;

typedef struct Pilha {
    int tamanho;
    Item *topo;
}Pilha;


typedef struct{
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

Pilha *CriaPilha();
Item *empilhar(Pilha *P, float dados);
void desempilhar(Pilha *P);
void mostraPilha(Pilha *P);

int main(){
    Pilha *Calculadora = CriaPilha();

    empilhar(Calculadora, 2);
    empilhar(Calculadora, 3);

    mostraPilha(Calculadora);

    // Exemplo de desempilhar
    desempilhar(Calculadora);
    mostraPilha(Calculadora);



    free(Calculadora);

    return 0;
}


Item *empilhar(Pilha *P,float dados){
    Item *I = (Item *)malloc(sizeof(Item));

    if(I == NULL){
        printf("ERRO: Não foi possivel alocar memoria\n");
        return NULL;
    }
    I -> dados = dados;
    I -> Anterior = P->topo;
    P -> topo = I;
    P -> tamanho++;
    return I;
}

Pilha *CriaPilha(){
    Pilha *P = (Pilha *)malloc(sizeof(Pilha));
    if (P == NULL){
        printf("ERRO: Não foi possivel alocar memoria para a lista!\n");        
        return NULL;
    }

    P -> tamanho = 0;
    P -> topo = NULL;

    return P;
}



void desempilhar(Pilha *P){

    if(P -> tamanho == 0){
        printf("A pilha esta vazia");
        return;
    }

    Item *Atual = P->topo;
    P -> topo = Atual -> Anterior;
    free(Atual);
    P-> tamanho--;
}

void mostraPilha(Pilha *P){

    Item *Atual = P -> topo;
    while (Atual != NULL){
        printf("%.2f\n", Atual -> dados);
        Atual = Atual -> Anterior;
    }
}