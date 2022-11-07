#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "guiao4.h"

/**
    A função @junta junta 2 arrays, ou adiciona um elemento ao topo ou início de um array, junta strings,
    ou adiciona um carater ao inicio ou fim da string. Colocando assim no topo da stack o respetivo array ou string.
*/

int junta (Stack *s, char *operacao)
{
    if (s->sp< 2 || strcmp (operacao, "+")!=0) return 0;
    STACK_ELEM x=pop (s);
    STACK_ELEM y=pop(s);
    if (x.t == ARR && y.t == ARR)
    {
        // Stack *nova = newStack();
        Stack *arr1 = y.elem;
        Stack *arr2 = x.elem;
        juntaStacks(arr1,arr2);
        free (x.elem);
    
    }
    else if (y.t == ARR) ///o penúltimo é array mas o último não.
    {
        push ((Stack *) y.elem,x);
    }
    else if (x.t == ARR) /// o penúltimo é array mas o último não.
    {
        Stack *nova = newStack();
        Stack *arr2 = (Stack *) x.elem;
        push(nova,y);
        juntaStacks(nova,arr2);
        free (x.elem);
        y.elem = nova;
        y.t = ARR;
    }
    else if (x.t == STR && y.t == STR)
    {
        char *str2 = (char *) x.elem;
        char *str1 = (char *) y.elem;
        for (int i=0,j = strlen(str1); str2[i] != '\0' ;i++,j++)
            str1[j] = str2[i];
        free (x.elem);     
    }
    else if (y.t == STR && x.t == CHAR)
    {
        char *str = malloc(1000* sizeof(char));
        copiaStr((char *) y.elem, str);
        int t = strlen(str);
        str[t] = stckToChar (x);
        str[t+1] = '\0';
        free (y.elem);
        y.elem = str;
    }
    else if (x.t == STR && y.t == CHAR)
    {
        char *str = malloc(1000* sizeof(char));
        copiaStr ((char *) x.elem,str+1);
        str[0] = stckToChar(y);
        free (y.elem);
        y.elem = str;
        y.t = STR;
    }
    else //operaçoes com blocos por defenir.
    {
        push(s,y);push(s,x);
        return 0;
    }
    push (s,y);
    return 1;

}

/**
    A função @concatena põe na stack n vezes (sendo n o valor do topo da stack) o conteúdo
    da penúltima posição da stack.
*/
int concatena (Stack *s, char *operacao)
{
    if (s->sp < 2 || strcmp (operacao, "*")!=0) return 0;
    STACK_ELEM vezes = pop (s);
    STACK_ELEM x = pop (s);
    if (vezes.t != LONG && vezes.t != DBL && vezes.t != CHAR)
    {
        push(s,x);push(s,vezes);
        return 0;
    }
    long repetir = stckToLong(vezes); /// número de vezes  concatenar strings ou arrays
    if (x.t == STR)
    {
        char *nova = malloc(1000*sizeof(char));
        int i =0;
        char *str = (char *) x.elem;
        int t = strlen(str);
        while (repetir > i)
        {
            copiaStr(str,nova+(i*t));
            i++;
        }
        free(x.elem);
        x.elem = nova;
    }
    else if (x.t == ARR)
    {
        Stack *nova = newStack();
        while (repetir > 0)
        {
            STACK_ELEM repetido = duplicaElemento(x);
            Stack *r = (Stack *) repetido.elem;
            juntaStacks(nova,r);
            repetir--;
            free(repetido.elem);
        }
        libertaStack ((Stack *) x.elem);
        free(x.elem);
        x.elem = nova;
       
    }
    else // nao sabemos o que temos de fazer se for um bloco
    {
        push(s,x);push(s,vezes);
        return 0;
    }
    push(s,x);
    
    return 1;
}

/**
    A função @procura, se o último elemento for um número n e o penúltimo um array ou string,
    coloca o n-ésimo elemento no topo da stack, com um pointer diferente mas conteudo igual.
*/

int procura (Stack *s, char *operacao)
{
    if (s->sp <2 || strcmp (operacao,"=")!=0) return 0;
    STACK_ELEM elemento=pop (s);
    STACK_ELEM y=pop (s);
    if (elemento.t == BLK || elemento.t == ARR || elemento.t ==STR || (y.t != STR && y.t != ARR))
    {
        push(s,elemento);
        return 0;
    }
    long pos = stckToLong(elemento);
    STACK_ELEM resultado;
    if (y.t ==ARR)
    {
        Stack *nova = (Stack *) y.elem;
        /* esta mal porque vamos ter 2 elementos com o mesmo apontador(problemas com frees e possiveis operaçoes)
        resultado = nova->stack[stckToLong(elemento)];
        */
        STACK_ELEM x =nova->stack[pos];
        resultado = duplicaElemento(x);
        libertaStack (nova);
    }
    if (y.t ==STR)
    {
        char *str = (char *) y.elem;
        resultado.t = CHAR;
        char *aux = (char *) malloc(sizeof(char));
        *aux = str[pos];
        resultado.elem = aux;
    }
    push (s,resultado);
    free (y.elem);
    return 1;
}

/**
    A função @colocatodos se o último elemento da stack for um array (outra stack) desfaz esta e coloca todos os
    seus elementos na stack principal por ordem.
*/

int colocatodos(Stack *s,char *operacao)
{
    if (s->sp<1 || strcmp(operacao,"~")!=0) return 0;
    STACK_ELEM x= pop (s);
    if (x.t!=ARR)
    {
        push(s,x);
        return 0;
    }
    Stack *teste = (Stack *) x.elem;
    juntaStacks(s,teste);
    free(x.elem);
    return 1;
}

/**
    Se o último elemento da stack for um array (outra stack) ou string, remove esta e coloca no topo da stack o seu tamanho,
    libertando todo o espaço ocupado por este. Caso contrário cria um array com a range do último elemento.
*/

int tamanhoourange (Stack *s, char *operacao)
{
    if (s->sp<1 || strcmp(operacao,",")!=0) return 0;
    STACK_ELEM x = pop(s);
    if (x.t == BLK)
    {
        push(s,x);
        return 0;
    }
    STACK_ELEM resultado;
    if (x.t==ARR)   ///caso seja um array, calcula o seu tamanho e devolve-o para a stack.
    {
        resultado.t=LONG;
        long *tamanho =(long *) malloc(sizeof(long));
        Stack *array = (Stack *) x.elem;
        *tamanho = array->sp; 
        resultado.elem= tamanho;
        libertaStack(array);
        free(x.elem);
    }
    else  if(x.t == STR)  ///introduzir o tamanho da string no array.
    {
        resultado.t=LONG;
        long *tamanho =(long *) malloc(sizeof(long));
        *tamanho = strlen((char *) x.elem);
        resultado.elem = tamanho;
        free(x.elem);
    }
    else
    {
        resultado.t = ARR;
        resultado.elem = newStack();
        long maximo = stckToLong(x);
        int i;
        STACK_ELEM novo;
        novo.t = LONG;
        for (i =0;i<maximo;i++)
        {
            long *e = (long *) malloc (sizeof(long));
            *e = i;
            novo.elem = e;
            push((Stack *) resultado.elem,novo);

        }        
    }
    push(s,resultado);
    return 1;
}

/**
    A função @removeElemUlt caso o último elemento da stack for um array, remove lhe o último elemento e coloca-o na stack principal. Se for uma string,
    cria um elemento de stack , com o conteúdo do último carater desta, coloca-o na stack e encurta a string.
*/

int removeElemUlt (Stack *s, char *operacao)
{
    if (s->sp<1 || strcmp(operacao,")")!=0) return 0;
    STACK_ELEM x = pop(s);
    STACK_ELEM ultElem;
    if (x.t == ARR)
    {

        STACK_ELEM y = pop((Stack *) x.elem);
        ultElem = y;
    }
    else if (x.t ==STR)
    {
        char *str = (char *) x.elem;
        ultElem.t = CHAR;
        ultElem.elem = malloc(sizeof(char));
        int i;
        for (i=0;str[i+1]!='\0';i++);
        *(char *) ultElem.elem = str[i];
        str[i] = '\0';
    }
    else
    {
        push(s,x);
        return 0;
    }
    push (s,x);
    push (s,ultElem);
    return 1;
}

/**
    A função @removeElemPrim remove o primeiro elemento da string/array que está no topo da Stack, e coloca-o
    separadamente no topo da stack.
*/

int removeElemPrim (Stack *s, char *operacao)
{
    if (s->sp<1 || strcmp(operacao,"(")!=0) return 0;
    STACK_ELEM x = pop(s);
    STACK_ELEM primElem;
    if (x.t == ARR)
    {
        Stack *arr = (Stack *) x.elem;
        
        primElem = arr->stack[0]; /// buscar o primeiro elemento da stack.
        arr->sp--;
        for (int i= 0;i < arr->sp;i++)
        {
            arr->stack[i] = arr->stack[i+1];
        }
        /// puxar a stack 1 elemento para trás.
    }
    else if (x.t ==STR)
    {
        char *str = (char *) x.elem;
        primElem.elem = malloc(sizeof(char));
        primElem.t = CHAR;
        *(char *)primElem.elem = str[0];
        copiaStr (str+1,str); /// avançar com a string.
    }
    push (s,x);
    push (s,primElem);
    return 1;
}
/**
    A função @procuraSubString procura todas as subtrings na string presente no topo da stack.
*/
int procurarSubString (Stack *s, char *operacao)
{
    // funçao nao funcional
    if (s->sp<2 || strcmp(operacao,"#")!=0) return 0;
    STACK_ELEM x = pop(s);
    STACK_ELEM y = pop(s);
    STACK_ELEM resultado;
    if(x.t==CHAR)
    {
        x.elem=stckToSTR(x);
        x.t=STR;
    }
    resultado.t = LONG;
    long *tamanho = (long *) malloc(sizeof(long));
    if (x.t != STR || y.t != STR)
    {
        push(s,y);
        push(s,x);
        return 0;
    }
    char *str2 = (char *) x.elem;
    char *str1 = (char *) y.elem;
    int i, j, h;
    for (i = 0;str1[i]!='\0';i++)
    {
        for (j = 0,h = i;str2[j]!='\0' && str2[j]==str1[h] && str1[h]!='\0';j++,h++);
        if (str2[j]=='\0')
        {
            *tamanho = i;
            resultado.elem = tamanho;
            push(s,resultado);
            free(x.elem);
            free(y.elem);
            return 1;
        }
    }
    *tamanho = -1;
    resultado.elem = tamanho;
    push(s,resultado);
    free(x.elem);
    free(y.elem);
    return 1;
}

/**
    A função @buscarPriElemCarat
*/

int buscarPriElemCarat (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,"<")!=0) return 0;
    STACK_ELEM x = pop(s);
    if (x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);
        return 0;
    }
    STACK_ELEM y = pop(s);
    if (y.t != STR && y.t != ARR)
    {
        push(s,x);
        push(s,y);
        return 0;
    }
    long n = stckToLong(x);
    if (y.t == STR)
    {
        char *str1 = (char *) y.elem;
        int i;
        for (i=0;str1[i]!='\0' && n>0;i++,n--);
        str1[i] = '\0';
        free(x.elem);
        push(s,y);
    }
    else
    {
        Stack *arr = (Stack *) y.elem;
        if (arr->sp < n)
        {
            push(s,y);
        }
        else
        {
            arr->sp = n;
            push(s,y);
        }
    }
    return 1;
}


/**
    A função @buscarUltElemCarat
*/
int buscarUltElemCarat (Stack *s, char *operacao)
{
    if (s->sp<2 || strcmp(operacao,">")!=0) return 0;
    STACK_ELEM x = pop(s);
    if (x.t == STR || x.t == ARR || x.t == BLK)
    {
        push(s,x);
        return 0;
    }
    
    STACK_ELEM y = pop(s);
    if (y.t != STR && y.t != ARR)
    {
        push(s,x);
        push(s,y);
        return 0;
    }
    long n = stckToLong(x);

    if (y.t == STR)
    {
        char *str1 = (char *) y.elem;
        int tamanho = strlen(str1);
        int retira = tamanho - n;
        int i;
        if (tamanho<=n)
        {
            push(s,y);
            push(s,x);
            return 0;
        }
        for (;retira>0;retira--)
        {
            for (i=0;str1[i]!='\0';i++)
            {
                str1[i] = str1[i+1];
            }
        }
        free(x.elem);

    }
    else
    {
        STACK_ELEM z;
        Stack *arr = (Stack *) y.elem;
        if (arr->sp < n)
        {
            push(s,y);
            push(s,x);
            return 0;
        }
        else
        {
            z.t = ARR;
            Stack *novo =newStack();
            z.elem = novo;
            if (n > arr->sp)
            {
                push(s,y);
                push(s,x);
                return 0;
            }
            int retira = arr->sp - n;
            for (int n = retira;n<arr->sp;n++)
            {
                STACK_ELEM elemento = arr->stack[n];
                push(novo,elemento);
            }
            libertaElemento(x);
            push(s,z);
        }
    }
    return 1;
}

/**
    A função @comparaStr vai auxiliar a que separa em substrings e por newLines para ser mais fácil
    fazer os teste de igualdade!
*/

int comparaStr(int i,char *str, char *delim)
{
    char strTeste[100];
    int x,aux = strlen(delim);
    for(x=0;x<aux;x++) /// mede o tamanho da string à qual vamos quere fazer divisão.
    {
        strTeste[x]=str[i++]; /// cria uma string a partir da original com o tamanho que foi mencionado a cima.
    }
    strTeste[x]='\0';
    if(strcmp(delim,strTeste)==0) return 1; /// depois de criada a string testa se sao iguais e se deve ser feita a divisao em substrings.
    return 0;
}

/**
    A função @subString
*/
int subString(Stack *s,char *operacao)
{
    // função não funcional
    if (s->sp<2 || strcmp(operacao,"/")!=0) return 0;      // teste normal
    STACK_ELEM y = pop(s);
    STACK_ELEM x = pop(s);
    STACK_ELEM subStr,array; // isto é o que vai armazenar as novas subStrings para dar push
    /* provavel origem do erro
    char originalStr[100], divideStr[100]; // estas serao as strings correspondentes a x.elem e y.elem respetivamente
    copiaStr(x.elem,originalStr);copiaStr(y.elem,divideStr); // copia para os endereços criados anteriormente
    */
    char *originalStr = (char *) x.elem;
    char *divideStr = (char *) y.elem;
    // forma mais simples
    array.t = ARR;
    array.elem = newStack();
    char newSubStr[20000]; // isto é so para criar as novas strings e depois poder copiar para a subStr.elem e dar push
    subStr.t=STR;
    subStr.elem = newSubStr;
    int j=0;
    /*
    de forma resumida este for é basico e serve para criar as novas subStrings a cada vez que encontramos
    os elementos iguais aos do y.elem
    */
    for (int i=0;originalStr[i]!='\0';i++)
    {
        if(comparaStr(i,originalStr,divideStr))
        {
            newSubStr[j]='\0';
            if (j) push((Stack *) array.elem,duplicaElemento(subStr));
            j=0;
            i+=strlen(divideStr); // isto serve para o i ir acompanhando a string e fazer a correspondencia certa
        }
        newSubStr[j]=originalStr[i];
        j++;
    }
    newSubStr[j] = '\0';
    if (newSubStr[0] != '\0')
        push((Stack *) array.elem,duplicaElemento(subStr));
    push(s,array);
    free(originalStr);
    free(divideStr);
    return 1;
}

/**
    A função @newLineDivide
*/

int newLineDivide(Stack *s,char *operacao)
{
    if (s->sp<1 || strcmp(operacao,"N/")!=0) return 0;  
       /// teste normal.
    STACK_ELEM x = pop(s);
    if (x.t != STR)
    {
        push(s,x);
        return 0;
    }
    STACK_ELEM array;
    array.t = ARR;
    Stack *aux = newStack();
    if (aux == NULL)
    {
        push(s,x);
        return 0;
    }
    STACK_ELEM subStr,y; /// isto é o que vai armazenar as novas subStrings para dar push.
    char *originalStr = (char *) x.elem; /// estas serão as strings correspondentes a x.elem (tems de usar malloc e strings de tamanho 1000 so pq sim).
    subStr.t=STR;
    int j=0;
    char newSubStr [20000];
    subStr.elem = newSubStr;
    /// isto é so para criar as novas strings e depois poder copiar para a subStr.elem e dar push.
    ///de forma resumida este for é basico e serve para criar as novas subStrings a cada vez que encontramos \n.
    for (int i=0;originalStr[i]!='\0';i++)
    {
        if(originalStr[i] == '\n')
        {
            newSubStr[j]='\0';
            for (; originalStr[i+1] == '\n'; i++);
            ///  avançar a string todos os \n seguidos.
            j=0;
            y  = duplicaElemento (subStr);
            push(aux,y);
        }
        else
        {
            newSubStr[j]=originalStr[i];
            j++;
        }
    }
    newSubStr[j] = '\0';
    if (j) /// se a string nao for vazia,ou seja, neste caso se o último elemento da string for '\n',coloca-la no array.
    {
        y  = duplicaElemento (subStr);   
        push(aux, y);
    }
    array.elem = aux;
    push(s,array);
    return 1;
}

/**
    A função @whiteSpaces
*/
//esta supostamente funciona mas nao consigo resolver o problema do segmentation fault
int whiteSpaces(Stack *s, char *operacao)
{
    if (s->sp<1 || strcmp(operacao,"S/")!=0) return 0;      // teste normal
    STACK_ELEM x = pop(s);
    if (x.t != STR)
    {
        push(s,x);
        return 0;
    }
    STACK_ELEM array;
    array.t =  ARR;
    Stack *aux = newStack();
    STACK_ELEM novo;            /// cria a nova variavel para colocar na stack.
    char *prov = (char *) malloc(sizeof(char) *20000);
    char *original =(char *) x.elem;
    novo.t=STR;         /// colocamos ja o tipo pois ja sabemos que vai ser string.
    novo.elem = prov;
    int j,i;
    for( i = j =0; original[i] != '\0';i++)
    {
        if (original [i] == ' ' || original[i] == '\n')
        {
            prov[j] = '\0';
            push(aux,duplicaElemento(novo));
            j = 0;
            for (; original[i+1] == ' '; i++);
           // prov[0] = '\0';
        }
        else
        {
            prov[j] = original[i];
            j++;
        }
    }
    if (j)
        push(aux,novo);
    else free(prov);

    array.elem  = aux;
    push (s,array);
    free(x.elem);
    return 1;
}

/**
    A função @guiao4 tem todas as funções utilizadas no guião 4.
*/
int guiao4 (Stack *s,char *operacao)
{
    return (newLineDivide(s,operacao) || subString(s,operacao) || whiteSpaces(s,operacao) || junta(s,operacao) || tamanhoourange(s,operacao) || colocatodos(s,operacao) || procura (s,operacao) || 
    concatena (s,operacao) || removeElemUlt(s,operacao) || removeElemPrim(s,operacao) || procurarSubString(s,operacao) 
    || buscarPriElemCarat(s,operacao) || buscarUltElemCarat(s,operacao));
}