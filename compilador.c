#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ============================================================================
// ============================= Lista de Tokens ==============================
// ============================================================================
typedef enum {
   DECLARE, AS, NUMBER, LETTER, PUT, IN, IF, THE, ELSE, FOREACH, DO, FOR, FROM, TO
   , RESIZE, READ, PRINT, COMMA, DOT, OPENCOLCHETES, CLOSECOLCHETES, PLUS, MINUS, TIMES
   , DIVIDE, MOD, OPENPARENTESES, CLOSEPARENTESES, LESSTHAN, GREATERTHAN, LESSEQUALTHAN
   , GREATEREQUALTHAN, EQUAL, DIFERENT, ID, NUMBER, CONSTCHAR, CONSTSTRING
} tokenType;


typedef struct slistaToken {
    tokenType tokenVal;
    char * stringVal;
    int numVal;
    struct nolistaToken *prox;
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
    int length = f - i + 1, j;
    if(!(*s) = (char*) malloc(sizeof(char)*length + 1))
        return false;
    int j;
    for (j = 0; j < length; j++)
        (*s)[j] = e[i+j]; 
    stringVal[j] = 0;
    return true;
}

void salvaTokenIdentificador(char *e, int i , int f, listaToken *l){
    char *stringVal;
    while(!formString(e, &stringVal, i, f));
    switch (length){
        case 2:
            if (strncmp(stringVal[], "AS", 2)) { while(!inseriToken(l, AS, stringVal)); }
            else if (strncmp(stringVal[], "IN", 2)) { while(!inseriToken(l, IN, stringVal));}
            else if (strncmp(stringVal[], "IF", 2)) { while(!inseriToken(l, IF, stringVal));}
            else if (strncmp(stringVal[], "DO", 2)) { while(!inseriToken(l, DO, stringVal));}
            else if (strncmp(stringVal[], "TO")) { while(!inseriToken(l, TO, stringVal));}
            else { while(!inseriToken(l, ID, stringVal));}
        break;
        case 3: 
            if (strncmp(stringVal[], "PUT", 3)) { while(!inseriToken(l, PUT, stringVal)); }
            else if (strncmp(stringVal[], "FOR", 3)) { while(!inseriToken(l, FOR, stringVal));}
            else { while(!inseriToken(l, ID, stringVal));}
        break
        case 4:
            if (strncmp(stringVal[], "THEN", 4)) { while(!inseriToken(l, THEN, stringVal)); }
            else if (strncmp(stringVal[], "ELSE", 4)) { while(!inseriToken(l, ELSE, stringVal));}
            else if (strncmp(stringVal[], "FROM", 4)) { while(!inseriToken(l, FROM, stringVal));}
            else if (strncmp(stringVal[], "READ", 4)) { while(!inseriToken(l, READ, stringVal));}
            else { while(!inseriToken(l, ID, stringVal));}
        break;
        case 5:
            if (strncmp(stringVal[], "PRINT", 5)) { while(!inseriToken(l, PRINT, stringVal)); }
            else { while(!inseriToken(l, ID, stringVal));}
        break;
        case 6:
            if (strncmp(stringVal[], "NUMBER", 6)) { while(!inseriToken(l, NUMBER, stringVal)); }
            else if (strncmp(stringVal[], "LETTER", 6)) { while(!inseriToken(l, LETTER, stringVal));}
            else if (strncmp(stringVal[], "RESIZE", 6)) { while(!inseriToken(l, RESIZE, stringVal));}
            else { while(!inseriToken(l, ID, stringVal));}
        break;
        case 7:
            if (strncmp(stringVal[], "DECLARE", 7)) { while(!inseriToken(l, DECLARE, stringVal)); }
            else if (strncmp(stringVal[], "FOREACH", 7)) { while(!inseriToken(l, FOREACH, stringVal));}
            else { while(!inseriToken(l, ID, stringVal));}
        break;
        default :
            while(!inseriToken(l, ID, stringVal));
    } 
}

void salvaTokenSeparadorUnico(char *e, int i, listaToken *l){
    char stringVal[2];
    stringVal[0] = e[i];
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

_Bool inseriToken (listaToken *l, tokenType tv, char *sv) {
    listaToken aux, p, pant;
    if (!(aux = (nolistaToken*) malloc (sizeof(nolistaToken))))
        return false;

    aux.tokenVal = tv;
    aux.stringVal = sv;
    if (tv == NUMBER)
        numVal = atoi(sv);
    aux.prox = NULL;

    for (p = *l, pant = NULL; (p); pant = p, p = (*p).prox);
    if(p == *l)
        *l = aux;
    else
        (*pant).prox = aux;
    return true;
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
            while(!inseriToken(lista, CONSTCHAR, stringVal));
        }
    } else {
        printErroLexico(e, pos, aux, l);
    }
    if(e[aux] == '\t' || e[aux] == '\n')
        aux--;
    return aux;
}

int checkString(char *e, int pos, int l){
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
        printf("ARQUIVO INVÁLIDO!\n");
        return 0;
    }
    
    int nextPos = 0;
    char nextChar;
    int line = 1;
    char stringVal;

    listaToken l;
    inicializar(&l);

    while (entrada[nextPos] != 0){}       
        nextChar = entrada[nextPos];
        if (isalpha(nextChar)) {
            nextPos = checkIdentificador(entrada, nextPos, &l);
        } else if (isNumber(nextChar)) {
            nextPos = checkNumber(entrada, nextPos, line);
        } else if (nextChar == '\''){
            nextPos = checkConstChar(entrada, nextPos, line);
        } else if (nextChar == '\"'){
            nextPos = checkString(entrada, nextPos, line);
        } else if (nextChar == '\n'){
            line++;
        } else if (nextChar == '>'){
            if (entrada[nextPos + 1] == '='){
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                while(!inseriToken(l, GREATEREQUALTHAN, stringVal));
            }
        } else if (nextChar == '<'){
            if (entrada[nextPos + 1] == '=') {
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                while(!inseriToken(l, LESSEQUALTHAN, stringVal));
            } else if (entrada[nextPos + 1] == '>'){
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                while(!inseriToken(l, DIFERENT, stringVal));
            }
        } else if (isSeparator(nextChar)){
            salvaTokenSeparadorUnico(entrada, nextPos, &l);
        } else if (nextChar != ' ' && nextChar != '\t')
            printErroLexico(entrada, nextPos, nextPos, line);
        nextPos++;
    }

    return 0;
}