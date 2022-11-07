#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

/**
    A função @retira, retira o elemento no topo da stack.
    Ex: 1 2 3 ; -> 12
*/

int retira (Stack *s,char *operacao)
{
    if (s->sp<1 || strcmp (operacao,";")) return 0;
    STACK_ELEM x = pop (s);
    if (x.t == ARR) libertaStack((Stack *) x.elem);/// libertar conteudo do array.
    free(x.elem); 
    ///liberta o espaço utilizado pelo último elemento.
    return 1;
}

/**
    A função @duplicar põe no topo da stack um elemento com o mesmo valor, mas apontador diferente do anterior.
    Desta forma podemos executar operações sobre um deles sem afetar o outro.
    EX: 1 2 _ ) -> 123
*/

int duplicar (Stack *s,char *operacao)
{
    if (s->sp<1 || strcmp (operacao,"_")) return 0;
    STACK_ELEM x = pop (s);
    push(s,x);
    STACK_ELEM y = duplicaElemento(x);
    /// criar um void pointer diferente mas com o mesmo conteudo e insere na stack.
    push(s,y);
    return 1;
}

/**
    A função @trocar troca a posição dos 2 elementos no topo da stack.
    Ex: 1 2 \ -> 21
*/

int trocar(Stack *s,char *operacao)
{
    if (s->sp<2 || strcmp (operacao,"\\")) return 0;
    STACK_ELEM x = pop (s); 
    STACK_ELEM y = pop (s);
    push(s, x);
    push(s, y);
    // por fazer e testar 1 2 3 \ -> 1 3 2
    return 1;
}

/**
    A função @rodar roda os 3 elementos no topo da stack.
    Ex: 1 2 3 @ -> 231
*/

int rodar(Stack *s,char  *operacao)
{
    if (strcmp (operacao,"@")) return 0;
    STACK_ELEM x = pop (s);
    STACK_ELEM y = pop (s);
    STACK_ELEM z = pop (s);
    /// insere os 3 elementos na nova ordem.
    push (s,y);
    push (s,x);
    push (s,z);
    return 1;
}

/**
    A função @copia retira do topo da stack o ultimo elemento, sendo que este vai determinar a posição, em indice,
    a partir do topo da stack na qual vamos buscar um elemento e colocá-lo no topo da stack com um apontador diferente mas
    com valores iguais.
    1 8 6 4 1 $ -> 18646

*/

int copia (Stack *s, char *operacao)
{
    if ( s->sp < 2 || strcmp(operacao,"$")) return 0;
    STACK_ELEM w = pop(s);
    if (w.t == STR || w.t == BLK || w.t == ARR)
    {
        push(s,w);return 0;
    }
    long z = stckToLong (w);
    free (w.elem);
    STACK_ELEM x = s->stack [s->sp-1-z],y;
    y = duplicaElemento (x);
    /// criar um void pointer diferente mas com o mesmo conteúdo.
    push(s,y);
    return 1;
}

/**
    As próximas funções são funções de insere para ler cada operação elemento da linha e testar
    automaticamente se são longs ou o que for.
*/


/**
    A função @convertToChar converte qualquer tipo dado como input para char.
*/

int convertToChar (Stack *s, char *operacao)
{
    if (strcmp (operacao,"c")) return 0;
    STACK_ELEM x = pop(s);
    if (x.t == ARR || x.t == BLK)
    {
        push(s,x);return 0;
    }
    char c = stckToChar(x);
    x.t = CHAR;
    free(x.elem);
    x.elem = malloc (sizeof(char));
    *(char *) x.elem = c;
    push(s,x);
    return 1;
}

/**
    A função @convertToDbl converte qualquer tipo dado como input para double.
*/

int convertToDbl (Stack *s, char *operacao)
{
    if (strcmp (operacao,"f")) return 0;
    STACK_ELEM x = pop(s);
    if (x.t == ARR || x.t == BLK)
    {
        push(s,x);return 0;
    }
    double c = stckToDbl(x);
    x.t = DBL;
    free(x.elem);
    x.elem = malloc (sizeof(double));
    *(double *) x.elem = c;
    push(s,x);
    return 1;
}

/**
    A função @convertToLONG converte qualquer tipo dado como input para long.
*/

int convertToLONG (Stack *s, char *operacao)
{
    if (strcmp (operacao,"i")) return 0;
    STACK_ELEM x = pop(s);
    long c = stckToLong(x);
    x.t = LONG;
    free(x.elem);
    x.elem = malloc (sizeof(long));
    *(long *) x.elem = c;
    push(s,x);
    return 1;
}

/**
    A função @convertToStr converte qualquer tipo dado como input para string.
*/

int convertToStr (Stack *s, char *operacao)
{
    if (strcmp (operacao,"s")) return 0;
    STACK_ELEM x = pop(s);
    char *str = stckToSTR(x);
    libertaElemento(x);
    x.t = STR;
    x.elem = str;
    push(s,x);
    return 1;
}

/**
    A função @conversoes é a função responsável por gerir qual das conversões deve ser feita.
*/

int conversoes (Stack *s,char *operacao)
{
    return (convertToChar(s,operacao) || convertToLONG(s,operacao) || convertToDbl(s,operacao) || convertToStr(s,operacao));
}


///  a função @guiao2 ,testa se é possível executar uma destas operações e caso seja, executa-la devidamente.
int guiao2(Stack *s,char *operacao)
{
    return (rodar(s,operacao) || copia (s,operacao) || trocar (s,operacao) || duplicar (s,operacao) ||
    retira (s,operacao));
}
