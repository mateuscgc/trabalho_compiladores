#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ============================================================================
// =========================== Estruturas de dados ============================
// ============================================================================
typedef enum {
   DECLARE = 0, AS = 1, NUMBER = 2, LETTER = 3, PUT = 4, IN = 5, IF = 6, THEN = 7, ELSE = 8, FOREACH = 9,
   DO = 10, FOR = 11, FROM = 12, TO = 13, RESIZE = 14, READ = 15, PRINT = 16, COMMA = 17, DOT = 18,
   OPENCOLCHETES = 19, CLOSECOLCHETES = 20, PLUS = 21, MINUS = 22, TIMES = 23, DIVIDE = 24, MOD = 25,
   OPENPARENTESES = 26, CLOSEPARENTESES = 27, LESSTHAN = 28, GREATERTHAN = 29, LESSEQUALTHAN = 30,
   GREATEREQUALTHAN = 31, EQUAL = 32, DIFERENT = 33, ID = 34, CONSTNUM = 35, CONSTCHAR = 36, CONSTSTRING = 37,
   
   //Não terminais
   SS = 38, S = 39, $ = 40, COMANDOS = 41, COMANDO = 42, VAZIO = 43, LISTAIDS = 44, TIPO = 45, EXPARIT = 46,
   T = 47, F = 48, E = 49, SENAO = 50, OPRREL = 51, ESCRITA = 52, PUTAVEL = 53, INDEX = 54, IMPRIMIVEL = 55, NINDEX = 56
} tokenType;


typedef struct slistaToken {
    tokenType tokenVal;
    char * stringVal;
    int numVal;
    struct slistaToken *prox;
} nolistaToken, *listaToken;

typedef struct spilhaToken {
    nolistaToken token;
    int estado;
    struct spilhaToken *prox;
} nopilhaToken, *pilhaToken ;

typedef struct producao {
    int p; // não terminal antes da seta
    int *q; // vetor dos simbolos depois da seta
    int n // número de simbolos depois da
} prod;

// ============================================================================
// ============================= Lista de Tokens ==============================
// ============================================================================

void inicializar (listaToken *l) {
    *l = NULL;
}

_Bool formString(char *e, char **s, int i, int f){
    int length = f - i + 1;
    int j;
    if(!((*s) = (char*) malloc(length + 1)))
        return false;
    for (j = 0; j < length; j++)
        (*s)[j] = e[i+j]; 
    (*s)[j] = 0;
    return true;
}
}

_Bool inseriToken(listaToken *l, tokenType tv, char *sv) {
    listaToken aux, p, pant;
    if (!(aux = (nolistaToken*) malloc (sizeof(nolistaToken))))
        return false;

    aux->tokenVal = tv;
    aux->stringVal = sv;
    if (tv == CONSTNUM)
        aux->numVal = atoi(sv);
    aux->prox = NULL;

    for (p = *l, pant = NULL; (p); pant = p, p = p->prox);
    if(pant == NULL){
        *l = aux;
    } else{
        pant->prox = aux;
    }
    return true;
}

void imprimeLista(listaToken *l){
    listaToken p;
    for(p = *l; (p); p = p->prox)
        printf("tv = %d\nsv = %s\n\n", p->tokenVal, p->stringVal);
}

_Bool compareString(char *s1, char *s2) {
    int i;
    for (i = 0; s2[i] != 0; i++){
        if(toupper(s1[i]) != toupper(s2[i])) 
            return false;
    }
    if (s1[i] != 0)
        return false;
    return true;
}

void salvaTokenIdentificador(char *e, int i , int f, listaToken *l){
    char *stringVal;
    while(!formString(e, &stringVal, i, f));
    switch (strlen(stringVal)){
        case 2:
            if      (compareString(stringVal, "AS"))       { while(!inseriToken(l, AS, stringVal));}
            else if (compareString(stringVal, "IN"))       { while(!inseriToken(l, IN, stringVal));}
            else if (compareString(stringVal, "IF"))       { while(!inseriToken(l, IF, stringVal));}
            else if (compareString(stringVal, "DO"))       { while(!inseriToken(l, DO, stringVal));}
            else if (compareString(stringVal, "TO"))       { while(!inseriToken(l, TO, stringVal));}
            else                                        { while(!inseriToken(l, ID, stringVal));}
        break;
        case 3: 
            if      (compareString(stringVal, "PUT"))      { while(!inseriToken(l, PUT, stringVal));}
            else if (compareString(stringVal, "FOR"))      { while(!inseriToken(l, FOR, stringVal));}
            else                                        { while(!inseriToken(l, ID, stringVal));}
        break;
        case 4:
            if      (compareString(stringVal, "THEN"))     { while(!inseriToken(l, THEN, stringVal));}
            else if (compareString(stringVal, "ELSE"))     { while(!inseriToken(l, ELSE, stringVal));}
            else if (compareString(stringVal, "FROM"))     { while(!inseriToken(l, FROM, stringVal));}
            else if (compareString(stringVal, "READ"))     { while(!inseriToken(l, READ, stringVal));}
            else                                        { while(!inseriToken(l, ID, stringVal));}
        break;
        case 5:
            if      (compareString(stringVal, "PRINT"))    { while(!inseriToken(l, PRINT, stringVal));}
            else                                        { while(!inseriToken(l, ID, stringVal));}
        break;
        case 6:
            if      (compareString(stringVal, "CONSTNUM")) { while(!inseriToken(l, NUMBER, stringVal));}
            else if (compareString(stringVal, "LETTER"))   { while(!inseriToken(l, LETTER, stringVal));}
            else if (compareString(stringVal, "RESIZE"))   { while(!inseriToken(l, RESIZE, stringVal));}
            else                                        { while(!inseriToken(l, ID, stringVal));}
        break;
        case 7:
            if      (compareString(stringVal, "DECLARE"))  { while(!inseriToken(l, DECLARE, stringVal));}
            else if (compareString(stringVal, "FOREACH"))  { while(!inseriToken(l, FOREACH, stringVal));}
            else                                        { while(!inseriToken(l, ID, stringVal));}
        break;
        default :
            while(!inseriToken(l, ID, stringVal));
    } 
}

void salvaTokenSeparadorUnico(char e, listaToken *l){
    char stringVal[2];
    stringVal[0] = e;
    stringVal[1] = 0;
    switch(stringVal[0]) {
        case ',':   while(!inseriToken(l, COMMA,            stringVal)); break;
        case '.':   while(!inseriToken(l, DOT,              stringVal)); break;
        case '[':   while(!inseriToken(l, OPENCOLCHETES,    stringVal)); break;
        case ']':   while(!inseriToken(l, CLOSECOLCHETES,   stringVal)); break;
        case '+':   while(!inseriToken(l, PLUS,             stringVal)); break;
        case '-':   while(!inseriToken(l, MINUS,            stringVal)); break;
        case '*':   while(!inseriToken(l, TIMES,            stringVal)); break;
        case '/':   while(!inseriToken(l, DIVIDE,           stringVal)); break;
        case '%':   while(!inseriToken(l, MOD,              stringVal)); break;
        case '(':   while(!inseriToken(l, OPENPARENTESES,   stringVal)); break;
        case ')':   while(!inseriToken(l, CLOSEPARENTESES,  stringVal)); break;
        case '=':   while(!inseriToken(l, EQUAL,            stringVal)); break;
        case '<':   while(!inseriToken(l, LESSTHAN,         stringVal)); break;
        case '>':   while(!inseriToken(l, GREATERTHAN,      stringVal)); break;
    }
}

// ============================================================================
// ============================= Pilha de tokens ==============================
// ============================================================================

void inicializaPilhaToken (pilhaToken *p){
    *p = NULL;
}

_Bool empilhaToken (pilhaToken *p, nolistaToken token, int estado){
    pilhaToken aux;
    if(!(aux = (nopilhaToken*) malloc (sizeof(nopilhaToken))))
        return false;
    aux->token = token;
    aux->estado = estado;
    aux->prox = (*p);
    (*p) = aux;
    return true;
}

_Bool desempilhaToken (pilhaToken *p, pilhaToken *x) {
    if (!(*p))
        return false;
    (*x) = (*p);
    (*p) = (*p)->prox;
    return true;
}

_Bool topoToken (pilhaToken *p, pilhaToken *x){
    if (!(*p))
        return false;
    (*x) = (*p);
    return true;
}


// ============================================================================
// ============================ Funções auxiliares ============================
// ============================================================================

long getFileSize(FILE *file) {
    long pos = ftell(file);
    long size;
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, pos, SEEK_SET);

    return size;
}

void append(char *s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len+1] = 0;
}

_Bool verificadorCaracteres (char entrada[]) {
    int i;
    for (i = 0; entrada[i] != 0; i++) {
        if(entrada[i] < 9 || (entrada[i] > 10 && entrada[i] < 32) || entrada[i] > 126){
            return false;
        }
    }
    return true;
}

_Bool isNumber(char c) {
    if (c >= 48 && c <= 57)
        return true;
    return false;
}

_Bool isSeparator(char elemento){
    if( elemento == ',' || elemento == '.' || elemento == '[' || elemento == ']'
        || elemento == '+' || elemento == '-' || elemento == '*' || elemento == '/'
        || elemento == '%' || elemento == '(' || elemento == ')' || elemento == '='
        || elemento == '<' || elemento == '>' || elemento == ' ' || elemento == '\t')
            return true;
    return false;
}

void printErroLexico (char *e, int i, int f, int l){
    char aux[f-i+2];
    memcpy (aux, &e[i], f-i+1);
    aux[f-i+1] = 0;
    printf("LINHA %d: %s\n", l, aux);
}


// ============================================================================
// ======================= Funções de Analise Lexica ==========================
// ============================================================================

int checkIdentificador(char *e,int pos, listaToken *lista){
	int aux = pos + 1;
	while (isalpha(e[aux]) || isNumber(e[aux])){
    	aux++;
    }
	salvaTokenIdentificador(e, pos, aux-1, lista);
	return aux - 1;
}
int checkNumber(char *e, int pos, int l,  listaToken *lista){
	int aux = pos + 1;
    bool letter = false;
	while (isNumber(e[aux]) || isalpha(e[aux])) {
		if (isalpha(e[aux]))
            letter = true;
        aux++;
	}

	if (aux - pos > 10 || letter)
        printErroLexico(e, pos, aux-1, l);
    else {
        char *stringVal;
        while(!formString(e, &stringVal, pos, aux - 1));
        printf("%s\n", stringVal);
        while(!inseriToken(lista, NUMBER, stringVal));
    }
    return aux - 1;
}

int checkConstChar(char *e,int pos, int l, listaToken*lista){
	int aux = pos + 1;
    char *stringVal;
    _Bool exception = false;
	while ((e[aux] != '\'' || (e[aux-1] == '\\' && exception == true)) && e[aux] != '\t' && e[aux] != '\n' && e[aux] != 0){
		if(e[aux] == '\\' && exception == false)
            exception = true;
        else
            exception = false;
        aux++;
	}
    int len = aux - pos -1;
	if(e[aux] != '\'' || len >= 3){
        if(e[aux] != 0 && e[aux] != '\t' && e[aux] != '\n')
            printErroLexico(e, pos, aux, l);
        else 
            printErroLexico(e, pos, aux-1, l);
	} else if(len == 2) {
        if (e[aux-1] == 't' || e[aux-1] == 'n' || e[aux-1] == '\\' || e[aux-1] == '\'' || e[aux-1] == '\"') {
            if (e[aux-2] != '\\')
                printErroLexico(e, pos, aux, l);
            else{
                while(!formString(e, &stringVal, pos+1, aux-1));
                printf("%s\n", stringVal);
                while(!inseriToken(lista, CONSTCHAR, stringVal));
            }
        } else {
            printErroLexico(e, pos, aux, l);
        }
    } else if (len == 1){
        if (e[aux-1] == '\\' || e[aux-1] == '\"') {
            printErroLexico(e, pos, aux, l);
        } else {
            while(!formString(e, &stringVal, pos+1, aux-1));
            printf("%s\n", stringVal);
            while(!inseriToken(lista, CONSTCHAR, stringVal));
        }
    } else {
        printErroLexico(e, pos, aux, l);
    }
    if(e[aux] == '\t' || e[aux] == '\n')
        aux--;
    return aux;
}

int checkString(char *e, int pos, int l, listaToken *lista){
    int aux = pos  + 1;
    int len = 0;
    _Bool exception = false;
    _Bool contraBarraInvalido = false;
    _Bool aspasInvalida = false;
    while ((e[aux] != '\"' || (e[aux-1] == '\\' && exception))  && e[aux] != '\t' && e[aux] != '\n' && e[aux] != 0){ // O problema é "\\'" por exemplo
        if ((e[aux] != '\'' && e[aux] != '\"' && e[aux] != 't'  && e[aux] != 'n'  && e[aux] != '\\') || exception == false)
            len++;
        if(exception && e[aux] != 't' && e[aux] != 'n' && e[aux] != '\\' && e[aux] != '\''  && e[aux] != '\"'){
            contraBarraInvalido = true;
        }
        if(!exception && e[aux] == '\''){
            aspasInvalida = true;
        } if(e[aux] == '\\' && !exception){
            exception = true;
        } else {
            exception = false;
        }
        aux++;
    }
    if(e[aux] != '\"' || contraBarraInvalido || aspasInvalida){
        if(e[aux] != 0 && e[aux] != '\t' && e[aux] != '\n'){
            printErroLexico(e, pos, aux, l);
        } else {
            printErroLexico(e, pos, aux-1, l);
        }
    } else if (len > 257){
        printErroLexico(e, pos, aux, l);
    } else {
        char *stringVal;
        while(!formString(e, &stringVal, pos+1, aux-1));
        printf("%s\n", stringVal);
        while(!inseriToken(lista, CONSTSTRING, stringVal));
    }
    if(e[aux] == '\t' || e[aux] == '\n')
        aux--;
    return aux;
}


// ============================================================================
// ======================= Funções de Analise Lexica ==========================
// ============================================================================





// ============================================================================

int main(int argc, char const *argv[]) {
    
    //PRODUÇÕES
    prod producoes[48];
    
    producoes[0].p = SS;
    producoes[0].q = (int*) malloc(2*sizeof(int));
    producoes[0].q[0] = S;
    producoes[0].q[1] = $;
    producoes[0].n = 2;

    producoes[1].p = S;
    producoes[1].q = (int*) malloc(sizeof(int));
    producoes[1].q[0] = COMANDOS;
    producoes[1].n = 1;

    producoes[2].p = COMANDOS;
    producoes[2].q = (int*) malloc(2*sizeof(int));
    producoes[2].q[0] = COMANDO;
    producoes[2].q[1] = COMANDOS;
    producoes[2].n = 2;

    producoes[3].p = COMANDOS;
    producoes[3].q = (int*) malloc (sizeof(int));
    producoes[3].q[0] = VAZIO;
    producoes[3].n = 0;

    producoes[4].p = COMANDO;
    producoes[4].q = (int*) malloc(5*sizeof(int));
    producoes[4].q[0] = DECLARE;
    producoes[4].q[1] = LISTAIDS;
    producoes[4].q[2] = AS;
    producoes[4].q[3] = TIPO;
    producoes[4].q[4] = DOT;
    producoes[4].n = 5;

    producoes[5].p = COMANDO;
    producoes[5].q = (int*) malloc(4*sizeof(int));
    producoes[5].q[0] = RESIZE;
    producoes[5].q[1] = ID;
    producoes[5].q[2] = TO;
    producoes[5].q[3] = EXPARIT;
    producoes[5].q[4] = DOT;
    producoes[5].n = 5;

    producoes[6].p = COMANDO;
    producoes[6].q = (int*) malloc(6*sizeof(int));
    producoes[6].q[0] = PUT;
    producoes[6].q[1] = PUTAVEL;
    producoes[6].q[2] = IN;
    producoes[6].q[3] = ID;
    producoes[6].q[4] = INDEX;
    producoes[6].q[5] = DOT;
    producoes[6].n = 6;

    producoes[7].p = COMANDO;
    producoes[7].q = (int*) malloc(9*sizeof(int));
    producoes[7].q[0] = IF;
    producoes[7].q[1] = EXPARIT;
    producoes[7].q[2] = OPRREL;
    producoes[7].q[3] = EXPARIT;
    producoes[7].q[4] = THEN;
    producoes[7].q[5] = OPENCOLCHETES;
    producoes[7].q[6] = COMANDOS;
    producoes[7].q[7] = CLOSECOLCHETES;
    producoes[7].q[8] = SENAO;
    producoes[7].n = 9;

    producoes[8].p = COMANDO;
    producoes[8].q = (int*) malloc(11*sizeof(int));
    producoes[8].q[0] = FOR;
    producoes[8].q[1] = ID;
    producoes[8].q[2] = INDEX;
    producoes[8].q[3] = FROM;
    producoes[8].q[4] = EXPARIT;
    producoes[8].q[5] = TO;
    producoes[8].q[6] = EXPARIT;
    producoes[8].q[7] = DO;
    producoes[8].q[8] = OPENCOLCHETES;
    producoes[8].q[9] = COMANDOS;
    producoes[8].q[10] = CLOSECOLCHETES;
    producoes[8].n = 11;

    producoes[9].p = COMANDO;
    producoes[9].q = (int*) malloc(9*sizeof(int));
    producoes[9].q[0] = FOREACH;
    producoes[9].q[1] = ID;
    producoes[9].q[2] = INDEX;
    producoes[9].q[3] = IN;
    producoes[9].q[4] = ID;
    producoes[9].q[5] = DO;
    producoes[9].q[6] = OPENCOLCHETES;
    producoes[9].q[7] = COMANDOS;
    producoes[9].q[8] = CLOSECOLCHETES;
    producoes[9].n = 9;

    producoes[10].p = COMANDO;
    producoes[10].q = (int*) malloc(4*sizeof(int));
    producoes[10].q[0] = READ;
    producoes[10].q[1] = ID;
    producoes[10].q[2] = INDEX;
    producoes[10].q[3] = DOT;
    producoes[10].n = 4;

    producoes[11].p = COMANDO;
    producoes[11].q = (int*) malloc(3*sizeof(int));
    producoes[11].q[0] = PRINT;
    producoes[11].q[1] = IMPRIMIVEL;
    producoes[11].q[2] = DOT;
    producoes[11].n = 3;

    producoes[12].p = LISTAIDS;
    producoes[12].q = (int*) malloc(4*sizeof(int));
    producoes[12].q[0] = ID;
    producoes[12].q[1] = NINDEX;
    producoes[12].q[2] = COMMA;
    producoes[12].q[3] = LISTAIDS;
    producoes[12].n = 4;

    producoes[13].p = LISTAIDS;
    producoes[13].q = (int*) malloc(2*sizeof(int));
    producoes[13].q[0] = ID;
    producoes[13].q[1] = NINDEX;
    producoes[13].n = 2;

    producoes[14].p = NINDEX;
    producoes[14].q = (int*) malloc(2*sizeof(int));
    producoes[14].q[0] = OPENCOLCHETES;
    producoes[14].q[1] = CLOSECOLCHETES;
    producoes[14].n = 2;

    producoes[15].p = NINDEX;
    producoes[15].q = (int*) malloc(1*sizeof(int));
    producoes[15].q[0] = VAZIO;
    producoes[15].n = 0;

    producoes[16].p = INDEX;
    producoes[16].q = (int*) malloc(3*sizeof(int));
    producoes[16].q[0] = OPENCOLCHETES;
    producoes[16].q[1] = EXPARIT;
    producoes[16].q[2] = CLOSECOLCHETES;
    producoes[16].n = 3;

    producoes[17].p = INDEX;
    producoes[17].q = (int*) malloc(1*sizeof(int));
    producoes[17].q[0] = VAZIO;
    producoes[17].n = 0;

    producoes[18].p = TIPO;
    producoes[18].q = (int*) malloc(1*sizeof(int));
    producoes[18].q[0] = NUMBER;
    producoes[18].n = 1;

    producoes[19].p = TIPO;
    producoes[19].q = (int*) malloc(1*sizeof(int));
    producoes[19].q[0] = LETTER;
    producoes[19].n = 1;

    producoes[20].p = EXPARIT;
    producoes[20].q = (int*) malloc(3*sizeof(int));
    producoes[20].q[0] = EXPARIT;
    producoes[20].q[1] = PLUS;
    producoes[20].q[2] = T;
    producoes[20].n = 3;

    producoes[21].p = EXPARIT;
    producoes[21].q = (int*) malloc(3*sizeof(int));
    producoes[21].q[0] = EXPARIT;
    producoes[21].q[1] = MINUS;
    producoes[21].q[2] = T;
    producoes[21].n = 3;


    producoes[22].p = EXPARIT;
    producoes[22].q = (int*) malloc(1*sizeof(int));
    producoes[22].q[0] = EXPARIT;
    producoes[22].q[1] = T;
    producoes[22].n = 2;

    producoes[23].p = T;
    producoes[23].q = (int*) malloc(3*sizeof(int));
    producoes[23].q[0] = T;
    producoes[23].q[1] = TIMES;
    producoes[23].q[2] = F;
    producoes[23].n = 3;

    producoes[24].p = T;
    producoes[24].q = (int*) malloc(3*sizeof(int));
    producoes[24].q[0] = T;
    producoes[24].q[1] = DIVIDE;
    producoes[24].q[2] = F;
    producoes[24].n = 3;

    producoes[25].p = T;
    producoes[25].q = (int*) malloc(3*sizeof(int));
    producoes[25].q[0] = T;
    producoes[25].q[1] = MOD;
    producoes[25].q[2] = F;
    producoes[25].n = 3;

    producoes[26].p = F;
    producoes[26].q = (int*) malloc(3*sizeof(int));
    producoes[26].q[0] = OPENPARENTESES;
    producoes[26].q[1] = E;
    producoes[26].q[2] = CLOSEPARENTESES;
    producoes[26].n = 3;

    producoes[27].p = F;
    producoes[27].q = (int*) malloc(4*sizeof(int));
    producoes[27].q[0] = MINUS;
    producoes[27].q[1] = OPENPARENTESES;
    producoes[27].q[2] = E;
    producoes[27].q[3] = CLOSEPARENTESES;
    producoes[27].n = 4;

    producoes[28].p = F;
    producoes[28].q = (int*) malloc(2*sizeof(int));
    producoes[28].q[0] = ID;
    producoes[28].q[1] = INDEX;
    producoes[28].n = 2;

    producoes[29].p = F;
    producoes[29].q = (int*) malloc(3*sizeof(int));
    producoes[29].q[0] = MINUS;
    producoes[29].q[1] = ID;
    producoes[29].q[2] = INDEX;
    producoes[29].n = 3;

    producoes[30].p = F;
    producoes[30].q = (int*) malloc(sizeof(int));
    producoes[30].q[0] = CONSTNUM;
    producoes[30].n = 1;

    producoes[31].p = F;
    producoes[31].q = (int*) malloc(2*sizeof(int));
    producoes[31].q[0] = MINUS;
    producoes[31].q[1] = CONSTNUM;
    producoes[31].n = 2;

    producoes[32].p = F;
    producoes[32].q = (int*) malloc(sizeof(int));
    producoes[32].q[0] = CONSTCHAR;
    producoes[32].n = 1;

    producoes[33].p = F;
    producoes[33].q = (int*) malloc(2*sizeof(int));
    producoes[33].q[0] = MINUS;
    producoes[33].q[1] = CONSTCHAR;
    producoes[33].n = 2;

    producoes[34].p = PUTAVEL;
    producoes[34].q = (int*) malloc(sizeof(int));
    producoes[34].q[0] = EXPARIT;
    producoes[34].n = 1;

    producoes[35].p = PUTAVEL;
    producoes[35].q = (int*) malloc(sizeof(int));
    producoes[35].q[0] = CONSTSTRING;
    producoes[35].n = 1;

    producoes[36].p = SENAO;
    producoes[36].q = (int*) malloc(4*sizeof(int));
    producoes[36].q[0] = ELSE;
    producoes[36].q[1] = OPENCOLCHETES;
    producoes[36].q[2] = COMANDOS;
    producoes[36].q[3] = CLOSECOLCHETES;
    producoes[36].n = 4;

    producoes[37].p = SENAO;
    producoes[37].q = (int*) malloc(sizeof(int));
    producoes[37].q[0] = VAZIO;
    producoes[37].n = 0;

    producoes[38].p = OPRREL;
    producoes[38].q = (int*) malloc(sizeof(int));
    producoes[38].q[0] = LESSTHAN;
    producoes[38].n = 1;

    producoes[39].p = OPRREL;
    producoes[39].q = (int*) malloc(sizeof(int));
    producoes[39].q[0] = GREATERTHAN;
    producoes[39].n = 1;

    producoes[40].p = OPRREL;
    producoes[40].q = (int*) malloc(sizeof(int));
    producoes[40].q[0] = LESSEQUALTHAN;
    producoes[40].n = 1;

    producoes[41].p = OPRREL;
    producoes[41].q = (int*) malloc(sizeof(int));
    producoes[41].q[0] = GREATEREQUALTHAN;
    producoes[41].n = 1;

    producoes[42].p = OPRREL;
    producoes[42].q = (int*) malloc(sizeof(int));
    producoes[42].q[0] = EQUAL;
    producoes[42].n = 1;

    producoes[43].p = OPRREL;
    producoes[43].q = (int*) malloc(sizeof(int));
    producoes[43].q[0] = DIFERENT;
    producoes[43].n = 1;

    producoes[44].p = IMPRIMIVEL;
    producoes[44].q = (int*) malloc(2*sizeof(int));
    producoes[44].q[0] = ID;
    producoes[44].q[1] = INDEX;
    producoes[44].n = 2;

    producoes[45].p = IMPRIMIVEL;
    producoes[45].q = (int*) malloc(sizeof(int));
    producoes[45].q[0] = CONSTNUM;
    producoes[45].n = 1;

    producoes[46].p = IMPRIMIVEL;
    producoes[46].q = (int*) malloc(sizeof(int));
    producoes[46].q[0] = CONSTCHAR;
    producoes[46].n = 1;

    producoes[47].p = IMPRIMIVEL;
    producoes[47].q = (int*) malloc(sizeof(int));
    producoes[47].q[0] = CONSTSTRING;
    producoes[47].n = 1;


    printf("%d\n%d\n%d\n", producoes[0].p, producoes[0].q[0], producoes[0].n);

    char *entrada;

    if (argc > 1)
    {
        FILE *program;
        program = fopen(argv[1], "r");
        if (program){
            entrada = (char *) calloc(getFileSize(program) + 1, sizeof(char));
            char c;
            while ((c = getc(program)) != EOF){
                append(entrada, c);
            }
            append(entrada, '\0');
            fclose(program);
        }
    } else
        return 0;


    if(!verificadorCaracteres(entrada)){
        printf("ARQUIVO INVALIDO!\n");
        return 0;
    }
    
    int nextPos = 0;
    char nextChar;
    int line = 1;
    char *stringVal;

    listaToken l;
    inicializar(&l);
    //Analise Lexica
    while (entrada[nextPos] != 0){
        nextChar = entrada[nextPos];
        if (isalpha(nextChar)) {
            nextPos = checkIdentificador(entrada, nextPos, &l);
        } else if (isNumber(nextChar)) {
            nextPos = checkNumber(entrada, nextPos, line, &l);
        } else if (nextChar == '\''){
            nextPos = checkConstChar(entrada, nextPos, line, &l);
        } else if (nextChar == '\"'){
            nextPos = checkString(entrada, nextPos, line, &l);
        } else if (nextChar == '\n'){
            line++;
        } else if (nextChar == '>'){
            if (entrada[nextPos + 1] == '='){
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                printf("%s\n", stringVal);
                while(!inseriToken(&l, GREATEREQUALTHAN, stringVal));
            } else salvaTokenSeparadorUnico(nextChar, &l);
        } else if (nextChar == '<'){
            if (entrada[nextPos + 1] == '=') {
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                printf("%s\n", stringVal);
                while(!inseriToken(&l, LESSEQUALTHAN, stringVal));
            } else if (entrada[nextPos + 1] == '>'){
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                printf("%s\n", stringVal);
                while(!inseriToken(&l, DIFERENT, stringVal));
            } else salvaTokenSeparadorUnico(nextChar, &l);
        } else if (isSeparator(nextChar)){
            salvaTokenSeparadorUnico(nextChar, &l);
        } else if (nextChar != ' ' && nextChar != '\t')
            printErroLexico(entrada, nextPos, nextPos, line);
        nextPos++;
    }

    imprimeLista(&l);

    //Analise Sintatica
    /*empilha $entrada
    while (topoPEntrada) {
        acaoTabela(topoPEntrada, estado) ----> retorna uma string com "eX" , "rX" , "ac" , "n"
        if (eX) {
            empilhaPTrabalho topoPEntrada e estado ---->
            estado = x;
        } else if (rX) {
            consulta num de tokens comidos pela redução
            desempilhaPTrabalho num tokens
            estado = estado de PTrabalho[num]
            empilhaPEntrada tokens gerados pela redução
        } else if (ac){
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
    }   */
    return 0;
}