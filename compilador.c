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
   T = 47, F = 48, E = 49, SENAO = 50, OPRREL = 51, ESCRITA = 52, PUTAVEL = 53, INDEX = 54, IMPRIMIVEL = 55, NINDEX = 56, ACEITA = 200
} tokenType;





typedef struct slistaToken {
    tokenType tokenVal;
    char * stringVal;
    int numVal;
    struct slistaToken *prox;
    struct slistaToken *ant;
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
        aux->ant = NULL;
    } else{
        pant->prox = aux;
        aux->ant = pant;
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

_Bool empilhaToken (pilhaToken *p, listaToken token, int estado){
    pilhaToken aux;
    if(!(aux = (nopilhaToken*) malloc (sizeof(nopilhaToken))))
        return false;
    aux->token = (*token);
    aux->estado = estado;
    aux->prox = (*p);
    (*p) = aux;
    return true;
}
void empilhaPTrabalho (pilhaToken *p, pilhaToken token, int estado) {
    token->estado = estado;
    token->prox = (*p);
    (*p) = token;
}

_Bool desempilhaToken (pilhaToken *p, pilhaToken *x) {
    if (!(*p))
        return false;
    (*x) = (*p);
    (*p) = (*p)->prox;
    return true;
}

_Bool topoToken (pilhaToken p, pilhaToken *x){
    if (!p)
        return false;
    (*x) = p;
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

    int tabela[111][57] = {0};
    tabela[0][$] = -3;
    tabela[0][DECLARE] = 4;
    tabela[0][FOR] = 8;
    tabela[0][FOREACH] = 9;
    tabela[0][IF] = 7;
    tabela[0][PRINT] = 11;
    tabela[0][PUT] = 6;
    tabela[0][READ] = 10;
    tabela[0][RESIZE] = 5;
    tabela[0][CLOSECOLCHETES] = -3;
    tabela[1][$] = ACEITA;
    tabela[2][$] = -1;
    tabela[3][$] = -3;
    tabela[3][DECLARE] = 4;
    tabela[3][FOR] = 8;
    tabela[3][FOREACH] = 9;
    tabela[3][IF] = 7;
    tabela[3][PRINT] = 11;
    tabela[3][PUT] = 6;
    tabela[3][READ] = 10;
    tabela[3][RESIZE] = 5;
    tabela[3][CLOSECOLCHETES] = -3;
    tabela[4][ID] = 14;
    tabela[5][ID] = 15;
    tabela[6][CONSTSTRING] = 18;
    tabela[8][ID] = 21;
    tabela[9][ID] = 22;
    tabela[10][ID] = 23;
    tabela[11][CONSTCHAR] = 27;
    tabela[11][CONSTNUM] = 26;
    tabela[11][CONSTSTRING] = 28;
    tabela[11][ID] = 25;
    tabela[12][$] = -2;
    tabela[12][CLOSECOLCHETES] = -2;
    tabela[13][AS] = 29;
    tabela[14][COMMA] = -15;
    tabela[14][AS] = -15;
    tabela[14][OPENCOLCHETES] = 31;
    tabela[15][TO] = 32;
    tabela[16][IN] = 33;
    tabela[17][PLUS] = 34;
    tabela[17][MINUS] = 35;
    tabela[17][IN] = -34;
    tabela[18][IN] = -35;
    tabela[19][MOD] = 38;
    tabela[19][TIMES] = 36;
    tabela[19][PLUS] = -22;
    tabela[19][MINUS] = -22;
    tabela[19][DOT] = -22;
    tabela[19][DIVIDE] = 37;
    tabela[19][LESSTHAN] = -22;
    tabela[19][EQUAL] = -22;
    tabela[19][GREATERTHAN] = -22;
    tabela[19][DO] = -22;
    tabela[19][IN] = -22;
    tabela[19][THEN] = -22;
    tabela[19][TO] = -22;
    tabela[19][CLOSECOLCHETES] = -22;
    tabela[20][PLUS] = 34;
    tabela[20][MINUS] = 35;
    tabela[20][LESSTHAN] = 40;
    tabela[20][EQUAL] = 42;
    tabela[20][GREATERTHAN] = 41;
    tabela[21][MOD] = -17;
    tabela[21][TIMES] = -17;
    tabela[21][PLUS] = -17;
    tabela[21][MINUS] = -17;
    tabela[21][DOT] = -17;
    tabela[21][DIVIDE] = -17;
    tabela[21][LESSTHAN] = -17;
    tabela[21][EQUAL] = -17;
    tabela[21][GREATERTHAN] = -17;
    tabela[21][DO] = -17;
    tabela[21][FROM] = -17;
    tabela[21][IN] = -17;
    tabela[21][THEN] = -17;
    tabela[21][TO] = -17;
    tabela[21][OPENCOLCHETES] = 44;
    tabela[21][CLOSECOLCHETES] = -17;
    tabela[22][MOD] = -17;
    tabela[22][TIMES] = -17;
    tabela[22][PLUS] = -17;
    tabela[22][MINUS] = -17;
    tabela[22][DOT] = -17;
    tabela[22][DIVIDE] = -17;
    tabela[22][LESSTHAN] = -17;
    tabela[22][EQUAL] = -17;
    tabela[22][GREATERTHAN] = -17;
    tabela[22][DO] = -17;
    tabela[22][FROM] = -17;
    tabela[22][IN] = -17;
    tabela[22][THEN] = -17;
    tabela[22][TO] = -17;
    tabela[22][OPENCOLCHETES] = 44;
    tabela[22][CLOSECOLCHETES] = -17;
    tabela[23][MOD] = -17;
    tabela[23][TIMES] = -17;
    tabela[23][PLUS] = -17;
    tabela[23][MINUS] = -17;
    tabela[23][DOT] = -17;
    tabela[23][DIVIDE] = -17;
    tabela[23][LESSTHAN] = -17;
    tabela[23][EQUAL] = -17;
    tabela[23][GREATERTHAN] = -17;
    tabela[23][DO] = -17;
    tabela[23][FROM] = -17;
    tabela[23][IN] = -17;
    tabela[23][THEN] = -17;
    tabela[23][TO] = -17;
    tabela[23][OPENCOLCHETES] = 44;
    tabela[23][CLOSECOLCHETES] = -17;
    tabela[24][DOT] = 47;
    tabela[25][MOD] = -17;
    tabela[25][TIMES] = -17;
    tabela[25][PLUS] = -17;
    tabela[25][MINUS] = -17;
    tabela[25][DOT] = -17;
    tabela[25][DIVIDE] = -17;
    tabela[25][LESSTHAN] = -17;
    tabela[25][EQUAL] = -17;
    tabela[25][GREATERTHAN] = -17;
    tabela[25][DO] = -17;
    tabela[25][FROM] = -17;
    tabela[25][IN] = -17;
    tabela[25][THEN] = -17;
    tabela[25][TO] = -17;
    tabela[25][OPENCOLCHETES] = 44;
    tabela[25][CLOSECOLCHETES] = -17;
    tabela[26][DOT] = -45;
    tabela[27][DOT] = -46;
    tabela[28][DOT] = -47;
    tabela[29][LETTER] = 51;
    tabela[29][NUMBER] = 50;
    tabela[30][COMMA] = 52;
    tabela[30][AS] = -13;
    tabela[31][CLOSECOLCHETES] = 53;
    tabela[33][ID] = 55;
    tabela[36][OPENPARENTESES] = 59;
    tabela[36][MINUS] = 60;
    tabela[36][CONSTCHAR] = 63;
    tabela[36][CONSTNUM] = 62;
    tabela[36][ID] = 61;
    tabela[37][OPENPARENTESES] = 59;
    tabela[37][MINUS] = 60;
    tabela[37][CONSTCHAR] = 63;
    tabela[37][CONSTNUM] = 62;
    tabela[37][ID] = 61;
    tabela[38][OPENPARENTESES] = 59;
    tabela[38][MINUS] = 60;
    tabela[38][CONSTCHAR] = 63;
    tabela[38][CONSTNUM] = 62;
    tabela[38][ID] = 61;
    tabela[40][EQUAL] = 67;
    tabela[40][GREATERTHAN] = 68;
    tabela[41][EQUAL] = 69;
    tabela[43][FROM] = 70;
    tabela[45][IN] = 72;
    tabela[46][DOT] = 73;
    tabela[47][$] = -11;
    tabela[47][DECLARE] = -11;
    tabela[47][FOR] = -11;
    tabela[47][FOREACH] = -11;
    tabela[47][IF] = -11;
    tabela[47][PRINT] = -11;
    tabela[47][PUT] = -11;
    tabela[47][READ] = -11;
    tabela[47][RESIZE] = -11;
    tabela[47][CLOSECOLCHETES] = -11;
    tabela[48][DOT] = -44;
    tabela[49][DOT] = 74;
    tabela[50][DOT] = -18;
    tabela[51][DOT] = -19;
    tabela[52][ID] = 14;
    tabela[53][COMMA] = -14;
    tabela[53][AS] = -14;
    tabela[54][PLUS] = 34;
    tabela[54][MINUS] = 35;
    tabela[54][DOT] = 76;
    tabela[55][MOD] = -17;
    tabela[55][TIMES] = -17;
    tabela[55][PLUS] = -17;
    tabela[55][MINUS] = -17;
    tabela[55][DOT] = -17;
    tabela[55][DIVIDE] = -17;
    tabela[55][LESSTHAN] = -17;
    tabela[55][EQUAL] = -17;
    tabela[55][GREATERTHAN] = -17;
    tabela[55][DO] = -17;
    tabela[55][FROM] = -17;
    tabela[55][IN] = -17;
    tabela[55][THEN] = -17;
    tabela[55][TO] = -17;
    tabela[55][OPENCOLCHETES] = 44;
    tabela[55][CLOSECOLCHETES] = -17;
    tabela[56][MOD] = 38;
    tabela[56][TIMES] = 36;
    tabela[56][PLUS] = -20;
    tabela[56][MINUS] = -20;
    tabela[56][DOT] = -20;
    tabela[56][DIVIDE] = 37;
    tabela[56][LESSTHAN] = -20;
    tabela[56][EQUAL] = -20;
    tabela[56][GREATERTHAN] = -20;
    tabela[56][DO] = -20;
    tabela[56][IN] = -20;
    tabela[56][THEN] = -20;
    tabela[56][TO] = -20;
    tabela[56][CLOSECOLCHETES] = -20;
    tabela[57][MOD] = 38;
    tabela[57][TIMES] = 36;
    tabela[57][PLUS] = -21;
    tabela[57][MINUS] = -21;
    tabela[57][DOT] = -21;
    tabela[57][DIVIDE] = 37;
    tabela[57][LESSTHAN] = -21;
    tabela[57][EQUAL] = -21;
    tabela[57][GREATERTHAN] = -21;
    tabela[57][DO] = -21;
    tabela[57][IN] = -21;
    tabela[57][THEN] = -21;
    tabela[57][TO] = -21;
    tabela[57][CLOSECOLCHETES] = -21;
    tabela[58][MOD] = -23;
    tabela[58][TIMES] = -23;
    tabela[58][PLUS] = -23;
    tabela[58][MINUS] = -23;
    tabela[58][DOT] = -23;
    tabela[58][DIVIDE] = -23;
    tabela[58][LESSTHAN] = -23;
    tabela[58][EQUAL] = -23;
    tabela[58][GREATERTHAN] = -23;
    tabela[58][DO] = -23;
    tabela[58][IN] = -23;
    tabela[58][THEN] = -23;
    tabela[58][TO] = -23;
    tabela[58][CLOSECOLCHETES] = -23;
    tabela[59][E] = 78;
    tabela[60][OPENPARENTESES] = 79;
    tabela[60][CONSTCHAR] = 82;
    tabela[60][CONSTNUM] = 81;
    tabela[60][ID] = 80;
    tabela[61][MOD] = -17;
    tabela[61][TIMES] = -17;
    tabela[61][PLUS] = -17;
    tabela[61][MINUS] = -17;
    tabela[61][DOT] = -17;
    tabela[61][DIVIDE] = -17;
    tabela[61][LESSTHAN] = -17;
    tabela[61][EQUAL] = -17;
    tabela[61][GREATERTHAN] = -17;
    tabela[61][DO] = -17;
    tabela[61][FROM] = -17;
    tabela[61][IN] = -17;
    tabela[61][THEN] = -17;
    tabela[61][TO] = -17;
    tabela[61][OPENCOLCHETES] = 44;
    tabela[61][CLOSECOLCHETES] = -17;
    tabela[62][MOD] = -30;
    tabela[62][TIMES] = -30;
    tabela[62][PLUS] = -30;
    tabela[62][MINUS] = -30;
    tabela[62][DOT] = -30;
    tabela[62][DIVIDE] = -30;
    tabela[62][LESSTHAN] = -30;
    tabela[62][EQUAL] = -30;
    tabela[62][GREATERTHAN] = -30;
    tabela[62][DO] = -30;
    tabela[62][IN] = -30;
    tabela[62][THEN] = -30;
    tabela[62][TO] = -30;
    tabela[62][CLOSECOLCHETES] = -30;
    tabela[63][MOD] = -32;
    tabela[63][TIMES] = -32;
    tabela[63][PLUS] = -32;
    tabela[63][MINUS] = -32;
    tabela[63][DOT] = -32;
    tabela[63][DIVIDE] = -32;
    tabela[63][LESSTHAN] = -32;
    tabela[63][EQUAL] = -32;
    tabela[63][GREATERTHAN] = -32;
    tabela[63][DO] = -32;
    tabela[63][IN] = -32;
    tabela[63][THEN] = -32;
    tabela[63][TO] = -32;
    tabela[63][CLOSECOLCHETES] = -32;
    tabela[64][MOD] = -24;
    tabela[64][TIMES] = -24;
    tabela[64][PLUS] = -24;
    tabela[64][MINUS] = -24;
    tabela[64][DOT] = -24;
    tabela[64][DIVIDE] = -24;
    tabela[64][LESSTHAN] = -24;
    tabela[64][EQUAL] = -24;
    tabela[64][GREATERTHAN] = -24;
    tabela[64][DO] = -24;
    tabela[64][IN] = -24;
    tabela[64][THEN] = -24;
    tabela[64][TO] = -24;
    tabela[64][CLOSECOLCHETES] = -24;
    tabela[65][MOD] = -25;
    tabela[65][TIMES] = -25;
    tabela[65][PLUS] = -25;
    tabela[65][MINUS] = -25;
    tabela[65][DOT] = -25;
    tabela[65][DIVIDE] = -25;
    tabela[65][LESSTHAN] = -25;
    tabela[65][EQUAL] = -25;
    tabela[65][GREATERTHAN] = -25;
    tabela[65][DO] = -25;
    tabela[65][IN] = -25;
    tabela[65][THEN] = -25;
    tabela[65][TO] = -25;
    tabela[65][CLOSECOLCHETES] = -25;
    tabela[66][PLUS] = 34;
    tabela[66][MINUS] = 35;
    tabela[66][THEN] = 84;
    tabela[71][PLUS] = 34;
    tabela[71][MINUS] = 35;
    tabela[71][CLOSECOLCHETES] = 86;
    tabela[72][ID] = 87;
    tabela[73][$] = -10;
    tabela[73][DECLARE] = -10;
    tabela[73][FOR] = -10;
    tabela[73][FOREACH] = -10;
    tabela[73][IF] = -10;
    tabela[73][PRINT] = -10;
    tabela[73][PUT] = -10;
    tabela[73][READ] = -10;
    tabela[73][RESIZE] = -10;
    tabela[73][CLOSECOLCHETES] = -10;
    tabela[74][$] = -4;
    tabela[74][DECLARE] = -4;
    tabela[74][FOR] = -4;
    tabela[74][FOREACH] = -4;
    tabela[74][IF] = -4;
    tabela[74][PRINT] = -4;
    tabela[74][PUT] = -4;
    tabela[74][READ] = -4;
    tabela[74][RESIZE] = -4;
    tabela[74][CLOSECOLCHETES] = -4;
    tabela[75][AS] = -12;
    tabela[76][$] = -5;
    tabela[76][DECLARE] = -5;
    tabela[76][FOR] = -5;
    tabela[76][FOREACH] = -5;
    tabela[76][IF] = -5;
    tabela[76][PRINT] = -5;
    tabela[76][PUT] = -5;
    tabela[76][READ] = -5;
    tabela[76][RESIZE] = -5;
    tabela[76][CLOSECOLCHETES] = -5;
    tabela[77][DOT] = 88;
    tabela[78][CLOSEPARENTESES] = 89;
    tabela[79][E] = 90;
    tabela[80][MOD] = -17;
    tabela[80][TIMES] = -17;
    tabela[80][PLUS] = -17;
    tabela[80][MINUS] = -17;
    tabela[80][DOT] = -17;
    tabela[80][DIVIDE] = -17;
    tabela[80][LESSTHAN] = -17;
    tabela[80][EQUAL] = -17;
    tabela[80][GREATERTHAN] = -17;
    tabela[80][DO] = -17;
    tabela[80][FROM] = -17;
    tabela[80][IN] = -17;
    tabela[80][THEN] = -17;
    tabela[80][TO] = -17;
    tabela[80][OPENCOLCHETES] = 44;
    tabela[80][CLOSECOLCHETES] = -17;
    tabela[81][MOD] = -31;
    tabela[81][TIMES] = -31;
    tabela[81][PLUS] = -31;
    tabela[81][MINUS] = -31;
    tabela[81][DOT] = -31;
    tabela[81][DIVIDE] = -31;
    tabela[81][LESSTHAN] = -31;
    tabela[81][EQUAL] = -31;
    tabela[81][GREATERTHAN] = -31;
    tabela[81][DO] = -31;
    tabela[81][IN] = -31;
    tabela[81][THEN] = -31;
    tabela[81][TO] = -31;
    tabela[81][CLOSECOLCHETES] = -31;
    tabela[82][MOD] = -33;
    tabela[82][TIMES] = -33;
    tabela[82][PLUS] = -33;
    tabela[82][MINUS] = -33;
    tabela[82][DOT] = -33;
    tabela[82][DIVIDE] = -33;
    tabela[82][LESSTHAN] = -33;
    tabela[82][EQUAL] = -33;
    tabela[82][GREATERTHAN] = -33;
    tabela[82][DO] = -33;
    tabela[82][IN] = -33;
    tabela[82][THEN] = -33;
    tabela[82][TO] = -33;
    tabela[82][CLOSECOLCHETES] = -33;
    tabela[83][MOD] = -28;
    tabela[83][TIMES] = -28;
    tabela[83][PLUS] = -28;
    tabela[83][MINUS] = -28;
    tabela[83][DOT] = -28;
    tabela[83][DIVIDE] = -28;
    tabela[83][LESSTHAN] = -28;
    tabela[83][EQUAL] = -28;
    tabela[83][GREATERTHAN] = -28;
    tabela[83][DO] = -28;
    tabela[83][IN] = -28;
    tabela[83][THEN] = -28;
    tabela[83][TO] = -28;
    tabela[83][CLOSECOLCHETES] = -28;
    tabela[84][OPENCOLCHETES] = 92;
    tabela[85][PLUS] = 34;
    tabela[85][MINUS] = 35;
    tabela[85][TO] = 93;
    tabela[86][MOD] = -16;
    tabela[86][TIMES] = -16;
    tabela[86][PLUS] = -16;
    tabela[86][MINUS] = -16;
    tabela[86][DOT] = -16;
    tabela[86][DIVIDE] = -16;
    tabela[86][LESSTHAN] = -16;
    tabela[86][EQUAL] = -16;
    tabela[86][GREATERTHAN] = -16;
    tabela[86][DO] = -16;
    tabela[86][FROM] = -16;
    tabela[86][IN] = -16;
    tabela[86][THEN] = -16;
    tabela[86][TO] = -16;
    tabela[86][CLOSECOLCHETES] = -16;
    tabela[87][DO] = 94;
    tabela[88][$] = -6;
    tabela[88][DECLARE] = -6;
    tabela[88][FOR] = -6;
    tabela[88][FOREACH] = -6;
    tabela[88][IF] = -6;
    tabela[88][PRINT] = -6;
    tabela[88][PUT] = -6;
    tabela[88][READ] = -6;
    tabela[88][RESIZE] = -6;
    tabela[88][CLOSECOLCHETES] = -6;
    tabela[89][MOD] = -26;
    tabela[89][TIMES] = -26;
    tabela[89][PLUS] = -26;
    tabela[89][MINUS] = -26;
    tabela[89][DOT] = -26;
    tabela[89][DIVIDE] = -26;
    tabela[89][LESSTHAN] = -26;
    tabela[89][EQUAL] = -26;
    tabela[89][GREATERTHAN] = -26;
    tabela[89][DO] = -26;
    tabela[89][IN] = -26;
    tabela[89][THEN] = -26;
    tabela[89][TO] = -26;
    tabela[89][CLOSECOLCHETES] = -26;
    tabela[90][CLOSEPARENTESES] = 95;
    tabela[91][MOD] = -29;
    tabela[91][TIMES] = -29;
    tabela[91][PLUS] = -29;
    tabela[91][MINUS] = -29;
    tabela[91][DOT] = -29;
    tabela[91][DIVIDE] = -29;
    tabela[91][LESSTHAN] = -29;
    tabela[91][EQUAL] = -29;
    tabela[91][GREATERTHAN] = -29;
    tabela[91][DO] = -29;
    tabela[91][IN] = -29;
    tabela[91][THEN] = -29;
    tabela[91][TO] = -29;
    tabela[91][CLOSECOLCHETES] = -29;
    tabela[92][$] = -3;
    tabela[92][DECLARE] = 4;
    tabela[92][FOR] = 8;
    tabela[92][FOREACH] = 9;
    tabela[92][IF] = 7;
    tabela[92][PRINT] = 11;
    tabela[92][PUT] = 6;
    tabela[92][READ] = 10;
    tabela[92][RESIZE] = 5;
    tabela[92][CLOSECOLCHETES] = -3;
    tabela[94][OPENCOLCHETES] = 98;
    tabela[95][MOD] = -27;
    tabela[95][TIMES] = -27;
    tabela[95][PLUS] = -27;
    tabela[95][MINUS] = -27;
    tabela[95][DOT] = -27;
    tabela[95][DIVIDE] = -27;
    tabela[95][LESSTHAN] = -27;
    tabela[95][EQUAL] = -27;
    tabela[95][GREATERTHAN] = -27;
    tabela[95][DO] = -27;
    tabela[95][IN] = -27;
    tabela[95][THEN] = -27;
    tabela[95][TO] = -27;
    tabela[95][CLOSECOLCHETES] = -27;
    tabela[96][CLOSECOLCHETES] = 99;
    tabela[97][PLUS] = 34;
    tabela[97][MINUS] = 35;
    tabela[97][DO] = 100;
    tabela[98][$] = -3;
    tabela[98][DECLARE] = 4;
    tabela[98][FOR] = 8;
    tabela[98][FOREACH] = 9;
    tabela[98][IF] = 7;
    tabela[98][PRINT] = 11;
    tabela[98][PUT] = 6;
    tabela[98][READ] = 10;
    tabela[98][RESIZE] = 5;
    tabela[98][CLOSECOLCHETES] = -3;
    tabela[99][$] = -37;
    tabela[99][DECLARE] = -37;
    tabela[99][ELSE] = 103;
    tabela[99][FOR] = -37;
    tabela[99][FOREACH] = -37;
    tabela[99][IF] = -37;
    tabela[99][PRINT] = -37;
    tabela[99][PUT] = -37;
    tabela[99][READ] = -37;
    tabela[99][RESIZE] = -37;
    tabela[99][CLOSECOLCHETES] = -37;
    tabela[100][OPENCOLCHETES] = 104;
    tabela[101][CLOSECOLCHETES] = 105;
    tabela[102][$] = -7;
    tabela[102][DECLARE] = -7;
    tabela[102][FOR] = -7;
    tabela[102][FOREACH] = -7;
    tabela[102][IF] = -7;
    tabela[102][PRINT] = -7;
    tabela[102][PUT] = -7;
    tabela[102][READ] = -7;
    tabela[102][RESIZE] = -7;
    tabela[102][CLOSECOLCHETES] = -7;
    tabela[103][OPENCOLCHETES] = 106;
    tabela[104][$] = -3;
    tabela[104][DECLARE] = 4;
    tabela[104][FOR] = 8;
    tabela[104][FOREACH] = 9;
    tabela[104][IF] = 7;
    tabela[104][PRINT] = 11;
    tabela[104][PUT] = 6;
    tabela[104][READ] = 10;
    tabela[104][RESIZE] = 5;
    tabela[104][CLOSECOLCHETES] = -3;
    tabela[105][$] = -9;
    tabela[105][DECLARE] = -9;
    tabela[105][FOR] = -9;
    tabela[105][FOREACH] = -9;
    tabela[105][IF] = -9;
    tabela[105][PRINT] = -9;
    tabela[105][PUT] = -9;
    tabela[105][READ] = -9;
    tabela[105][RESIZE] = -9;
    tabela[105][CLOSECOLCHETES] = -9;
    tabela[106][$] = -3;
    tabela[106][DECLARE] = 4;
    tabela[106][FOR] = 8;
    tabela[106][FOREACH] = 9;
    tabela[106][IF] = 7;
    tabela[106][PRINT] = 11;
    tabela[106][PUT] = 6;
    tabela[106][READ] = 10;
    tabela[106][RESIZE] = 5;
    tabela[106][CLOSECOLCHETES] = -3;
    tabela[107][CLOSECOLCHETES] = 109;
    tabela[108][CLOSECOLCHETES] = 110;
    tabela[109][$] = -8;
    tabela[109][DECLARE] = -8;
    tabela[109][FOR] = -8;
    tabela[109][FOREACH] = -8;
    tabela[109][IF] = -8;
    tabela[109][PRINT] = -8;
    tabela[109][PUT] = -8;
    tabela[109][READ] = -8;
    tabela[109][RESIZE] = -8;
    tabela[109][CLOSECOLCHETES] = -8;
    tabela[110][$] = -36;
    tabela[110][DECLARE] = -36;
    tabela[110][FOR] = -36;
    tabela[110][FOREACH] = -36;
    tabela[110][IF] = -36;
    tabela[110][PRINT] = -36;
    tabela[110][PUT] = -36;
    tabela[110][READ] = -36;
    tabela[110][RESIZE] = -36;
    tabela[110][CLOSECOLCHETES] = -36;

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


    // printf("%d\n%d\n%d\n", producoes[0].p, producoes[0].q[0], producoes[0].n);

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
    pilhaToken pEntrada;
    pilhaToken pTrabalho
    inicializaPilhaToken(&pEntrada);
    inicializaPilhaToken(&pTrabalho);
    
    listaToken p;
    listaToken pant;
    
    for (p = l, pant = NULL; (p); pant = p, p = p->prox);
    for (;(pant); pant = pant->ant) {
        while(!(empilhaToken(&pEntrada, pant, -1)));
    }

    int acao;
    int estado = 0;
    pilhaToken x;

    while(topoToken(pEntrada, &x)) {
        acao = slr1(x, estado, tabela);
        if (acao == ACEITA) {
            //Aceita
        } else if (acao > 0 ) {
            desempilhaToken(&pEntrada, &x);
            empilhaPTrabalho(&pTrabalho, x, estado);
            estado = acao;
        }
    }

    // desempilhaToken(&pEntrada, &x);
    // desempilhaToken(&pEntrada, &x);
    // desempilhaToken(&pEntrada, &x);
    // topoToken(&pEntrada, &x);
    // printf("%d\n%s\n", x->token.tokenVal, x->token.stringVal);
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