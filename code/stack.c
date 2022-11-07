#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

/**
    A funcção @newStack inicializa uma stack, reservando espaço para esta na memória,caso seja possível
    establecer o stack pointer para 0, de forma a saber que a stack apresenta 0 elementos inseridos.
*/

Stack *newStack()
{
    /// criar apontador da stack
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s!=NULL) 
    {
        /// se nao ocorrer nenhum erro , inicializar a stack vazia com espaço para 25 elementos.
        s->sp =0;
        s->size = 8;
        s->stack = (STACK_ELEM *) malloc (s->size*sizeof(STACK_ELEM));
    }
    return s;
}

/**
    A função @push coloca um elemento na stack e acrescenta o stack pointer.
    (Duplica o tamanho se for necessário).
*/

void push(Stack *s, STACK_ELEM x)
{
     ///se a stack encontrar-se cheia, duplicar o seu tamanho.
    if (s->sp >= s->size)
    {
        s->size *= 2;
        s->stack = (STACK_ELEM *) realloc (s->stack,s->size*sizeof(STACK_ELEM));
    }
    /// introduzir o elemento x no topo da stack.
    s->stack[s->sp] = x ;
    s->sp++;
}

/**
    A função @pop decrementa o stack pointer de forma diminuir o número efetivo de elementos da stack
    e da return do elemento que se encontrava, anteriormente, no topo desta.
*/

STACK_ELEM pop(Stack *s)
{
    s->sp--;
    STACK_ELEM ret = s->stack[s->sp];
    return ret;
}

/**
    A função 2stckToDbl recebe um elemento da stack e, sem alterar este, faz corresponde-lo
    ao seu valor em forma de double, retornando-o.
*/

double stckToDbl (STACK_ELEM x)
{
    double r=0;
    if (x.t == DBL)
    {
        r = *(double *) (x.elem);
    }
    else if (x.t == LONG)
    {
        r = *(long *) (x.elem); 
    }   
    else if (x.t == CHAR)
    {
        r = *(char *) (x.elem);
    }
    else if (x.t == STR)
    {
        sscanf((char *)x.elem, "%lf", &r);
    }
    /// transformar os restantes tipos em dbl.
    return r;
}

/**
    A função @stckToLong recebe um elemento da stack e, sem alterar este, faz corresponde-lo
    ao seu valor em forma de long, retornando-o.
*/

long stckToLong (STACK_ELEM x)
{
    long r=0;
    if (x.t == DBL)
        r = *(double *) x.elem;
    
    else if (x.t == LONG)
        r = *(long *) x.elem;
    
    else if (x.t == CHAR)
        r = *(char *) x.elem;
    else
    {
        r = atoi((char *) x.elem);
    }    
    /// transformar os restantes tipos em long.
    return r;
}

/**
    A função @stckToChar recebe um elemento da stack e, sem alterar este, faz corresponde-lo
    ao seu valor em forma de char, retornando-o.
*/

char stckToChar (STACK_ELEM x)
{
    char r=0;
    if (x.t == DBL)
        r = *(double *) x.elem;
    
    else if (x.t == LONG)
        r = *(long *) x.elem;
    
    else if (x.t == CHAR)
        r = *(char *) x.elem;
    else
        r = ((char *)x.elem) [0];
    
    /// transformar os restantes tipos em dbl.
   return r;
}

char *stckToSTR (STACK_ELEM x)
{
    char *str = malloc(20000*sizeof(char));
    if (x.t == STR || x.t == BLK)
    {
        copiaStr((char *) x.elem,str);
    }
    if (x.t == CHAR)
    {
        str [0] = *(char *) x.elem;
        str[1] = '\0';
    }
    else if (x.t == LONG)
    {
        sprintf(str,"%ld",*(long *) x.elem);

    }
    else if (x.t == DBL)
    {
        sprintf(str,"%g",*(double *) x.elem);

    }
    else 
    {
        STACK_ELEM prov = arrParaSTR (x);
        free(str);
        str = stckToSTR(prov);
        libertaElemento (prov);
    }
    return str;
}

// função que determina o valor lógico de um elemento (0 se for false 1 se for true)
/// A função @valorLogico determina o valor lógico de um elemento (0 se for false 1 se for true).
int valorLogico (STACK_ELEM x)
{
    /// supor que um bloco não tem valor lógico.
    int n=0;
    if (x.t == ARR)
    {
        Stack *s = (Stack *) x.elem;
        if (s->sp ) /// se o array não for vazio.
            n =1;
    }
    else if (x.t == STR)
    {
        char *str = (char *) x.elem;
        if (str[0] != '\0') /// se a string não for vazia.
            n = 1;
    }
    else
    {
        if (stckToDbl(x)) /// se o valor númerico não corresponder a 0.
            n =1;
    }
    return n;
}

/// A função @copiaStr recebe uma string e copia os valores para outra string, mantendo os apontadores inalteráveis.
void copiaStr (char *original,char * destino)
{
    int i=0;
    for (i =0;original[i]!= '\0';i++)
        destino[i] =original[i];
    destino[i] = '\0';
}

/// A função @simplificaInput retira todos os espaços a esquerda e a direita do input, além disso,
/// passa a quebra de linha final (caso haja mais que uma linha) para o fim da string.
void simplificaInput (char v[])
{
    if (v[0] == '\0') return;
    int ultimapos = strlen(v)-1,i;
    if (v[ultimapos] == '\n')
    {
        v[ultimapos] = '\0';
        ultimapos--;
    }
    for (;v[ultimapos] == ' ' && ultimapos > 0;ultimapos--);
    v[ultimapos+1] = '\0';
    for (i =0;v[i] == ' ';i++);
    copiaStr (v+i,v);
}

/// A função @juntaStacks dá push aos elementos da segunda para a primeira stack.
void juntaStacks(Stack *nova,Stack *arr1)
{
    // [1 2 3 "ola"] [1 [4 5]] -> [1 2 3 "ola" 1 [4 5]]
    int i =0;
    while (i< arr1->sp)
    {
        push (nova,arr1->stack[i]);
        i++;
    }
}

/// A função @libertaStack liberta o espaço ocupado pelos elementos de uma stack.
void libertaStack (Stack *s)
{
    while (s->sp >0)
    {
        STACK_ELEM x = pop(s);
        if (x.t == ARR) libertaStack((Stack *) x.elem);

        free(x.elem);
    }
    free(s->stack);
}

/**
    A função @duplicaElemento a partir de um elemento x, cria outro, com o mesmo conteudo,
    mas com um apontador diferente, de forma a que alterar ou libertar um não vai afetar o outro elemento.
*/

STACK_ELEM duplicaElemento (STACK_ELEM x)
{
    STACK_ELEM duplo;
    duplo.t = x.t;
    if (x.t == LONG)
    {
        long *novo = (long *) malloc(sizeof(long));
        *novo = *(long *) x.elem;
        duplo.elem = novo;
    }
    else if (x.t == DBL)
    {
        double *novo = (double *) malloc(sizeof(double));
        *novo = *(double *) x.elem;
        duplo.elem = novo;
    }
    else if (x.t == CHAR)
    {
        char *novo = (char *) malloc(sizeof(char));
        *novo = *(char *) x.elem;
        duplo.elem = novo;
    }
    else if (x.t == STR || x.t == BLK)
    {
        char *str = (char *) malloc(20000*sizeof(char));
        copiaStr ((char *) x.elem,str);
        duplo.elem = str;
    }
    else if (x.t == ARR )
    {
        Stack *s = newStack();
        Stack *original = (Stack *) x.elem;
        int i=0;
        while (original->sp > i)
        {
            STACK_ELEM y = original->stack[i];
            push(s,duplicaElemento(y));
            i++;
        }
        duplo.elem = s;
    }
    return duplo;
}

/**
    A função @libertaElemento liberta o espaço ocupado por um elemento da stack, sendo que
    se este for um array(stack), vamos ter de libertar o espaço de todos os seus elementos
    e o espaço reservado para o array.
*/

void libertaElemento (STACK_ELEM x)
{
    if (x.t == ARR) libertaStack ((Stack *) x.elem);
    free(x.elem);
}

/**
    A função @parteArray parte um certo array nos seus respetivos subarrays.
*/

Stack *parteArray (Stack *s)
{ //[1 2 3 4 6 "ola" [1 2]]- > [[2] [4] [6] ["olaola"] [[1 2 1 2]]] 2 * -> []
    
    Stack *novo = newStack();
    int i=0;
    while (i<s->sp)
    {
        Stack *parte = newStack();
        push(parte,duplicaElemento(s->stack[i]));
        STACK_ELEM x;
        x.t = ARR;
        x.elem = parte;
        push(novo,x);
        i++;
    }
    return novo;
}

/// A função @arrParaSTR transforma um array de carateres em uma string.
STACK_ELEM arrParaSTR (STACK_ELEM x)
{
    Stack *s = (Stack *) x.elem;
    char *str = malloc(sizeof(char)*20000);
    STACK_ELEM resultado;
    resultado.t = STR;
    resultado.elem=str;

    int i =0;
    while (i < s->sp)
    {
        STACK_ELEM car = s-> stack[i];
        if (car.t == BLK || car.t == STR || car.t == ARR)
        {
            str[0] ='\0';
            return resultado;
        }
        str[i] = stckToChar(car);
        i++;
    }
    str[i] = '\0';
    return resultado;
}

/// A função @strParaARR que transforma uma string num array.
STACK_ELEM strParaARR (STACK_ELEM x)
{
    char *str = (char *) x.elem;
    Stack *novo = newStack();
    STACK_ELEM resultado;
    resultado.t = ARR;
    resultado.elem = novo;
    int i = 0;
    while (str[i] != '\0')
    {
        char *ch = (char *) malloc (sizeof(char));
        STACK_ELEM car;
        car.t = CHAR;
        car.elem = ch;
        *ch = str[i];
        push(novo,car);
        i++;
    }
    return resultado;
}

int menorElem (STACK_ELEM x,STACK_ELEM y)
// funçao que testa se o elemento x é menor que y
{
    int cond;
    if (x.t == y.t && y.t==STR)
    {
        if (strcmp((char *) x.elem,(char *) y.elem)<0) cond=1;
        else cond = 0;
    }
    else
    {
        if (stckToDbl(x)<stckToDbl(y)) cond=1;
        else cond = 0;
    }
    return cond;
}

void duplicaVARS (STACK_ELEM original[], STACK_ELEM copia[])
{
    int i =0;
    while (i<26)
    {
        STACK_ELEM x = original[i];
        if (x.elem != NULL)
            copia[i] = duplicaElemento(x);
        else
            copia[i] = x;
        i++;
    }
}

void libertaVARS (STACK_ELEM vars[])
{
    int i =0;
    while (i<26)
    {
        STACK_ELEM x = vars[i];
        if (x.elem != NULL)
        {
            libertaElemento(x);
        }
        i++;
    }
}