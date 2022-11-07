/// biblioteca com todas as funções necessárias para o guiao5.

int insereInt (Stack *s,char *operacao);
int insereChar (Stack *s,char *operacao);
void insereStr (Stack *s,char *operacao);
int insere(Stack *s, char *operacao);
int recebeinput (char parte[],Stack *s,STACK_ELEM vars[]);
int usar (Stack *s,char * parte,STACK_ELEM vars[]);
int lerlinha (Stack *s,char *operacao);
int lertodaslinhas (Stack *s,char *operacao);
void imprimeSt (Stack *s);
int filtra (Stack *s,char *parte,STACK_ELEM vars[]);
int aplicarBloco(Stack *s,char *parte,STACK_ELEM vars[]);
int executaBloco (Stack *s,char *operacao,STACK_ELEM vars[]);


