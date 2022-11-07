/// biblioteca com todas as funções necessárias para o guiao4.

int igual (Stack *s, char *operacao);

int menor (Stack *s, char *operacao);

int maior (Stack *s, char *operacao);

int menorDosDois (Stack *s, char *operacao);

int maiorDosDois (Stack *s, char *operacao);

int eShortcut (Stack *s, char *operacao);

int ouShortcut (Stack *s, char *operacao);

int nao (Stack *s, char *operacao);

int ifThenElse (Stack *s, char *operacao);

int logica (Stack *s,char *operacao);

int guiao3(Stack *s, char *operacao,STACK_ELEM vars[]);

int operaVars (Stack *s,char *operacao,STACK_ELEM vars[]);

void inicializaVars (STACK_ELEM vars[]);

int introduzVar(Stack *s,char *operacao, STACK_ELEM vars[]);

int posEfetiva (char c);

int alteraVars (Stack *s,char *operacao,STACK_ELEM vars[]);