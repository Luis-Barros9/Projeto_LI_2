/// biblioteca com todas as funções necessárias para o projeto, juntamente com os tipos utilizados.

typedef enum
{
    CHAR,
    LONG,
    DBL,
    STR,
    ARR,
    BLK
} TYPE;

typedef struct
{
    TYPE t;
    void *elem;
} STACK_ELEM;

typedef struct
{
    int sp;            /// número de elementos dentro da stack
    STACK_ELEM *stack; /// array de elementos de tamanho dinâmico
    int size;          /// tamanho da stack
} Stack;

Stack *newStack();
void push(Stack *sp, STACK_ELEM n);
STACK_ELEM pop(Stack *sp);
long stckToLong(STACK_ELEM x);
char stckToChar(STACK_ELEM x);
double stckToDbl(STACK_ELEM x);
char *stckToSTR(STACK_ELEM x);

void copiaStr(char *original, char *destino);
void simplificaInput(char v[]);
void juntaStacks(Stack *nova, Stack *arr1);
void libertaStack(Stack *s);
STACK_ELEM duplicaElemento(STACK_ELEM x);
void libertaElemento(STACK_ELEM x);
Stack *parteArray(Stack *s);
int valorLogico(STACK_ELEM x);
STACK_ELEM strParaARR(STACK_ELEM x);
STACK_ELEM arrParaSTR(STACK_ELEM x);
int menorElem(STACK_ELEM x, STACK_ELEM y);
void duplicaVARS(STACK_ELEM original[], STACK_ELEM copia[]);
void libertaVARS(STACK_ELEM vars[]);
