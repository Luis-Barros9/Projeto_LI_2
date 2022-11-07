#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stack.h"


/// Estas funções retornam 1 caso correspondam a operação executada, caso contrário retornam 0.

/**
    Se a operação executa for "#",a função @exps tira os 2 últimos elementos da, calcula a potencia do antepenultimo elevado ao último
e, de acordo com o tipo de dados destes, armazena na stack esta potencia da forma adqueada.
*/

int exps (Stack *s,char * operacao)
{
    if (s->sp <2 || strcmp(operacao,"#") != 0) return 0;
    STACK_ELEM n = pop (s);
    STACK_ELEM x = pop(s);
    if (n.t == STR || n.t == ARR || n.t == BLK || x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);push(s,n);
        return 0;
    }
    STACK_ELEM y;
    double base = stckToDbl(x), expoente = stckToDbl(n);
    double potencia = pow(base,expoente);
    if (n.t == DBL || x.t == DBL)
    {
        y.t = DBL;
        double *aux = (double *) malloc(sizeof(double));
        *aux = potencia;
        y.elem = aux;
    }
    else if (n.t == LONG || x.t == LONG)
    {
        y.t = LONG;
        long *aux = (long *) malloc(sizeof(long));
        *aux = potencia;
        y.elem = aux;
    }
    else
    {
        y.t = CHAR;
        char *aux = (char *) malloc(sizeof(char));
        *aux = potencia;
        y.elem = aux;
    }
    free(x.elem);free(n.elem);
    //libertar o espaço utilizado pelos anteriores 2 últimos elementos da stack
    push(s,y);
    return 1;
}


/**
    Se a operação executada for "&", a função @e tira os 2 últimos elementos e executa a operação entre estes,
colocando ,assim, no topo da stack o resultado, com o tipo de dados certo.

    Notar que: Só podemos executar a operação "&" com tipos integrais, logo caso um deles seja double, passamos este para long. 
*/
int e (Stack *s,char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"&") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    STACK_ELEM resultado;
    if (x.t == CHAR && y.t == CHAR)
 
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToChar (x) & stckToChar(y);
        resultado.elem = r;
    }
    else
    {
        resultado.t = LONG;
        long *r = (long *) malloc(sizeof(long));
        *r = stckToLong (x) & stckToLong(y);
        resultado.elem = r;
    }
    free (x.elem);
    free (y.elem);
    //libertar o espaço utilizado pelos anteriores 2 últimos elementos da stack
    push (s, resultado);
    return 1;
}

/**
    Se a operação executada for "|", a função @ou tira os 2 últimos elementos e executa a operação entre estes,
colocando ,assim, no topo da stack o resultado, com o tipo de dados certo.

    Notar que: Só podemos executar a operação "|" com tipos integrais, logo caso um deles seja double, passamos este para long.
*/
int ou (Stack *s,char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"|") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    STACK_ELEM resultado;
    if (x.t == LONG || y.t == LONG)
    {
        resultado.t = LONG;
        long *r = (long *) malloc(sizeof(long));
        *r = stckToLong (x) | stckToLong(y);
        resultado.elem = r;
    }
    else 
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToChar (x) | stckToChar(y);
        resultado.elem = r;
    }
    free(x.elem);free(y.elem);
    //libertar o espaço utilizado pelos anteriores 2 últimos elementos da stack
    push (s, resultado);
    return 1;
}

/**
    Se a operação executada for "&", a função @xore tira os 2 últimos elementos e executa a operação entre estes,
colocando, assim, no topo da stack o resultado, com o tipo de dados certo.

    Notar que: Só podemos executar a operação "^" com tipos integrais, logo caso um deles seja double, passamos este para long.
*/

int xore (Stack *s,char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"^") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    STACK_ELEM resultado;
    if (x.t == CHAR && y.t == CHAR)
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToChar (x) ^ stckToChar(y);
        resultado.elem = r;
    }
    else
    {
        resultado.t = LONG;
        long *r = (long *) malloc(sizeof(long));
        *r = stckToLong (x) ^ stckToLong(y);
        resultado.elem = r;
    }
    free (x.elem);
    free (y.elem);
    //libertar o espaço utilizado pelos anteriores 2 últimos elementos da stack
    push (s, resultado);
    return 1;
}

/**
    Se a operação executada for "~", a função @note tira últimos elemento e calcula a sua negação, inserindo o resultado,
com o tipo de dados adequado, no topo da stack.

    Notar que: Só podemos executar a operação "&" com tipos integrais, logo caso este seja double, passamo-lo para long

*/
int note (Stack *s,char *operacao)
{
    if (s->sp <1 || strcmp (operacao,"~") != 0) return 0;
    STACK_ELEM n = pop (s);
    if (n.t == CHAR) // atualizar o valor do último elemento
    {
        
        *(char *) n.elem = ~(*(char *) n.elem);
    }
    else if (n.t == DBL || n.t == LONG)
    {
    ///como tanto pode ser um double como um long, criamos um novo apontador com o devido valor
    /// e libertamos o espaço utilizado pelo anterior
        long *x = (long *) malloc (sizeof(long));
        *x = ~(stckToLong(n));
        free(n.elem);
        n.t = LONG;
        n.elem = x;
    }
    else
    {
        push(s,n);
        return 0;
    }
    push(s,n);
    return 1;
}

/**
    Se a operação executa for "+",a função @add tira os 2 últimos elementos da, somando-os e,
    de acordo com o tipo de dados destes, armazena na stack o resultado da forma adqueada.
*/
int add (Stack *s,char *operacao)
{
    if (s->sp < 2 || strcmp (operacao,"+") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    if (y.t == STR || y.t == ARR || y.t == BLK || x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);push(s,y);
        return 0;
    }
    STACK_ELEM resultado;
    if (x.t == DBL || y.t == DBL)
    {
        resultado.t = DBL;
        double *db = (double *) malloc(sizeof(double));
        *db = stckToDbl (x) + stckToDbl(y);
        resultado.elem = db;
        push(s,resultado);
    }
    else if (x.t == LONG || y.t == LONG)
    {
        resultado.t = LONG;
        resultado.elem = malloc(sizeof(long));
        *(long *) resultado.elem =stckToLong(x) + stckToLong(y);
        push(s,resultado);
    }
    else 
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToLong (x) + stckToLong(y);
        resultado.elem = r;
        push(s,resultado);
    }
    free (x.elem);
    free (y.elem);
    ///libertar o espaço utilizado pelos anteriores elementos da stack
    return 1;
}

/**
    Se a operação executa for "-", a função @sub tira os 2 últimos elementos da stack, subtraindo o penúltimo pelo último e,
    de acordo com o tipo de dados destes, armazena na stack a diferença da forma adqueada.
*/

int sub (Stack *s,char *operacao)
{
if (s->sp <2 || strcmp (operacao,"-") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    if (y.t == STR || y.t == ARR || y.t == BLK || x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);push(s,y);
        return 0;
    }
    STACK_ELEM resultado;

    if (x.t == DBL || y.t == DBL)
    {
        resultado.t = DBL;
        double *r = (double *) malloc(sizeof(double));
        *r = stckToDbl (x) - stckToDbl(y);
        resultado.elem = r;

    }
    else if (x.t == LONG || y.t == LONG)
    {
        resultado.t = LONG;
        long *r = (long *) malloc(sizeof(long));
        *r = stckToLong (x) - stckToLong(y);
        resultado.elem = r;
    }
    else 
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToChar (x) - stckToChar(y);
        resultado.elem = r;
    }
    free (x.elem);
    free (y.elem);
    // libertar o espaço utilizado pelos antigos elementos da stack
    push (s, resultado);
    return 1;
}


/**
    Se a operação executa for "/", a função @divS tira os 2 últimos elementos da stack, dividindo o penúltimo pelo último e,
    de acordo com o tipo de dados destes, armazena na stack o resultado da forma adqueada.
    Notar que: se nenhum destes elementos for double, vamos guardar o quociente da divisão.
*/

int divS (Stack *s,char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"/") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    if (y.t == STR || y.t == ARR || y.t == BLK || x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);push(s,y);
        return 0;
    }
    STACK_ELEM resultado;

    if (x.t == DBL || y.t == DBL)
    {
        resultado.t = DBL;
        double *r = (double *) malloc(sizeof(double));
        *r = stckToDbl (x) / stckToDbl(y);
        resultado.elem = r;

    }
    else if (x.t == LONG || y.t == LONG)
    {
        resultado.t = LONG;
        long *r = (long *) malloc(sizeof(long));
        *r = stckToLong (x) / stckToLong(y);
        resultado.elem = r;
    }
    else 
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToChar (x) / stckToChar(y);
        resultado.elem = r;
    }
    free (x.elem);
    free (y.elem);
    push (s, resultado);
    return 1;
}

/**
    Se a operação executa for "*", a função @mult tira os 2 últimos elementos da stack,multiplicando-os e,
    de acordo com o tipo de dados destes, armazena na stack o produto da forma adqueada.
*/

int mult (Stack *s,char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"*") != 0) return 0;
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    if (y.t == STR || y.t == ARR || y.t == BLK || x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);push(s,y);
        return 0;
    }
    STACK_ELEM resultado;

    if (x.t == DBL || y.t == DBL)
    {
        resultado.t = DBL;
        double *r = (double *) malloc(sizeof(double));
        *r = stckToDbl (x) * stckToDbl(y);
        resultado.elem = r;

    }
    else if (x.t == LONG || y.t == LONG)
    {
        resultado.t = LONG;
        long *r = (long *) malloc(sizeof(long));
        *r = stckToLong (x) * stckToLong(y);
        resultado.elem = r;
    }
    else 
    {
        resultado.t = CHAR;
        char *r = (char *) malloc(sizeof(char));
        *r = stckToChar (x) * stckToChar(y);
        resultado.elem = r;
    }
    free (x.elem);
    free (y.elem);
    push (s, resultado);
    return 1;
}

/**
    Caso a operação se trate de uma incrementação ou decrementação (")" ou "("), vamos manter o tipo de dados do último elemento e 
    incrementar ou decrementar o seu valor através da função @decreIncrementar, dependendo da operação, atualizando assim o valor do último elemento da stack.
*/

int decreIncrementar (Stack *s,char * operacao)
{
    if (s->sp <1) return 0;
    if (strcmp (operacao,")") == 0)
    {
        STACK_ELEM x= pop(s);
        if (x.t == ARR || x.t == STR || x.t == BLK)
        {
            push(s,x);
            return 0;
        }
        //testar se funciona
        if (x.t == CHAR)
        {
            *(char *) x.elem +=1;
            // testar * (char *) x.elem ++;
        }
        else if (x.t == LONG)
        {
            *(long *) x.elem +=1;
            // testar * (char *) x.elem ++;
        }
        else if (x.t == DBL)
        {
            *(double *) x.elem +=1;
            // testar * (char *) x.elem ++;
        }
        push (s,x);
        return 1;   
    }
    if (strcmp (operacao,"(") == 0)
    {
        STACK_ELEM x= pop(s);
        if (x.t == ARR || x.t == STR || x.t == BLK)
        {
            push(s,x);
            return 0;
        }
        //testar se funciona
        if (x.t == CHAR)
        {
            *(char *) x.elem -=1;
            // testar * (char *) x.elem ++;
        }
        else if (x.t == LONG)
        {
            *(long *) x.elem -=1;
            // testar * (char *) x.elem ++;
        }
        else if (x.t == DBL)
        {
            *(double *) x.elem -=1;
            // testar * (char *) x.elem ++;
        }
        push (s,x);
        return 1;
    }
    return 0;
}

/**
    Se a operação executa for "%", a função @modulo tira os 2 últimos elementos da stack, dividindo o penúltimo pelo último e,
    de acordo com o tipo de dados destes, armazena na stack o resto da divisão da forma adqueada.
    Notar que: o módulo da divisão só interessa para inteiros, logo caso um destes elementos seja um double vamos passá-lo para long.
*/

int modulo(Stack *s,char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"%") != 0) return 0;
    STACK_ELEM divisor = pop(s);
    STACK_ELEM dividendo = pop(s);
    STACK_ELEM x;
    if (divisor.t == BLK || divisor.t == ARR || divisor.t ==STR || dividendo.t == BLK || dividendo.t == ARR || dividendo.t == STR)
    {
        push(s,dividendo);
        push(s,divisor);
        return 0;
    }
    if (divisor.t == CHAR && dividendo.t == CHAR)
    {
        x.t = CHAR;
        char *aux = (char *) malloc (sizeof(char));
        *aux = stckToChar(dividendo) % stckToChar(divisor);
        x.elem = aux;
    }
    else
    {
        x.t = LONG;
        long *aux = (long *) malloc (sizeof(char));
        *aux = stckToLong(dividendo) % stckToLong(divisor);
        x.elem = aux;
    }
    push(s,x);
    free(divisor.elem);free(dividendo.elem);
    return 1;
}


///  a função @guiao1 ,testa se é possível executar uma destas operações e caso seja, executa-la devidamente.
int guiao1 (Stack *s,char *operacao)
{   /// tenta executar as operações executaveis, retorna 1 se tiver executado uma operação ou retorna 0 se não conseguir.
    return (add (s, operacao) || sub (s,operacao) || divS(s,operacao) || mult (s,operacao) ||
            decreIncrementar (s, operacao) || modulo (s, operacao) || exps (s, operacao) || e (s, operacao) ||
            ou (s, operacao) || xore (s, operacao) || note (s, operacao));
}
