#include "stack.h"
#include "guiao1.h"
#include "guiao2.h"
#include "guiao3.h"
#include "guiao4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int recebeinput (char parte[],Stack *s,STACK_ELEM vars[]);
int usar (Stack *s,char * parte,STACK_ELEM vars[]);

void imprimeSt (Stack *s)
{
    if (s == NULL) return;
    for (int i=0;i<s->sp;i++)
    {
        STACK_ELEM x = s->stack[i];
        if (x.t==LONG)
        {
            printf("%ld", *(long *) (x.elem));
        }
        else if (x.t==CHAR)
        {
            printf("%c", *(char *) (x.elem));
        }
        else if (x.t==DBL)
        {
            printf("%g", *(double *) (x.elem));
        }
        else if (x.t==STR)
        {
            printf("%s", (char *) x.elem);
        } // libertar o espaço da memória do i-ésimo elemento da stack
        else if (x.t == ARR)
        {
            imprimeSt ((Stack *) x.elem);
        }
        else
        {
            // imprimir bloco
            printf("{%s}",(char *) x.elem);
        }
    }
}


/* 
Função de insere para ler cada operação/elemento da linha e testar
automaticamente se são longs ou o que for.
*/
int insereInt (Stack *s,char *operacao)
{
    long *x = (long *) malloc(sizeof(long));
    if (sscanf(operacao,"%ld",x) != 1) return 0; // testa se é long e se for, guarda na stack com esse tipo, se não for, testa se pode ser outro tipo.
    STACK_ELEM y;
    y.t=LONG;
    y.elem =x;
    push(s,y);
    return 1;
}

/* 
Função de insere para ler cada operação/elemento da linha e testar
automaticamente se são longs ou o que for.
*/
int insereChar (Stack *s,char *operacao)
{
    char *x = (char *) malloc(sizeof(char));
    if (sscanf(operacao,"%c",x) != 1) return 0; // testa se é char e se for, guarda na stack com esse tipo, se não for, testa se pode ser outro tipo.
    STACK_ELEM y;
    y.t = CHAR;
    y.elem = x;
    push(s,y);
    return 1;
}

/* 
Função de insere para ler cada operação/elemento da linha e testar
automaticamente se são longs ou o que for.
*/
int insereDbl (Stack *s,char *operacao)
{
    double *x = (double *) malloc (sizeof(double));
    int teste=0;
    if (sscanf(operacao,"%lf",x) != 1) return 0; // testa se é double e se for, guarda na stack com esse tipo, se não for, testa se pode ser outro tipo.
    for(int i=0;operacao[i]!='\0';i++) if(operacao[i]=='.') teste=1; // para saber se é double temos de ler se na string desse elemento est+a contido um ponto (.).
    if (teste==0) return 0;
    STACK_ELEM novo;
    novo.t=DBL;
    novo.elem=x;
    push(s,novo);
    return 1;

}

/* 
Função de insere para ler cada operação/elemento da linha e testar
automaticamente se são longs ou o que for.
*/
void insereStr (Stack *s,char *operacao)
{
    char *str = (char *) malloc (20000*sizeof(char)) ;
    copiaStr(operacao,str);
    STACK_ELEM x;
    x.t=STR;
    x.elem=str;
    push(s,x);
}


int lerlinha (Stack *s,char *operacao)
{
    if(strcmp(operacao,"l")) return 0;
    char *str=malloc (1000*sizeof(char));
    STACK_ELEM x;
    x.t = STR;
    if (fgets(str,1000,stdin) == NULL)
    {
        free(str);
        return 1;
    }
    x.elem = str;
    push(s,x);
    return 1;
}

int lertodaslinhas (Stack *s,char *operacao)
{
    if(strcmp(operacao,"t")) return 0;
    char *str=malloc (20000*sizeof(char));
    str[0] ='\0';
    STACK_ELEM x;
    x.t = STR;
    char aux[10000];
    int i = strlen(str);
    while (fgets(aux,10000,stdin) != NULL)
    {
        int t = strlen(aux)-1;
        if (t>=0 && aux[t] != '\n') // ultima linha
        { 
            str[i] = ' ';
            i++;
            str[i] = '\0';
        }
        copiaStr(aux,str+i);
        i = strlen(str);

    };
    x.elem = str;
    push(s,x);
    return 1;
}

int imprimeTopo (Stack *s,char *operacao)
{
    if (s->sp < 1 || strcmp(operacao,"p")) return 0;
    STACK_ELEM x = pop(s);
    push(s,x);
    if (x.t==LONG)
    {
        printf("%ld\n", *(long *) (x.elem));
    }
    else if (x.t==CHAR)
    {
        printf("%c\n", *(char *) (x.elem));
    }
    else if (x.t==DBL)
    {
        printf("%g\n", *(double *) (x.elem));
    }
    else if (x.t==STR)
    {
        printf("%s\n", (char *) x.elem);
    } // libertar o espaço da memória do i-ésimo elemento da stack
    else if (x.t == ARR)
    {
        imprimeSt ((Stack *) x.elem);
    }
    else
    {
    // imprimir bloco
        printf("{%s}\n",(char *) x.elem);
    }
    return 1;
}


/* funções que introduzem elementos na stack */
int insere(Stack *s, char *operacao)
{
    return (insereDbl(s,operacao) || insereInt(s,operacao));
}

int executaBloco (Stack *s,char *operacao,STACK_ELEM vars[])
{
    if (s->sp < 2 || strcmp(operacao,"~")) return 0;
    STACK_ELEM bloco = pop(s);
    if (bloco.t != BLK)
    {
        push(s,bloco);
        return 0;
    }
    recebeinput((char *) bloco.elem,s,vars);
    free (bloco.elem);
    return 1;    
}

int fold (Stack *s,char *parte, STACK_ELEM vars[])
{   // tenho que refazer.
    if (s->sp < 2 || strcmp(parte,"*")) return 0;
    STACK_ELEM opera = pop(s);
    STACK_ELEM array = pop(s);
    if (opera.t != BLK || array.t != ARR)
    {
        push(s,array);
        push(s,opera);
        return 0;
    }
    Stack *original = (Stack *) array.elem;
    char *str = (char *) opera.elem;
    char aux[20000];
    copiaStr(str,aux);
    Stack *teste = newStack();
    array.elem = teste;
    int i =0;
    while (i < original->sp)
    {
        STACK_ELEM x = original->stack[i];
        push(teste,x);
        STACK_ELEM acumula = duplicaElemento(array);
        STACK_ELEM copia[26];
        duplicaVARS(vars,copia);
        if (recebeinput(aux,teste,copia) == 0)
        {
            libertaElemento(array);
            array = acumula;
            teste = (Stack *) array.elem;
        }
        else libertaElemento(acumula);
        libertaVARS(copia);
        copiaStr(str,aux);
        i++;
    }    
    free(original->stack);
    free(original);
    free (str);
    push(s,array);
    return 1;    
}

int aplicarBloco(Stack *s,char *parte,STACK_ELEM vars[])
{
    if (s->sp < 2 || strcmp(parte,"%")) return 0;
    STACK_ELEM bloco = pop(s);
    STACK_ELEM x = pop(s);
    if (bloco.t != BLK || (x.t != ARR && x.t != STR))
    {
        push(s,x);
        push(s,bloco);
        return 0;
    }

    /* um blovo náo é uma stack
    Stack *new = (Stack *)bloco.elem;
    */
    int cond = (x.t == STR); //condição que mantém o registro que se trata de uma string
    if (cond)
    {
        void *aux = x.elem;
        x = strParaARR (x);
        free(aux);
    }
    char *str = (char *) bloco.elem;
    Stack *final = newStack();
    Stack *inicial = (Stack *) x.elem;
    Stack *partida = parteArray(inicial);
    libertaStack(inicial);
    free(inicial);
    int i = 0;
    while (i < partida->sp)
    {
        char operacao[20000];
        copiaStr(str,operacao);
        Stack *parte = (Stack *) (partida->stack[i]).elem;
        recebeinput(operacao,parte,vars);
        juntaStacks(final,parte);
        free(parte->stack);
        free(parte);
        i++;
    }
    free(partida->stack);
    free(partida);
    free(str);
    STACK_ELEM resultado;
    resultado.t = ARR;
    resultado.elem = final;
    if (cond)
    {
        STACK_ELEM aux = arrParaSTR(resultado);
        libertaElemento(resultado);
        resultado = aux;
    }
    push(s,resultado);    
    return 1;
}

int filtra (Stack *s,char *parte,STACK_ELEM vars[])
{
    if (s->sp < 2 || strcmp(parte,",")) return 0;
    STACK_ELEM opera = pop(s);
    STACK_ELEM arraystr = pop(s);
    // po enquanto filtrar apenas arrays
    if (opera.t != BLK || (arraystr.t != ARR && arraystr.t != STR))
    {
        push(s,arraystr);
        push(s,opera);
        return 0;
    }
    STACK_ELEM resultado;
    resultado.t = arraystr.t;
    if (arraystr.t == ARR)
    {
        STACK_ELEM original;
        original = duplicaElemento(arraystr);
        Stack *inicial = (Stack *) original.elem;
        push(s,arraystr);
        push(s,opera);
        aplicarBloco(s,"%",vars);
        STACK_ELEM condicoes = pop(s);
        Stack *novo = newStack();
        Stack *arrCond = (Stack *) condicoes.elem;
        int i = 0;
        while (i < arrCond->sp)
        {
            if (valorLogico(arrCond->stack[i]))
            {
                push(novo,inicial->stack[i]);
            }
            else
            {
                libertaElemento(inicial->stack[i]);
            }
            i++;
        }
        libertaElemento(condicoes);
        free(inicial->stack);
        free(inicial);
        resultado.elem = novo;
    }

        else
    {
        STACK_ELEM y = strParaARR(arraystr);
        push(s,y);
        push(s,opera);
        aplicarBloco(s,"%",vars);
        STACK_ELEM condicoes = pop(s);
        char *str = (char *) arraystr.elem;
        Stack *arrCond = (Stack *) condicoes.elem;
        int i = 0,j =0;
        while (i < arrCond->sp)
        {
            if (valorLogico(arrCond->stack[i]))
            {
                str[j] = str[i];
                j++;
            }
            i++;
        }
        libertaElemento(condicoes);
        str[j] = '\0';
        resultado.elem = str;
        // filtrar stirngs em falta
    }
    push(s,resultado);
    return 1; 

}

int ordena (Stack *s,char* parte, STACK_ELEM vars[])
{
    if (s->sp < 2 || strcmp(parte,"$")) return 0;

    STACK_ELEM opera = pop(s);
    STACK_ELEM arraystr = pop(s);
    // po enquanto filtrar apenas arrays
    if (opera.t != BLK || (arraystr.t != ARR && arraystr.t != STR))
    {
        push(s,arraystr);
        push(s,opera);
        return 0;
    }
    STACK_ELEM resultado;
    resultado.t = arraystr.t;
    if (arraystr.t == ARR)
    {
        STACK_ELEM original;
        original = duplicaElemento(arraystr);
        Stack *inicial = (Stack *) original.elem;
        push(s,arraystr);
        push(s,opera);
        aplicarBloco(s,"%",vars);
        STACK_ELEM condicoes = pop(s);
        Stack *arrCond = (Stack *) condicoes.elem;
        int i = 1;
        while (i < arrCond->sp)
        {
            int j =i;
            STACK_ELEM auxC = arrCond->stack[i];
            STACK_ELEM auxO = inicial->stack[i];
            while (j>0 && menorElem(auxC,arrCond->stack[j-1]))
            {
                arrCond->stack[j] = arrCond->stack[j-1];
                inicial->stack[j] = inicial->stack[j-1];
                j--;                
            }
            arrCond -> stack[j] = auxC;
            inicial -> stack[j] = auxO;
            i++;
        }
        libertaElemento(condicoes);
        resultado.elem = inicial;
    }

    else
    {
        STACK_ELEM y = strParaARR(arraystr);
        push(s,y);
        push(s,opera);
        aplicarBloco(s,"%",vars);
        STACK_ELEM condicoes = pop(s);
        char *str = (char *) arraystr.elem;
        Stack *arrCond = (Stack *) condicoes.elem;
        int i = 1;
        while (i < arrCond->sp)
        {
            int j = i;
            STACK_ELEM auxC = arrCond->stack[i];
            char carater = str[i];
            while (j>0 && menorElem(auxC,arrCond->stack[j-1]))
            {
                arrCond->stack[j] = arrCond->stack[j-1];
                str[j] = str[j-1];
                j--;                
            }
            arrCond -> stack[j] = auxC;
            str[j] = carater;
            i++;
        }
        libertaElemento(condicoes);
        resultado.elem = str;
        // filtrar stirngs em falta
    }
    push(s,resultado);
    return 1; 


}


/*
a truthy basicamente repete a funçao do bloco até o ultimo elemento, ou seja, 
o elemento do pop, ser negativo, igual a 0
*/
int truthy(Stack *s, char *parte,STACK_ELEM vars[])
{
    if (s->sp < 2 || strcmp(parte,"w")) return 0;
    int truthy=1;
    // esta parte trata de garantir que a operaçao, bloco, não é alterado
    char auxiliar[20000];
    STACK_ELEM operacao=pop(s);
    copiaStr(operacao.elem,auxiliar);
    while(truthy==1 && s->sp>0) 
    {   
        recebeinput(auxiliar,s,vars);
        STACK_ELEM x = pop(s);
        if (valorLogico(x)==0) truthy=0;
        copiaStr(operacao.elem,auxiliar);
    }
    free(operacao.elem);
    return 1;
}

int guiao5(Stack *s,char *parte,STACK_ELEM vars[])
{
    return (executaBloco(s,parte,vars) || fold(s,parte,vars) || filtra(s,parte,vars) || 
    aplicarBloco(s,parte,vars) || ordena(s,parte,vars) || truthy(s,parte,vars));
}

int inputOutput (Stack *s,char * parte)
{
    return (lerlinha(s,parte) || lertodaslinhas(s,parte) || imprimeTopo(s,parte));
}
/*
    função que recebe a operação a ser executada que esta delimitada por espaços.
*/
int usar (Stack *s,char * parte,STACK_ELEM vars[])
{
    return( guiao5(s,parte,vars) || guiao1(s,parte)  || guiao3(s,parte,vars) || guiao2(s,parte) || guiao4(s,parte) 
    || inputOutput(s,parte) || conversoes (s,parte) || insere (s, parte));
}

int recebeinput (char parte[],Stack *s,STACK_ELEM vars[])
{
    int i;
    int semfim = 1;
    char subParte [20000];
    while (parte[0] != '\n' && parte[0] != '\0')
    {
        simplificaInput(parte);
        if (parte[0] == '[')
        {
            int parentesis = 1;
            parte++;
            for (i=0; parentesis ;i++,parte++)//parentesis != 0
            {
                if (parte[0]=='[')
                {
                    parentesis++;
                }
                else if (parte[0]==']')
                {
                    parentesis--;
                }
                if (parentesis) subParte[i] = parte[0];
            }
            subParte[i] ='\0';
            Stack *r = newStack();
            semfim = recebeinput(subParte,r,vars);// tratamento de dados na nova substack
            STACK_ELEM novo;
            novo.t = ARR;
            novo.elem = r;
            push(s,novo);
        }
        else if (parte[0] == '{')
        {
            int chavetas = 1;
            parte++;
            for (i=0; chavetas ;i++,parte++)//parentesis != 0
            {
                if (parte[0]== '{')
                {
                    chavetas++;
                }
                else if (parte[0]=='}')
                {
                    chavetas--;
                }
                if (chavetas) subParte[i] = parte[0];
            }
            subParte[i-1] ='\0';
            // tratamento de dados na nova substack
            STACK_ELEM novo;
            novo.t = BLK;
            novo.elem = subParte;
            push(s,duplicaElemento(novo));
        }
        else if (parte[0] == '"') // introduzir string
        {
            parte++;
            for (i = 0; parte[0] != '"';i++,parte++)
            {
               subParte[i] = parte[0];
            }
            subParte[i] = '\0';
            insereStr(s,subParte);
            parte++;
        }
        else
        {
            char c = parte[0];
            i=0;
            while (c != '\0' && c != '\n' && c != ' ')
            {
                subParte[i] = c;
                parte++;i++;
                c = parte[0];
            }
            subParte [i] = '\0';
            if (i > 0) 
            {
                semfim = usar(s,subParte,vars);
            }
        }
    }
    return semfim;
}
