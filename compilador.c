#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ============================================================================
// ============================= Lista de Tokens ==============================
// ============================================================================
typedef enum {
   DECLARE = 0, AS = 1, NUMBER = 2, LETTER = 3, PUT = 4, IN = 5, IF = 6, THEN = 7, ELSE = 8, FOREACH = 9,
   DO = 10, FOR = 11, FROM = 12, TO = 13, RESIZE = 14, READ = 15, PRINT = 16, COMMA = 17, DOT = 18,
   OPENCOLCHETES = 19, CLOSECOLCHETES = 20, PLUS = 21, MINUS = 22, TIMES = 23, DIVIDE = 24, MOD = 25,
   OPENPARENTESES = 26, CLOSEPARENTESES = 27, LESSTHAN = 28, GREATERTHAN = 29, LESSEQUALTHAN = 30,
   GREATEREQUALTHAN = 31, EQUAL = 32, DIFERENT = 33, ID = 34, CONSTNUM = 35, CONSTCHAR = 36, CONSTSTRING = 37
} tokenType;


typedef struct slistaToken {
    tokenType tokenVal;
    char * stringVal;
    int numVal;
    struct slistaToken *prox;
} nolistaToken, *listaToken;


void inicializar (listaToken *l) {
    *l = NULL;
}

// _Bool consultar (listaToken *l, int x) {
//     listaToken aux;
//     for(aux = *l; (aux) && ((*aux).chave != x); aux = (*aux).prox);
//     return (aux);
// }

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

_Bool inseriToken(listaToken *l, tokenType tv, char *sv) {
    listaToken aux, p, pant;
    printf("%s\n", sv);
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

void imprimeLista(listaToken l){
    listaToken p;
    for(p = l; (p); p = p->prox)
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
    printf("%s\n", stringVal);
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

// void retirar(listaToken *l, int x) {
//     listaToken p, pant;
//     for (p = *l, pant = NULL; (p) && ((*p).chave != x); pant = p, p = (*p).prox);
//     if(p){
//         if(!pant)
//             *l = (*p).prox;
//         else
//             (*pant).prox = (*p).prox;
//         free(p);
//     }
// }

// ==============================================================================

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
// =========== Funções de Checagem de Estruturas da linguagem =================
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
// ==========================================================================


int main(int argc, char const *argv[]) {
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

    imprimeLista(l);

    return 0;
}