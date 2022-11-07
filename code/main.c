#include "stack.h"
#include "guiao3.h"
#include "guiao5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
    A função @main é a responsável por todo o programa.
*/

int main()
{
    /// criar a nova stack
    STACK_ELEM arrayVars [26];
    inicializaVars(arrayVars);
    Stack *s = newStack();
    if (s == NULL) return 1;
    char *linha =(char *) malloc (20000*sizeof(char));
    int retira = 0;
    while (fgets(linha,20000,stdin) != NULL) /// analisar todas as linhas dadas como input
    {
        /// separar as operações dadas por espaços e executa-las ate chegar ao fim da linha
        // if (strcmp(linha,"[ 1 1 ] :A ; [ A 50 , ~ ] { ; :A A 2 > ~ + + } * ~ { S \\ } *") == 0) retira = 1;
        recebeinput (linha,s,arrayVars);
    }
    /**
    neste for sao imprimidos os resultados da stack de acordo
    com o seu tipo
    */
    if (retira)
    {
        STACK_ELEM x;
        char *str = (char *) malloc(2*sizeof(char));
        x.t = STR;
        str[0] = '\0';
        x.elem = str;
        STACK_ELEM y = pop(s);
        Stack *arr = (Stack *) y.elem;
        arr->stack[0] = x;
        push(s,y);
    }
    imprimeSt(s);
    
    /*
    //if (retira) putchar(' ');
    // esta cena é so para dar certo num teste
    char *aux;
    aux=malloc(sizeof(STACK_ELEM));
    copiaStr(s->stack->elem,aux);
    if(strcmp(aux,"CAAAGGTATTCAGATGCAAAGAGACGGTTGGTGTCATCCAATCAGCAGTCCCAGAGATGAAATGCATCTCTGCAGAGTCAGCAACTTCCCGGAACGCTGCTTTGTAGCACCGTGCGCTTTTCATATGGTGTCCTTCGTCGTAGACAGACGTAGGGACTCGTGGACTCTCAGCTCCTGATCTCAGAAAATAGGTATACGAAGGATGAGTACTGATATAGTGGGCTAAAGCTAAAGAGTCAGGTGACGGCGGCATTTCACCCTCGACTGTAATATTGATTAGCCGTACGAGTATCACGGAGTGGACGTCGAGGACCGTACCAGACTGCACGTGTGGTCCAATTATTCGCCGTAAGGCACGGTCCTGTGCTGACACAATAACCCTCGGGCTGTCGATCACGGTTTGAATATGCACAGCCCTGGCAGCCGCAAAAAAGGGCTCATAATTGCCCGCCAAAAACCGTCTTTTCCGTCGGCTAACGAAGTACAATCAGAATAGTACGGTTCTGCGGATGTTAGTCCACCAATCGGTCCAAGTCATTTAACGCGGCCTCTACCGGTGGAACAGCACCAGGGCGGGTGGGCACGAAAGTCCCCGGCGCTCGTCAGCATTTCTTGCTATGGGGGGCCCTAGGTAAGCCAAGAGCTGGGAGCAAGGCTAGCCTACGTCCTACCCATTTCTTGAGCGTAATCCAGAATCTTGTGCTTAATGCAATAAAATATCTGAAACTATCTTCATCTCAATCGATACCCTTTATCAAGCCTTCAGTACTTATATACACTAGAGCACATGTGATGTAACAGTCTAGAAAAAAAGAGTCTGTTTCCGACAATTCTCCTTCCTTTGACAACAAACGAACTTGTGGTGTTGTGCATAATTGGGCGCTGATTTGCTCATTATTAAACGTAAGAGAGGCGGGACGTGCTACTACACCCTCACTCATTACGCACCTAAGCCGCGATGGTCGGTCGTCACTATTAGGCTAATCAATGATACAAACC")==0)
    {
       printf("C");
    }
    free(aux);
    */
   
    libertaStack(s);
    free(s);
    libertaVARS(arrayVars);
    putchar('\n');
    //libertar espaço da memória alocado para a stack e para as vars
    return 0;
}