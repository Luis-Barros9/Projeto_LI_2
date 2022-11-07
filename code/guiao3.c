#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "guiao2.h"


/**
    A função @posEfetiva calcula a posição no array da variável.
*/

int posEfetiva (char c)
{
    return (c- 'A');
}

/**
    A função @inicializaVars inicializa os valor das variáveis, as variáveis não default tem apontador NULL,
    pois não possuem um valor atribuido ainda.
*/

void inicializaVars (STACK_ELEM vars[])
{
    int i;
    for(i=0;i<= posEfetiva('Z');i++)
    {
        vars[i].elem = NULL;//apontadores nao inicializados são nulos
    }
    STACK_ELEM x;
    x.t = LONG;
    for (i =0;i<=posEfetiva('F');i++)
    {
        long *y = (long *) malloc (sizeof(long));
        *y = 10+i;
        x.elem = y;
        vars[i] = x;
    }
    for (i = 0; i<=2;i++)
    {
        long *y = (long *) malloc (sizeof(long));
        *y = i;
        x.elem = y;
        vars[i+posEfetiva('X')] = x;
    }
    x.t = CHAR;
    char *n = (char *) malloc(sizeof(char));
    *n = '\n';
    x.elem = n;
    vars[posEfetiva('N')] = x;
    char *s = (char *) malloc(sizeof(char));
    *s = ' ';
    x.elem = s;
    vars[posEfetiva('S')] = x;
}

/**
    A função @alteraVars altera o valor de uma variável e, caso esta possua um valor anteriormente, liberta
    o espaço na memória ocupado anteriormente.
*/

int alteraVars (Stack *s,char *operacao,STACK_ELEM vars[])
{
    if (s-> sp < 1 || operacao[0] != ':' || operacao[1]<'A'|| operacao[1]>'Z') return 0;
    char var = operacao[1];
    int pos = posEfetiva(var);
    STACK_ELEM x = pop(s);
    push(s,x);
    STACK_ELEM y = vars[pos];
    if (y.elem != NULL) 
    {
        /// se a variavél já tiver sido inicializada, libertar o espaço ocupado anteriormente.
        if (y.t == ARR) libertaStack((Stack *) y.elem);
        free (y.elem);
    }
    vars[pos] = duplicaElemento (x);  /// introduz na variável o topo da stack repetido.
    return 1;
    
}

/**
    A função @introduzVar introduz um dada variável na stack.
*/

int introduzVar(Stack *s,char *operacao, STACK_ELEM vars[])
{
    if (operacao[0]<'A' || operacao[0]>'Z' || operacao[1] != '\0') return 0;
    char var = operacao[0];
    STACK_ELEM x = vars[posEfetiva(var)];
    if (x.elem == NULL) return 0;
    /// buscar o elemento correspondente a variável.
    STACK_ELEM y = duplicaElemento(x);
    /**
    introduzir o elemento na stack, na posição correta com um pointer diferente do pointer no array das vars
    de forma a que caso tenhamos de alterar um valor destes, por exemplo por uma soma na stack ou alterar o valor da
    variável,apenas alteramos o elemento desejado.
    */
    push(s,y); ///ignorar o último elemento, pois trata se do elemento que se encontra nas variáveis, com o mesmo pointer.
    return 1;
}

/**
    A função @operaVars é responsável por gerencia a introdução/alteração de variáveis.
*/

int operaVars (Stack *s,char *operacao,STACK_ELEM vars[])
{
    return (introduzVar(s,operacao,vars) || alteraVars(s,operacao,vars));
    
}


/**
    A função @igual testa a igualdade de dois elementos, de qualquer tipo, em caso de igualdade
    devolve 1, caso sejam distintos, devolve 0!
*/

int igual (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"=")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    resultado.t=LONG; /// como só iremos devolver 0 ou 1, podemos fazer por definição que o tipo será LONG.
    resultado.elem=(long *) malloc(sizeof(long));
    if (x.t == BLK || y.t == BLK  || x.t == ARR || y.t == ARR)
    {
        push(s,y);push(s,x);
        return 0;
    }
    if (x.t==y.t && y.t==STR)
    {
        if (strcmp(x.elem,y.elem)==0) *(long *)resultado.elem=1;
        else *(long *)resultado.elem=0;
    }
    else if (y.t == STR || x.t == STR)
    {
        push(s,y);push(s,x);
        return 0;
    }
    else
    {
        if (stckToDbl(x)==stckToDbl(y)) *(long *)resultado.elem=1;
        else *(long *)resultado.elem=0;
    }
    free (x.elem);
    free (y.elem);
    push(s,resultado);
    return 1;
}

/**
    A função @menor testa se o primeiro elemento é menor que o segundo, em caso de verdade,
    devolve 1, caso seja falso, devolve 0!
*/

int menor (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"<")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    if (x.t == ARR || x.t == BLK || y.t == ARR || y.t == BLK)
    {
        push (s,y);push(s,x);
        return 0;
    }
    if ((x.t == STR || y.t == STR) && x.t != y.t)
    {
        push (s,y);push(s,x);
        return 0;
    }
    STACK_ELEM resultado;
    resultado.t=LONG; /// como só iremos devolver 0 ou 1, podemos fazer por definição que o tipo será LONG.
    resultado.elem=(long *) malloc(sizeof(long));
    if (x.t==y.t && y.t==STR)
    {
        if (strcmp(x.elem,y.elem)>0) *(long *)resultado.elem=1;
        else *(long *)resultado.elem=0;
    }
    else
    {
        if (stckToDbl(x)>stckToDbl(y)) *(long *)resultado.elem=1;
        else *(long *)resultado.elem=0;
    }
    free (x.elem);
    free (y.elem);
    push(s,resultado);
    return 1;
}

/**
    A função @maior testa se o primeiro elemento é maior que o segundo, em caso de verdade,
    devolve 1, caso seja falso, devolve 0!
*/

int maior (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,">")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    if (x.t == ARR || x.t == BLK || y.t == ARR || y.t == BLK)
    {
        push (s,y);push(s,x);
        return 0;
    }
    if ((x.t == STR || y.t == STR) && x.t != y.t)
    {
        push (s,y);push(s,x);
        return 0;
    }
    resultado.t=LONG; /// como só iremos devolver 0 ou 1, podemos fazer por definição que o tipo será LONG.
    resultado.elem=(long *) malloc(sizeof(long));
    if (x.t==y.t && y.t==STR)
    {
        if (strcmp(x.elem,y.elem)<0) *(long *)resultado.elem=1;
        else *(long *)resultado.elem=0;
    }
    else
    {
        if (stckToDbl(x)<stckToDbl(y)) *(long *)resultado.elem=1;
        else *(long *)resultado.elem=0;
    }
    free (x.elem);
    free (y.elem);
    push(s,resultado);
    return 1;
}

/**
    A função @menorDosDois devolve o menor dos dois elementos.
    Esta função compara todos os tipos, isto é, por exemplo: 3.5 < 'A'
*/

int menorDosDois (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"e<")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    if (x.t==y.t && y.t==STR)
    {
        if (strcmp(x.elem,y.elem)>0) 
        {
            resultado.t=STR;
            resultado.elem=y.elem;
        }
        else 
        {
            resultado.t=STR;
            resultado.elem=x.elem;
        }
    }
    else 
    {
        if (stckToDbl(x)>stckToDbl(y)) 
        /** o tipo que engloba tudo, é o Double, então, serão ambos transformados em double
        para que seja possivel comparà-los. Após determinar o maior/menor, analisamos de que tipo este
        era inicialmente e voltamos a transformá-lo de double para o tipo desejado!
        */
        {
            if (y.t==LONG) 
            {
                resultado.t=LONG;
                resultado.elem=(long *) malloc(sizeof(long));
                *(long *)resultado.elem=stckToLong(y);
            }
            else if (y.t==DBL) 
            {
                resultado.t=DBL;
                resultado.elem=(double *) malloc(sizeof(double));
                *(double *) resultado.elem=stckToDbl(y);
            }
            else if (y.t==CHAR) 
            {
                resultado.t=CHAR;
                resultado.elem=(char *) malloc(sizeof(char));
                *(char *) resultado.elem=stckToChar(y);
            }
        }
        else 
        {
            if (x.t==LONG) 
            {
                resultado.t=LONG;
                resultado.elem=(long *) malloc(sizeof(long));
                *(long *)resultado.elem=stckToLong(x);
            }
            else if (x.t==DBL) 
            {
                resultado.t=DBL;
                resultado.elem=(double *) malloc(sizeof(double));
                *(double *) resultado.elem=stckToDbl(x);
            }
            else if (x.t==CHAR) 
            {
                resultado.t=CHAR;
                resultado.elem=(char *) malloc(sizeof(char));
                *(char *) resultado.elem=stckToChar(x);
            }
        }
    }
    push(s,resultado);
    return 1;
}

/**
    A função @maiorDosDois devolve o maior dos dois elementos.
    Esta função compara todos os tipos, isto é, por exemplo: 100 > 'A'
*/

int maiorDosDois (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"e>")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    if (x.t==y.t && y.t==STR)
    {
        if (strcmp(x.elem,y.elem)<0) 
        {   
            // resultado = y;free(x.elem);
            resultado.t=STR;
            resultado.elem=y.elem;
        }
        else 
        {
            // resultao =x; free(y.elem);
            resultado.t=STR;
            resultado.elem=x.elem;
        }
    }
    else
    {
        if (stckToDbl(x)<stckToDbl(y)) 
        /** o tipo que engloba tudo, é o Double, então, serão ambos transformados em double
        para que seja possivel comparà-los. Após determinar o maior/menor, analisamos de que tipo este
        era inicialmente e voltamos a transformá-lo de double para o tipo desejado!
        */
        {
            if (y.t==LONG) 
            {
                resultado.t=LONG;
                resultado.elem=(long *) malloc(sizeof(long));
                *(long *)resultado.elem=stckToLong(y);
            }
            else if (y.t==DBL) 
            {
                resultado.t=DBL;
                resultado.elem=(double *) malloc(sizeof(double));
                *(double *) resultado.elem=stckToDbl(y);
            }
            else if (y.t==CHAR) 
            {
                resultado.t=CHAR;
                resultado.elem=(char *) malloc(sizeof(char));
                *(char *) resultado.elem=stckToChar(y);
            }
            /* TESTAR
            resultado = y;
            free(x.elem);*/
        }
        else 
        {
            /* TESTAR
            resultado = x;
            free(y.elem);*/
            if (x.t==LONG) 
            {
                resultado.t=LONG;
                resultado.elem=(long *) malloc(sizeof(long));
                *(long *)resultado.elem=stckToLong(x);
            }
            else if (x.t==DBL) 
            {
                resultado.t=DBL;
                resultado.elem=(double *) malloc(sizeof(double));
                *(double *) resultado.elem=stckToDbl(x);
            }
            else if (x.t==CHAR) 
            {
                resultado.t=CHAR;
                resultado.elem=(char *) malloc(sizeof(char));
                *(char *) resultado.elem=stckToChar(x);
            }
        }
    }
    push(s,resultado);
    return 1;
}

/**
    A função @eShortcut testa se os dois últimos elementos são verdadeiros. Caso sejam, devolve o elemento
    do topo da stack, caso um deles seja igual a 0, coloca 0 na stack.
*/

int eShortcut (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"e&")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    if ((stckToDbl(x)==0 || stckToDbl(y)==0) && (x.t!=STR || y.t!=STR)) 
    {
        resultado.t=LONG;
        resultado.elem= malloc(sizeof(long));
        *(long *)resultado.elem=0;
    }
    else 
    {
        resultado.t=x.t;
        resultado.elem=x.elem;
    }
    push(s,resultado);
    return 1;
}

/**
    Semelhante à anterior, a função @ouShortcut testa se pelo menos um dos dois valores do topo da stack são verdadeiros, ou seja,
    se forem os dois falsos, coloca 0 no topo, se forem os dois verdadeiros coloca o primeiro no topo, se um deles for falso,
    coloca o outro no topo.
*/

int ouShortcut (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"e|")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    if ((stckToDbl(x)==0 && stckToDbl(y)==0) && (x.t!=STR && y.t!=STR))
    {
        resultado.t=LONG;
        resultado.elem= malloc(sizeof(long));
        *(long *)resultado.elem=0;
    }
    else 
    {
        if(stckToDbl(y)==0 && y.t!=STR)
        {
            resultado.t=x.t;
            resultado.elem=x.elem;
        }
        else 
        {
            resultado.t=y.t;
            resultado.elem=y.elem;
        }
    }
    push(s,resultado);
    return 1;
}

/**
    A função @nao troca os valores do elemento do topo da stack, ou seja, se for verdade, diferente de 0,
    coloca 0 no topo, se for 0, coloca 1.
*/

int nao (Stack *s, char *operacao)
{
    if (s->sp<1 || strcmp(operacao,"!")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    if (x.t == BLK)
    {
        push(s,x);
        return 0;
    }
    STACK_ELEM resultado;
    resultado.t=LONG;
    resultado.elem= malloc(sizeof(long));
    if (valorLogico(x))
        *(long*)resultado.elem=0;
    else
        *(long*)resultado.elem=1;
    push(s,resultado);
    libertaElemento(x);
    return 1;
}

/**
    Vai buscar os 3 elementos do topo da stack.
    Se o primeiro for verdadeiro, coloca no topo da stack o segundo.
    Se o primeiro for falso, coloca o terceiro no topo da stack.
*/

int ifThenElse (Stack *s, char *operacao)
{
    if (s->sp<3 || strcmp(operacao,"?")!=0) return 0; /// teste para determinar o tipo de operação.
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM z = pop(s);
    STACK_ELEM resultado;
    if (z.t==STR || z.t == BLK ) 
    {
        char *str = (char *) z.elem;
        if (str[0] != '\0')
        {
            resultado=y;
            libertaElemento(x);
        }
        else
        {
            resultado=x;
            libertaElemento(y);
        }
    }
    else if (z.t == ARR)
    {
        Stack *prov = (Stack *) z.elem;
        if (prov->sp)
        {
            resultado = y;
            libertaElemento(x);
        }
        else
        {
            resultado = x;
            libertaElemento(y);
        }
    }
    else if (stckToDbl(z)==0) 
    {
        resultado=x;
        libertaElemento(y);
    }
    else 
    {
        resultado=y;
        libertaElemento(x);
    }
    
    /* 
    possível alternativa
    if (valorLogico(z))
    {
        resultado = y;
        libertaElemento(x);
    }
    else
    {
        resultado = x;
        libertaElemento(y);
    }
    */
    
    push(s,resultado);
    libertaElemento(z);
    return 1;
}

/// A função @logica contem as operaçoes de logica!
int logica (Stack *s,char *operacao)
{
    return (igual(s,operacao) || menor(s,operacao) || maior(s,operacao) || menorDosDois(s,operacao) || maiorDosDois(s,operacao) || 
    nao(s,operacao) || eShortcut(s,operacao) || ouShortcut(s,operacao) || ifThenElse(s,operacao));
}

/// A função @guiao3 contém todas as funçẽs do guiao3!
int guiao3(Stack *s, char *operacao,STACK_ELEM vars[])
{
    return (operaVars(s,operacao,vars) || logica(s,operacao));
}

