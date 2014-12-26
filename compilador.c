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
   T = 47, F = 48, SENAO = 49, OPRREL = 50, ESCRITA = 51, PUTAVEL = 52, INDEX = 53, IMPRIMIVEL = 54,
   NINDEX = 55, ACEITA = 200
} tokenType;





typedef struct slistaToken {
    tokenType tokenVal;
    char * stringVal;
    int numVal;
    struct slistaToken *prox;
    struct slistaToken *ant;
} nolistaToken, *listaToken;

typedef struct spilhaToken {
    tokenType token;
    int estado;
    struct spilhaToken *prox;
} nopilhaToken, *pilhaToken ;

typedef struct producao {
    int p; // não terminal antes da seta
    int *q; // vetor dos simbolos depois da seta
    int n // número de simbolos depois da
} prod;


static int erroL = 0;
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
            else if (compareString(stringVal, "NUMBER"))   { while(!inseriToken(l, NUMBER, stringVal));}
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

_Bool empilhaToken (pilhaToken *p, tokenType token, int estado){
    pilhaToken aux;
    if(!(aux = (nopilhaToken*) malloc (sizeof(nopilhaToken))))
        return false;
    aux->token = token;
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
    erroL = 1;
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
        // printf("%s\n", stringVal);
        while(!inseriToken(lista, CONSTNUM, stringVal));
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
        if (e[aux-1] == 't' || e[aux-1] == 'T' || e[aux-1] == 'n' || e[aux-1] == 'N' || e[aux-1] == '\\' || e[aux-1] == '\'' || e[aux-1] == '\"') {
            if (e[aux-2] != '\\')
                printErroLexico(e, pos, aux, l);
            else{
                while(!formString(e, &stringVal, pos+1, aux-1));
                // printf("%s\n", stringVal);
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
            // printf("%s\n", stringVal);
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
        if(exception && e[aux] != 't' && e[aux] != 'T' && e[aux] != 'n' && e[aux] != 'N' && e[aux] != '\\' && e[aux] != '\''  && e[aux] != '\"'){
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
        // printf("%s\n", stringVal);
        while(!inseriToken(lista, CONSTSTRING, stringVal));
    }
    if(e[aux] == '\t' || e[aux] == '\n')
        aux--;
    return aux;
}


// ============================================================================
// ====================== Funções de Analise Sintatica ========================
// ============================================================================

        // acao = slr1(x->token.tokenVal, estado, tabela);
    int slr1(tokenType token, int estado, int tabela[112][56]){
        return tabela[estado][token];
    }



// ============================================================================

int main(int argc, char const *argv[]) {

    int tabela[112][56] = {0};


tabela[0][$] = -3;
tabela[0][CLOSECOLCHETES] = -3;
tabela[0][DECLARE] = 4;
tabela[0][FOR] = 8;
tabela[0][FOREACH] = 9;
tabela[0][IF] = 7;
tabela[0][PRINT] = 11;
tabela[0][PUT] = 6;
tabela[0][READ] = 10;
tabela[0][RESIZE] = 5;
tabela[0][COMANDO] = 3;
tabela[0][COMANDOS] = 2;
tabela[0][S] = 1;
tabela[1][$] = ACEITA;
tabela[2][$] = -1;
tabela[3][$] = -3;
tabela[3][CLOSECOLCHETES] = -3;
tabela[3][DECLARE] = 4;
tabela[3][FOR] = 8;
tabela[3][FOREACH] = 9;
tabela[3][IF] = 7;
tabela[3][PRINT] = 11;
tabela[3][PUT] = 6;
tabela[3][READ] = 10;
tabela[3][RESIZE] = 5;
tabela[3][COMANDO] = 3;
tabela[3][COMANDOS] = 12;
tabela[4][ID] = 14;
tabela[4][LISTAIDS] = 13;
tabela[5][ID] = 15;
tabela[6][CONSTCHAR] = 25;
tabela[6][CONSTNUM] = 24;
tabela[6][CONSTSTRING] = 18;
tabela[6][ID] = 23;
tabela[6][MINUS] = 22;
tabela[6][OPENPARENTESES] = 21;
tabela[6][EXPARIT] = 17;
tabela[6][F] = 20;
tabela[6][PUTAVEL] = 16;
tabela[6][T] = 19;
tabela[7][CONSTCHAR] = 25;
tabela[7][CONSTNUM] = 24;
tabela[7][ID] = 23;
tabela[7][MINUS] = 22;
tabela[7][OPENPARENTESES] = 21;
tabela[7][EXPARIT] = 26;
tabela[7][F] = 20;
tabela[7][T] = 19;
tabela[8][ID] = 27;
tabela[9][ID] = 28;
tabela[10][ID] = 29;
tabela[11][CONSTCHAR] = 33;
tabela[11][CONSTNUM] = 32;
tabela[11][CONSTSTRING] = 34;
tabela[11][ID] = 31;
tabela[11][IMPRIMIVEL] = 30;
tabela[12][$] = -2;
tabela[12][CLOSECOLCHETES] = -2;
tabela[13][AS] = 35;
tabela[14][AS] = -15;
tabela[14][COMMA] = -15;
tabela[14][OPENCOLCHETES] = 37;
tabela[14][NINDEX] = 36;
tabela[15][TO] = 38;
tabela[16][IN] = 39;
tabela[17][IN] = -35;
tabela[17][MINUS] = 41;
tabela[17][PLUS] = 40;
tabela[18][IN] = -36;
tabela[19][CLOSECOLCHETES] = -22;
tabela[19][CLOSEPARENTESES] = -22;
tabela[19][DIFERENT] = -22;
tabela[19][DIVIDE] = 43;
tabela[19][DO] = -22;
tabela[19][DOT] = -22;
tabela[19][EQUAL] = -22;
tabela[19][GREATEREQUALTHAN] = -22;
tabela[19][GREATERTHAN] = -22;
tabela[19][IN] = -22;
tabela[19][LESSEQUALTHAN] = -22;
tabela[19][LESSTHAN] = -22;
tabela[19][MINUS] = -22;
tabela[19][MOD] = 44;
tabela[19][PLUS] = -22;
tabela[19][THEN] = -22;
tabela[19][TIMES] = 42;
tabela[19][TO] = -22;
tabela[20][CLOSECOLCHETES] = -26;
tabela[20][CLOSEPARENTESES] = -26;
tabela[20][DIFERENT] = -26;
tabela[20][DIVIDE] = -26;
tabela[20][DO] = -26;
tabela[20][DOT] = -26;
tabela[20][EQUAL] = -26;
tabela[20][GREATEREQUALTHAN] = -26;
tabela[20][GREATERTHAN] = -26;
tabela[20][IN] = -26;
tabela[20][LESSEQUALTHAN] = -26;
tabela[20][LESSTHAN] = -26;
tabela[20][MINUS] = -26;
tabela[20][MOD] = -26;
tabela[20][PLUS] = -26;
tabela[20][THEN] = -26;
tabela[20][TIMES] = -26;
tabela[20][TO] = -26;
tabela[21][CONSTCHAR] = 25;
tabela[21][CONSTNUM] = 24;
tabela[21][ID] = 23;
tabela[21][MINUS] = 22;
tabela[21][OPENPARENTESES] = 21;
tabela[21][EXPARIT] = 45;
tabela[21][F] = 20;
tabela[21][T] = 19;
tabela[22][CONSTCHAR] = 49;
tabela[22][CONSTNUM] = 48;
tabela[22][ID] = 47;
tabela[22][OPENPARENTESES] = 46;
tabela[23][CLOSECOLCHETES] = -17;
tabela[23][CLOSEPARENTESES] = -17;
tabela[23][DIFERENT] = -17;
tabela[23][DIVIDE] = -17;
tabela[23][DO] = -17;
tabela[23][DOT] = -17;
tabela[23][EQUAL] = -17;
tabela[23][FROM] = -17;
tabela[23][GREATEREQUALTHAN] = -17;
tabela[23][GREATERTHAN] = -17;
tabela[23][IN] = -17;
tabela[23][LESSEQUALTHAN] = -17;
tabela[23][LESSTHAN] = -17;
tabela[23][MINUS] = -17;
tabela[23][MOD] = -17;
tabela[23][OPENCOLCHETES] = 51;
tabela[23][PLUS] = -17;
tabela[23][THEN] = -17;
tabela[23][TIMES] = -17;
tabela[23][TO] = -17;
tabela[23][INDEX] = 50;
tabela[24][CLOSECOLCHETES] = -31;
tabela[24][CLOSEPARENTESES] = -31;
tabela[24][DIFERENT] = -31;
tabela[24][DIVIDE] = -31;
tabela[24][DO] = -31;
tabela[24][DOT] = -31;
tabela[24][EQUAL] = -31;
tabela[24][GREATEREQUALTHAN] = -31;
tabela[24][GREATERTHAN] = -31;
tabela[24][IN] = -31;
tabela[24][LESSEQUALTHAN] = -31;
tabela[24][LESSTHAN] = -31;
tabela[24][MINUS] = -31;
tabela[24][MOD] = -31;
tabela[24][PLUS] = -31;
tabela[24][THEN] = -31;
tabela[24][TIMES] = -31;
tabela[24][TO] = -31;
tabela[25][CLOSECOLCHETES] = -33;
tabela[25][CLOSEPARENTESES] = -33;
tabela[25][DIFERENT] = -33;
tabela[25][DIVIDE] = -33;
tabela[25][DO] = -33;
tabela[25][DOT] = -33;
tabela[25][EQUAL] = -33;
tabela[25][GREATEREQUALTHAN] = -33;
tabela[25][GREATERTHAN] = -33;
tabela[25][IN] = -33;
tabela[25][LESSEQUALTHAN] = -33;
tabela[25][LESSTHAN] = -33;
tabela[25][MINUS] = -33;
tabela[25][MOD] = -33;
tabela[25][PLUS] = -33;
tabela[25][THEN] = -33;
tabela[25][TIMES] = -33;
tabela[25][TO] = -33;
tabela[26][DIFERENT] = 58;
tabela[26][EQUAL] = 57;
tabela[26][GREATEREQUALTHAN] = 56;
tabela[26][GREATERTHAN] = 54;
tabela[26][LESSEQUALTHAN] = 55;
tabela[26][LESSTHAN] = 53;
tabela[26][MINUS] = 41;
tabela[26][PLUS] = 40;
tabela[26][OPRREL] = 52;
tabela[27][CLOSECOLCHETES] = -17;
tabela[27][CLOSEPARENTESES] = -17;
tabela[27][DIFERENT] = -17;
tabela[27][DIVIDE] = -17;
tabela[27][DO] = -17;
tabela[27][DOT] = -17;
tabela[27][EQUAL] = -17;
tabela[27][FROM] = -17;
tabela[27][GREATEREQUALTHAN] = -17;
tabela[27][GREATERTHAN] = -17;
tabela[27][IN] = -17;
tabela[27][LESSEQUALTHAN] = -17;
tabela[27][LESSTHAN] = -17;
tabela[27][MINUS] = -17;
tabela[27][MOD] = -17;
tabela[27][OPENCOLCHETES] = 51;
tabela[27][PLUS] = -17;
tabela[27][THEN] = -17;
tabela[27][TIMES] = -17;
tabela[27][TO] = -17;
tabela[27][INDEX] = 59;
tabela[28][CLOSECOLCHETES] = -17;
tabela[28][CLOSEPARENTESES] = -17;
tabela[28][DIFERENT] = -17;
tabela[28][DIVIDE] = -17;
tabela[28][DO] = -17;
tabela[28][DOT] = -17;
tabela[28][EQUAL] = -17;
tabela[28][FROM] = -17;
tabela[28][GREATEREQUALTHAN] = -17;
tabela[28][GREATERTHAN] = -17;
tabela[28][IN] = -17;
tabela[28][LESSEQUALTHAN] = -17;
tabela[28][LESSTHAN] = -17;
tabela[28][MINUS] = -17;
tabela[28][MOD] = -17;
tabela[28][OPENCOLCHETES] = 51;
tabela[28][PLUS] = -17;
tabela[28][THEN] = -17;
tabela[28][TIMES] = -17;
tabela[28][TO] = -17;
tabela[28][INDEX] = 60;
tabela[29][CLOSECOLCHETES] = -17;
tabela[29][CLOSEPARENTESES] = -17;
tabela[29][DIFERENT] = -17;
tabela[29][DIVIDE] = -17;
tabela[29][DO] = -17;
tabela[29][DOT] = -17;
tabela[29][EQUAL] = -17;
tabela[29][FROM] = -17;
tabela[29][GREATEREQUALTHAN] = -17;
tabela[29][GREATERTHAN] = -17;
tabela[29][IN] = -17;
tabela[29][LESSEQUALTHAN] = -17;
tabela[29][LESSTHAN] = -17;
tabela[29][MINUS] = -17;
tabela[29][MOD] = -17;
tabela[29][OPENCOLCHETES] = 51;
tabela[29][PLUS] = -17;
tabela[29][THEN] = -17;
tabela[29][TIMES] = -17;
tabela[29][TO] = -17;
tabela[29][INDEX] = 61;
tabela[30][DOT] = 62;
tabela[31][CLOSECOLCHETES] = -17;
tabela[31][CLOSEPARENTESES] = -17;
tabela[31][DIFERENT] = -17;
tabela[31][DIVIDE] = -17;
tabela[31][DO] = -17;
tabela[31][DOT] = -17;
tabela[31][EQUAL] = -17;
tabela[31][FROM] = -17;
tabela[31][GREATEREQUALTHAN] = -17;
tabela[31][GREATERTHAN] = -17;
tabela[31][IN] = -17;
tabela[31][LESSEQUALTHAN] = -17;
tabela[31][LESSTHAN] = -17;
tabela[31][MINUS] = -17;
tabela[31][MOD] = -17;
tabela[31][OPENCOLCHETES] = 51;
tabela[31][PLUS] = -17;
tabela[31][THEN] = -17;
tabela[31][TIMES] = -17;
tabela[31][TO] = -17;
tabela[31][INDEX] = 63;
tabela[32][DOT] = -46;
tabela[33][DOT] = -47;
tabela[34][DOT] = -48;
tabela[35][LETTER] = 66;
tabela[35][NUMBER] = 65;
tabela[35][TIPO] = 64;
tabela[36][AS] = -13;
tabela[36][COMMA] = 67;
tabela[37][CLOSECOLCHETES] = 68;
tabela[38][CONSTCHAR] = 25;
tabela[38][CONSTNUM] = 24;
tabela[38][ID] = 23;
tabela[38][MINUS] = 22;
tabela[38][OPENPARENTESES] = 21;
tabela[38][EXPARIT] = 69;
tabela[38][F] = 20;
tabela[38][T] = 19;
tabela[39][ID] = 70;
tabela[40][CONSTCHAR] = 25;
tabela[40][CONSTNUM] = 24;
tabela[40][ID] = 23;
tabela[40][MINUS] = 22;
tabela[40][OPENPARENTESES] = 21;
tabela[40][F] = 20;
tabela[40][T] = 71;
tabela[41][CONSTCHAR] = 25;
tabela[41][CONSTNUM] = 24;
tabela[41][ID] = 23;
tabela[41][MINUS] = 22;
tabela[41][OPENPARENTESES] = 21;
tabela[41][F] = 20;
tabela[41][T] = 72;
tabela[42][CONSTCHAR] = 25;
tabela[42][CONSTNUM] = 24;
tabela[42][ID] = 23;
tabela[42][MINUS] = 22;
tabela[42][OPENPARENTESES] = 21;
tabela[42][F] = 73;
tabela[43][CONSTCHAR] = 25;
tabela[43][CONSTNUM] = 24;
tabela[43][ID] = 23;
tabela[43][MINUS] = 22;
tabela[43][OPENPARENTESES] = 21;
tabela[43][F] = 74;
tabela[44][CONSTCHAR] = 25;
tabela[44][CONSTNUM] = 24;
tabela[44][ID] = 23;
tabela[44][MINUS] = 22;
tabela[44][OPENPARENTESES] = 21;
tabela[44][F] = 75;
tabela[45][CLOSEPARENTESES] = 76;
tabela[45][MINUS] = 41;
tabela[45][PLUS] = 40;
tabela[46][CONSTCHAR] = 25;
tabela[46][CONSTNUM] = 24;
tabela[46][ID] = 23;
tabela[46][MINUS] = 22;
tabela[46][OPENPARENTESES] = 21;
tabela[46][EXPARIT] = 77;
tabela[46][F] = 20;
tabela[46][T] = 19;
tabela[47][CLOSECOLCHETES] = -17;
tabela[47][CLOSEPARENTESES] = -17;
tabela[47][DIFERENT] = -17;
tabela[47][DIVIDE] = -17;
tabela[47][DO] = -17;
tabela[47][DOT] = -17;
tabela[47][EQUAL] = -17;
tabela[47][FROM] = -17;
tabela[47][GREATEREQUALTHAN] = -17;
tabela[47][GREATERTHAN] = -17;
tabela[47][IN] = -17;
tabela[47][LESSEQUALTHAN] = -17;
tabela[47][LESSTHAN] = -17;
tabela[47][MINUS] = -17;
tabela[47][MOD] = -17;
tabela[47][OPENCOLCHETES] = 51;
tabela[47][PLUS] = -17;
tabela[47][THEN] = -17;
tabela[47][TIMES] = -17;
tabela[47][TO] = -17;
tabela[47][INDEX] = 78;
tabela[48][CLOSECOLCHETES] = -32;
tabela[48][CLOSEPARENTESES] = -32;
tabela[48][DIFERENT] = -32;
tabela[48][DIVIDE] = -32;
tabela[48][DO] = -32;
tabela[48][DOT] = -32;
tabela[48][EQUAL] = -32;
tabela[48][GREATEREQUALTHAN] = -32;
tabela[48][GREATERTHAN] = -32;
tabela[48][IN] = -32;
tabela[48][LESSEQUALTHAN] = -32;
tabela[48][LESSTHAN] = -32;
tabela[48][MINUS] = -32;
tabela[48][MOD] = -32;
tabela[48][PLUS] = -32;
tabela[48][THEN] = -32;
tabela[48][TIMES] = -32;
tabela[48][TO] = -32;
tabela[49][CLOSECOLCHETES] = -34;
tabela[49][CLOSEPARENTESES] = -34;
tabela[49][DIFERENT] = -34;
tabela[49][DIVIDE] = -34;
tabela[49][DO] = -34;
tabela[49][DOT] = -34;
tabela[49][EQUAL] = -34;
tabela[49][GREATEREQUALTHAN] = -34;
tabela[49][GREATERTHAN] = -34;
tabela[49][IN] = -34;
tabela[49][LESSEQUALTHAN] = -34;
tabela[49][LESSTHAN] = -34;
tabela[49][MINUS] = -34;
tabela[49][MOD] = -34;
tabela[49][PLUS] = -34;
tabela[49][THEN] = -34;
tabela[49][TIMES] = -34;
tabela[49][TO] = -34;
tabela[50][CLOSECOLCHETES] = -29;
tabela[50][CLOSEPARENTESES] = -29;
tabela[50][DIFERENT] = -29;
tabela[50][DIVIDE] = -29;
tabela[50][DO] = -29;
tabela[50][DOT] = -29;
tabela[50][EQUAL] = -29;
tabela[50][GREATEREQUALTHAN] = -29;
tabela[50][GREATERTHAN] = -29;
tabela[50][IN] = -29;
tabela[50][LESSEQUALTHAN] = -29;
tabela[50][LESSTHAN] = -29;
tabela[50][MINUS] = -29;
tabela[50][MOD] = -29;
tabela[50][PLUS] = -29;
tabela[50][THEN] = -29;
tabela[50][TIMES] = -29;
tabela[50][TO] = -29;
tabela[51][CONSTCHAR] = 25;
tabela[51][CONSTNUM] = 24;
tabela[51][ID] = 23;
tabela[51][MINUS] = 22;
tabela[51][OPENPARENTESES] = 21;
tabela[51][EXPARIT] = 79;
tabela[51][F] = 20;
tabela[51][T] = 19;
tabela[52][CONSTCHAR] = 25;
tabela[52][CONSTNUM] = 24;
tabela[52][ID] = 23;
tabela[52][MINUS] = 22;
tabela[52][OPENPARENTESES] = 21;
tabela[52][EXPARIT] = 80;
tabela[52][F] = 20;
tabela[52][T] = 19;
tabela[53][CONSTCHAR] = -39;
tabela[53][CONSTNUM] = -39;
tabela[53][ID] = -39;
tabela[53][MINUS] = -39;
tabela[53][OPENPARENTESES] = -39;
tabela[54][CONSTCHAR] = -40;
tabela[54][CONSTNUM] = -40;
tabela[54][ID] = -40;
tabela[54][MINUS] = -40;
tabela[54][OPENPARENTESES] = -40;
tabela[55][CONSTCHAR] = -41;
tabela[55][CONSTNUM] = -41;
tabela[55][ID] = -41;
tabela[55][MINUS] = -41;
tabela[55][OPENPARENTESES] = -41;
tabela[56][CONSTCHAR] = -42;
tabela[56][CONSTNUM] = -42;
tabela[56][ID] = -42;
tabela[56][MINUS] = -42;
tabela[56][OPENPARENTESES] = -42;
tabela[57][CONSTCHAR] = -43;
tabela[57][CONSTNUM] = -43;
tabela[57][ID] = -43;
tabela[57][MINUS] = -43;
tabela[57][OPENPARENTESES] = -43;
tabela[58][CONSTCHAR] = -44;
tabela[58][CONSTNUM] = -44;
tabela[58][ID] = -44;
tabela[58][MINUS] = -44;
tabela[58][OPENPARENTESES] = -44;
tabela[59][FROM] = 81;
tabela[60][IN] = 82;
tabela[61][DOT] = 83;
tabela[62][$] = -11;
tabela[62][CLOSECOLCHETES] = -11;
tabela[62][DECLARE] = -11;
tabela[62][FOR] = -11;
tabela[62][FOREACH] = -11;
tabela[62][IF] = -11;
tabela[62][PRINT] = -11;
tabela[62][PUT] = -11;
tabela[62][READ] = -11;
tabela[62][RESIZE] = -11;
tabela[63][DOT] = -45;
tabela[64][DOT] = 84;
tabela[65][DOT] = -18;
tabela[66][DOT] = -19;
tabela[67][ID] = 14;
tabela[67][LISTAIDS] = 85;
tabela[68][AS] = -14;
tabela[68][COMMA] = -14;
tabela[69][DOT] = 86;
tabela[69][MINUS] = 41;
tabela[69][PLUS] = 40;
tabela[70][CLOSECOLCHETES] = -17;
tabela[70][CLOSEPARENTESES] = -17;
tabela[70][DIFERENT] = -17;
tabela[70][DIVIDE] = -17;
tabela[70][DO] = -17;
tabela[70][DOT] = -17;
tabela[70][EQUAL] = -17;
tabela[70][FROM] = -17;
tabela[70][GREATEREQUALTHAN] = -17;
tabela[70][GREATERTHAN] = -17;
tabela[70][IN] = -17;
tabela[70][LESSEQUALTHAN] = -17;
tabela[70][LESSTHAN] = -17;
tabela[70][MINUS] = -17;
tabela[70][MOD] = -17;
tabela[70][OPENCOLCHETES] = 51;
tabela[70][PLUS] = -17;
tabela[70][THEN] = -17;
tabela[70][TIMES] = -17;
tabela[70][TO] = -17;
tabela[70][INDEX] = 87;
tabela[71][CLOSECOLCHETES] = -20;
tabela[71][CLOSEPARENTESES] = -20;
tabela[71][DIFERENT] = -20;
tabela[71][DIVIDE] = 43;
tabela[71][DO] = -20;
tabela[71][DOT] = -20;
tabela[71][EQUAL] = -20;
tabela[71][GREATEREQUALTHAN] = -20;
tabela[71][GREATERTHAN] = -20;
tabela[71][IN] = -20;
tabela[71][LESSEQUALTHAN] = -20;
tabela[71][LESSTHAN] = -20;
tabela[71][MINUS] = -20;
tabela[71][MOD] = 44;
tabela[71][PLUS] = -20;
tabela[71][THEN] = -20;
tabela[71][TIMES] = 42;
tabela[71][TO] = -20;
tabela[72][CLOSECOLCHETES] = -21;
tabela[72][CLOSEPARENTESES] = -21;
tabela[72][DIFERENT] = -21;
tabela[72][DIVIDE] = 43;
tabela[72][DO] = -21;
tabela[72][DOT] = -21;
tabela[72][EQUAL] = -21;
tabela[72][GREATEREQUALTHAN] = -21;
tabela[72][GREATERTHAN] = -21;
tabela[72][IN] = -21;
tabela[72][LESSEQUALTHAN] = -21;
tabela[72][LESSTHAN] = -21;
tabela[72][MINUS] = -21;
tabela[72][MOD] = 44;
tabela[72][PLUS] = -21;
tabela[72][THEN] = -21;
tabela[72][TIMES] = 42;
tabela[72][TO] = -21;
tabela[73][CLOSECOLCHETES] = -23;
tabela[73][CLOSEPARENTESES] = -23;
tabela[73][DIFERENT] = -23;
tabela[73][DIVIDE] = -23;
tabela[73][DO] = -23;
tabela[73][DOT] = -23;
tabela[73][EQUAL] = -23;
tabela[73][GREATEREQUALTHAN] = -23;
tabela[73][GREATERTHAN] = -23;
tabela[73][IN] = -23;
tabela[73][LESSEQUALTHAN] = -23;
tabela[73][LESSTHAN] = -23;
tabela[73][MINUS] = -23;
tabela[73][MOD] = -23;
tabela[73][PLUS] = -23;
tabela[73][THEN] = -23;
tabela[73][TIMES] = -23;
tabela[73][TO] = -23;
tabela[74][CLOSECOLCHETES] = -24;
tabela[74][CLOSEPARENTESES] = -24;
tabela[74][DIFERENT] = -24;
tabela[74][DIVIDE] = -24;
tabela[74][DO] = -24;
tabela[74][DOT] = -24;
tabela[74][EQUAL] = -24;
tabela[74][GREATEREQUALTHAN] = -24;
tabela[74][GREATERTHAN] = -24;
tabela[74][IN] = -24;
tabela[74][LESSEQUALTHAN] = -24;
tabela[74][LESSTHAN] = -24;
tabela[74][MINUS] = -24;
tabela[74][MOD] = -24;
tabela[74][PLUS] = -24;
tabela[74][THEN] = -24;
tabela[74][TIMES] = -24;
tabela[74][TO] = -24;
tabela[75][CLOSECOLCHETES] = -25;
tabela[75][CLOSEPARENTESES] = -25;
tabela[75][DIFERENT] = -25;
tabela[75][DIVIDE] = -25;
tabela[75][DO] = -25;
tabela[75][DOT] = -25;
tabela[75][EQUAL] = -25;
tabela[75][GREATEREQUALTHAN] = -25;
tabela[75][GREATERTHAN] = -25;
tabela[75][IN] = -25;
tabela[75][LESSEQUALTHAN] = -25;
tabela[75][LESSTHAN] = -25;
tabela[75][MINUS] = -25;
tabela[75][MOD] = -25;
tabela[75][PLUS] = -25;
tabela[75][THEN] = -25;
tabela[75][TIMES] = -25;
tabela[75][TO] = -25;
tabela[76][CLOSECOLCHETES] = -27;
tabela[76][CLOSEPARENTESES] = -27;
tabela[76][DIFERENT] = -27;
tabela[76][DIVIDE] = -27;
tabela[76][DO] = -27;
tabela[76][DOT] = -27;
tabela[76][EQUAL] = -27;
tabela[76][GREATEREQUALTHAN] = -27;
tabela[76][GREATERTHAN] = -27;
tabela[76][IN] = -27;
tabela[76][LESSEQUALTHAN] = -27;
tabela[76][LESSTHAN] = -27;
tabela[76][MINUS] = -27;
tabela[76][MOD] = -27;
tabela[76][PLUS] = -27;
tabela[76][THEN] = -27;
tabela[76][TIMES] = -27;
tabela[76][TO] = -27;
tabela[77][CLOSEPARENTESES] = 88;
tabela[77][MINUS] = 41;
tabela[77][PLUS] = 40;
tabela[78][CLOSECOLCHETES] = -30;
tabela[78][CLOSEPARENTESES] = -30;
tabela[78][DIFERENT] = -30;
tabela[78][DIVIDE] = -30;
tabela[78][DO] = -30;
tabela[78][DOT] = -30;
tabela[78][EQUAL] = -30;
tabela[78][GREATEREQUALTHAN] = -30;
tabela[78][GREATERTHAN] = -30;
tabela[78][IN] = -30;
tabela[78][LESSEQUALTHAN] = -30;
tabela[78][LESSTHAN] = -30;
tabela[78][MINUS] = -30;
tabela[78][MOD] = -30;
tabela[78][PLUS] = -30;
tabela[78][THEN] = -30;
tabela[78][TIMES] = -30;
tabela[78][TO] = -30;
tabela[79][CLOSECOLCHETES] = 89;
tabela[79][MINUS] = 41;
tabela[79][PLUS] = 40;
tabela[80][MINUS] = 41;
tabela[80][PLUS] = 40;
tabela[80][THEN] = 90;
tabela[81][CONSTCHAR] = 25;
tabela[81][CONSTNUM] = 24;
tabela[81][ID] = 23;
tabela[81][MINUS] = 22;
tabela[81][OPENPARENTESES] = 21;
tabela[81][EXPARIT] = 91;
tabela[81][F] = 20;
tabela[81][T] = 19;
tabela[82][ID] = 92;
tabela[83][$] = -10;
tabela[83][CLOSECOLCHETES] = -10;
tabela[83][DECLARE] = -10;
tabela[83][FOR] = -10;
tabela[83][FOREACH] = -10;
tabela[83][IF] = -10;
tabela[83][PRINT] = -10;
tabela[83][PUT] = -10;
tabela[83][READ] = -10;
tabela[83][RESIZE] = -10;
tabela[84][$] = -4;
tabela[84][CLOSECOLCHETES] = -4;
tabela[84][DECLARE] = -4;
tabela[84][FOR] = -4;
tabela[84][FOREACH] = -4;
tabela[84][IF] = -4;
tabela[84][PRINT] = -4;
tabela[84][PUT] = -4;
tabela[84][READ] = -4;
tabela[84][RESIZE] = -4;
tabela[85][AS] = -12;
tabela[86][$] = -5;
tabela[86][CLOSECOLCHETES] = -5;
tabela[86][DECLARE] = -5;
tabela[86][FOR] = -5;
tabela[86][FOREACH] = -5;
tabela[86][IF] = -5;
tabela[86][PRINT] = -5;
tabela[86][PUT] = -5;
tabela[86][READ] = -5;
tabela[86][RESIZE] = -5;
tabela[87][DOT] = 93;
tabela[88][CLOSECOLCHETES] = -28;
tabela[88][CLOSEPARENTESES] = -28;
tabela[88][DIFERENT] = -28;
tabela[88][DIVIDE] = -28;
tabela[88][DO] = -28;
tabela[88][DOT] = -28;
tabela[88][EQUAL] = -28;
tabela[88][GREATEREQUALTHAN] = -28;
tabela[88][GREATERTHAN] = -28;
tabela[88][IN] = -28;
tabela[88][LESSEQUALTHAN] = -28;
tabela[88][LESSTHAN] = -28;
tabela[88][MINUS] = -28;
tabela[88][MOD] = -28;
tabela[88][PLUS] = -28;
tabela[88][THEN] = -28;
tabela[88][TIMES] = -28;
tabela[88][TO] = -28;
tabela[89][CLOSECOLCHETES] = -16;
tabela[89][CLOSEPARENTESES] = -16;
tabela[89][DIFERENT] = -16;
tabela[89][DIVIDE] = -16;
tabela[89][DO] = -16;
tabela[89][DOT] = -16;
tabela[89][EQUAL] = -16;
tabela[89][FROM] = -16;
tabela[89][GREATEREQUALTHAN] = -16;
tabela[89][GREATERTHAN] = -16;
tabela[89][IN] = -16;
tabela[89][LESSEQUALTHAN] = -16;
tabela[89][LESSTHAN] = -16;
tabela[89][MINUS] = -16;
tabela[89][MOD] = -16;
tabela[89][PLUS] = -16;
tabela[89][THEN] = -16;
tabela[89][TIMES] = -16;
tabela[89][TO] = -16;
tabela[90][OPENCOLCHETES] = 94;
tabela[91][MINUS] = 41;
tabela[91][PLUS] = 40;
tabela[91][TO] = 95;
tabela[92][DO] = 96;
tabela[93][$] = -6;
tabela[93][CLOSECOLCHETES] = -6;
tabela[93][DECLARE] = -6;
tabela[93][FOR] = -6;
tabela[93][FOREACH] = -6;
tabela[93][IF] = -6;
tabela[93][PRINT] = -6;
tabela[93][PUT] = -6;
tabela[93][READ] = -6;
tabela[93][RESIZE] = -6;
tabela[94][$] = -3;
tabela[94][CLOSECOLCHETES] = -3;
tabela[94][DECLARE] = 4;
tabela[94][FOR] = 8;
tabela[94][FOREACH] = 9;
tabela[94][IF] = 7;
tabela[94][PRINT] = 11;
tabela[94][PUT] = 6;
tabela[94][READ] = 10;
tabela[94][RESIZE] = 5;
tabela[94][COMANDO] = 3;
tabela[94][COMANDOS] = 97;
tabela[95][CONSTCHAR] = 25;
tabela[95][CONSTNUM] = 24;
tabela[95][ID] = 23;
tabela[95][MINUS] = 22;
tabela[95][OPENPARENTESES] = 21;
tabela[95][EXPARIT] = 98;
tabela[95][F] = 20;
tabela[95][T] = 19;
tabela[96][OPENCOLCHETES] = 99;
tabela[97][CLOSECOLCHETES] = 100;
tabela[98][DO] = 101;
tabela[98][MINUS] = 41;
tabela[98][PLUS] = 40;
tabela[99][$] = -3;
tabela[99][CLOSECOLCHETES] = -3;
tabela[99][DECLARE] = 4;
tabela[99][FOR] = 8;
tabela[99][FOREACH] = 9;
tabela[99][IF] = 7;
tabela[99][PRINT] = 11;
tabela[99][PUT] = 6;
tabela[99][READ] = 10;
tabela[99][RESIZE] = 5;
tabela[99][COMANDO] = 3;
tabela[99][COMANDOS] = 102;
tabela[100][$] = -38;
tabela[100][CLOSECOLCHETES] = -38;
tabela[100][DECLARE] = -38;
tabela[100][ELSE] = 104;
tabela[100][FOR] = -38;
tabela[100][FOREACH] = -38;
tabela[100][IF] = -38;
tabela[100][PRINT] = -38;
tabela[100][PUT] = -38;
tabela[100][READ] = -38;
tabela[100][RESIZE] = -38;
tabela[100][SENAO] = 103;
tabela[101][OPENCOLCHETES] = 105;
tabela[102][CLOSECOLCHETES] = 106;
tabela[103][$] = -7;
tabela[103][CLOSECOLCHETES] = -7;
tabela[103][DECLARE] = -7;
tabela[103][FOR] = -7;
tabela[103][FOREACH] = -7;
tabela[103][IF] = -7;
tabela[103][PRINT] = -7;
tabela[103][PUT] = -7;
tabela[103][READ] = -7;
tabela[103][RESIZE] = -7;
tabela[104][OPENCOLCHETES] = 107;
tabela[105][$] = -3;
tabela[105][CLOSECOLCHETES] = -3;
tabela[105][DECLARE] = 4;
tabela[105][FOR] = 8;
tabela[105][FOREACH] = 9;
tabela[105][IF] = 7;
tabela[105][PRINT] = 11;
tabela[105][PUT] = 6;
tabela[105][READ] = 10;
tabela[105][RESIZE] = 5;
tabela[105][COMANDO] = 3;
tabela[105][COMANDOS] = 108;
tabela[106][$] = -9;
tabela[106][CLOSECOLCHETES] = -9;
tabela[106][DECLARE] = -9;
tabela[106][FOR] = -9;
tabela[106][FOREACH] = -9;
tabela[106][IF] = -9;
tabela[106][PRINT] = -9;
tabela[106][PUT] = -9;
tabela[106][READ] = -9;
tabela[106][RESIZE] = -9;
tabela[107][$] = -3;
tabela[107][CLOSECOLCHETES] = -3;
tabela[107][DECLARE] = 4;
tabela[107][FOR] = 8;
tabela[107][FOREACH] = 9;
tabela[107][IF] = 7;
tabela[107][PRINT] = 11;
tabela[107][PUT] = 6;
tabela[107][READ] = 10;
tabela[107][RESIZE] = 5;
tabela[107][COMANDO] = 3;
tabela[107][COMANDOS] = 109;
tabela[108][CLOSECOLCHETES] = 110;
tabela[109][CLOSECOLCHETES] = 111;
tabela[110][$] = -8;
tabela[110][CLOSECOLCHETES] = -8;
tabela[110][DECLARE] = -8;
tabela[110][FOR] = -8;
tabela[110][FOREACH] = -8;
tabela[110][IF] = -8;
tabela[110][PRINT] = -8;
tabela[110][PUT] = -8;
tabela[110][READ] = -8;
tabela[110][RESIZE] = -8;
tabela[111][$] = -37;
tabela[111][CLOSECOLCHETES] = -37;
tabela[111][DECLARE] = -37;
tabela[111][FOR] = -37;
tabela[111][FOREACH] = -37;
tabela[111][IF] = -37;
tabela[111][PRINT] = -37;
tabela[111][PUT] = -37;
tabela[111][READ] = -37;
tabela[111][RESIZE] = -37;

    //PRODUÇÕES
    prod reducoes[49];

    reducoes[0].p = SS;
    reducoes[0].q = (int*) malloc(2*sizeof(int));
    reducoes[0].q[0] = S;
    reducoes[0].q[1] = $;
    reducoes[0].n = 2;

    reducoes[1].p = S;
    reducoes[1].q = (int*) malloc(sizeof(int));
    reducoes[1].q[0] = COMANDOS;
    reducoes[1].n = 1;

    reducoes[2].p = COMANDOS;
    reducoes[2].q = (int*) malloc(2*sizeof(int));
    reducoes[2].q[0] = COMANDO;
    reducoes[2].q[1] = COMANDOS;
    reducoes[2].n = 2;

    reducoes[3].p = COMANDOS;
    reducoes[3].q = (int*) malloc (sizeof(int));
    reducoes[3].q[0] = VAZIO;
    reducoes[3].n = 0;

    reducoes[4].p = COMANDO;
    reducoes[4].q = (int*) malloc(5*sizeof(int));
    reducoes[4].q[0] = DECLARE;
    reducoes[4].q[1] = LISTAIDS;
    reducoes[4].q[2] = AS;
    reducoes[4].q[3] = TIPO;
    reducoes[4].q[4] = DOT;
    reducoes[4].n = 5;

    reducoes[5].p = COMANDO;
    reducoes[5].q = (int*) malloc(4*sizeof(int));
    reducoes[5].q[0] = RESIZE;
    reducoes[5].q[1] = ID;
    reducoes[5].q[2] = TO;
    reducoes[5].q[3] = EXPARIT;
    reducoes[5].q[4] = DOT;
    reducoes[5].n = 5;

    reducoes[6].p = COMANDO;
    reducoes[6].q = (int*) malloc(6*sizeof(int));
    reducoes[6].q[0] = PUT;
    reducoes[6].q[1] = PUTAVEL;
    reducoes[6].q[2] = IN;
    reducoes[6].q[3] = ID;
    reducoes[6].q[4] = INDEX;
    reducoes[6].q[5] = DOT;
    reducoes[6].n = 6;

    reducoes[7].p = COMANDO;
    reducoes[7].q = (int*) malloc(9*sizeof(int));
    reducoes[7].q[0] = IF;
    reducoes[7].q[1] = EXPARIT;
    reducoes[7].q[2] = OPRREL;
    reducoes[7].q[3] = EXPARIT;
    reducoes[7].q[4] = THEN;
    reducoes[7].q[5] = OPENCOLCHETES;
    reducoes[7].q[6] = COMANDOS;
    reducoes[7].q[7] = CLOSECOLCHETES;
    reducoes[7].q[8] = SENAO;
    reducoes[7].n = 9;

    reducoes[8].p = COMANDO;
    reducoes[8].q = (int*) malloc(11*sizeof(int));
    reducoes[8].q[0] = FOR;
    reducoes[8].q[1] = ID;
    reducoes[8].q[2] = INDEX;
    reducoes[8].q[3] = FROM;
    reducoes[8].q[4] = EXPARIT;
    reducoes[8].q[5] = TO;
    reducoes[8].q[6] = EXPARIT;
    reducoes[8].q[7] = DO;
    reducoes[8].q[8] = OPENCOLCHETES;
    reducoes[8].q[9] = COMANDOS;
    reducoes[8].q[10] = CLOSECOLCHETES;
    reducoes[8].n = 11;

    reducoes[9].p = COMANDO;
    reducoes[9].q = (int*) malloc(9*sizeof(int));
    reducoes[9].q[0] = FOREACH;
    reducoes[9].q[1] = ID;
    reducoes[9].q[2] = INDEX;
    reducoes[9].q[3] = IN;
    reducoes[9].q[4] = ID;
    reducoes[9].q[5] = DO;
    reducoes[9].q[6] = OPENCOLCHETES;
    reducoes[9].q[7] = COMANDOS;
    reducoes[9].q[8] = CLOSECOLCHETES;
    reducoes[9].n = 9;

    reducoes[10].p = COMANDO;
    reducoes[10].q = (int*) malloc(4*sizeof(int));
    reducoes[10].q[0] = READ;
    reducoes[10].q[1] = ID;
    reducoes[10].q[2] = INDEX;
    reducoes[10].q[3] = DOT;
    reducoes[10].n = 4;

    reducoes[11].p = COMANDO;
    reducoes[11].q = (int*) malloc(3*sizeof(int));
    reducoes[11].q[0] = PRINT;
    reducoes[11].q[1] = IMPRIMIVEL;
    reducoes[11].q[2] = DOT;
    reducoes[11].n = 3;

    reducoes[12].p = LISTAIDS;
    reducoes[12].q = (int*) malloc(4*sizeof(int));
    reducoes[12].q[0] = ID;
    reducoes[12].q[1] = NINDEX;
    reducoes[12].q[2] = COMMA;
    reducoes[12].q[3] = LISTAIDS;
    reducoes[12].n = 4;

    reducoes[13].p = LISTAIDS;
    reducoes[13].q = (int*) malloc(2*sizeof(int));
    reducoes[13].q[0] = ID;
    reducoes[13].q[1] = NINDEX;
    reducoes[13].n = 2;

    reducoes[14].p = NINDEX;
    reducoes[14].q = (int*) malloc(2*sizeof(int));
    reducoes[14].q[0] = OPENCOLCHETES;
    reducoes[14].q[1] = CLOSECOLCHETES;
    reducoes[14].n = 2;

    reducoes[15].p = NINDEX;
    reducoes[15].q = (int*) malloc(1*sizeof(int));
    reducoes[15].q[0] = VAZIO;
    reducoes[15].n = 0;

    reducoes[16].p = INDEX;
    reducoes[16].q = (int*) malloc(3*sizeof(int));
    reducoes[16].q[0] = OPENCOLCHETES;
    reducoes[16].q[1] = EXPARIT;
    reducoes[16].q[2] = CLOSECOLCHETES;
    reducoes[16].n = 3;

    reducoes[17].p = INDEX;
    reducoes[17].q = (int*) malloc(1*sizeof(int));
    reducoes[17].q[0] = VAZIO;
    reducoes[17].n = 0;

    reducoes[18].p = TIPO;
    reducoes[18].q = (int*) malloc(1*sizeof(int));
    reducoes[18].q[0] = NUMBER;
    reducoes[18].n = 1;

    reducoes[19].p = TIPO;
    reducoes[19].q = (int*) malloc(1*sizeof(int));
    reducoes[19].q[0] = LETTER;
    reducoes[19].n = 1;

    reducoes[20].p = EXPARIT;
    reducoes[20].q = (int*) malloc(3*sizeof(int));
    reducoes[20].q[0] = EXPARIT;
    reducoes[20].q[1] = PLUS;
    reducoes[20].q[2] = T;
    reducoes[20].n = 3;

    reducoes[21].p = EXPARIT;
    reducoes[21].q = (int*) malloc(3*sizeof(int));
    reducoes[21].q[0] = EXPARIT;
    reducoes[21].q[1] = MINUS;
    reducoes[21].q[2] = T;
    reducoes[21].n = 3;


    reducoes[22].p = EXPARIT;
    reducoes[22].q = (int*) malloc(sizeof(int));
    reducoes[22].q[0] = T;
    reducoes[22].n = 1;

    reducoes[23].p = T;
    reducoes[23].q = (int*) malloc(3*sizeof(int));
    reducoes[23].q[0] = T;
    reducoes[23].q[1] = TIMES;
    reducoes[23].q[2] = F;
    reducoes[23].n = 3;

    reducoes[24].p = T;
    reducoes[24].q = (int*) malloc(3*sizeof(int));
    reducoes[24].q[0] = T;
    reducoes[24].q[1] = DIVIDE;
    reducoes[24].q[2] = F;
    reducoes[24].n = 3;

    reducoes[25].p = T;
    reducoes[25].q = (int*) malloc(3*sizeof(int));
    reducoes[25].q[0] = T;
    reducoes[25].q[1] = MOD;
    reducoes[25].q[2] = F;
    reducoes[25].n = 3;

    reducoes[26].p = T;
    reducoes[26].q = (int*) malloc(sizeof(int));
    reducoes[26].q[0] = F;
    reducoes[26].n = 1;

    reducoes[27].p = F;
    reducoes[27].q = (int*) malloc(3*sizeof(int));
    reducoes[27].q[0] = OPENPARENTESES;
    reducoes[27].q[1] = EXPARIT;
    reducoes[27].q[2] = CLOSEPARENTESES;
    reducoes[27].n = 3;

    reducoes[28].p = F;
    reducoes[28].q = (int*) malloc(4*sizeof(int));
    reducoes[28].q[0] = MINUS;
    reducoes[28].q[1] = OPENPARENTESES;
    reducoes[28].q[2] = EXPARIT;
    reducoes[28].q[3] = CLOSEPARENTESES;
    reducoes[28].n = 4;

    reducoes[29].p = F;
    reducoes[29].q = (int*) malloc(2*sizeof(int));
    reducoes[29].q[0] = ID;
    reducoes[29].q[1] = INDEX;
    reducoes[29].n = 2;

    reducoes[30].p = F;
    reducoes[30].q = (int*) malloc(3*sizeof(int));
    reducoes[30].q[0] = MINUS;
    reducoes[30].q[1] = ID;
    reducoes[30].q[2] = INDEX;
    reducoes[30].n = 3;

    reducoes[31].p = F;
    reducoes[31].q = (int*) malloc(sizeof(int));
    reducoes[31].q[0] = CONSTNUM;
    reducoes[31].n = 1;

    reducoes[32].p = F;
    reducoes[32].q = (int*) malloc(2*sizeof(int));
    reducoes[32].q[0] = MINUS;
    reducoes[32].q[1] = CONSTNUM;
    reducoes[32].n = 2;

    reducoes[33].p = F;
    reducoes[33].q = (int*) malloc(sizeof(int));
    reducoes[33].q[0] = CONSTCHAR;
    reducoes[33].n = 1;

    reducoes[34].p = F;
    reducoes[34].q = (int*) malloc(2*sizeof(int));
    reducoes[34].q[0] = MINUS;
    reducoes[34].q[1] = CONSTCHAR;
    reducoes[34].n = 2;

    reducoes[35].p = PUTAVEL;
    reducoes[35].q = (int*) malloc(sizeof(int));
    reducoes[35].q[0] = EXPARIT;
    reducoes[35].n = 1;

    reducoes[36].p = PUTAVEL;
    reducoes[36].q = (int*) malloc(sizeof(int));
    reducoes[36].q[0] = CONSTSTRING;
    reducoes[36].n = 1;

    reducoes[37].p = SENAO;
    reducoes[37].q = (int*) malloc(4*sizeof(int));
    reducoes[37].q[0] = ELSE;
    reducoes[37].q[1] = OPENCOLCHETES;
    reducoes[37].q[2] = COMANDOS;
    reducoes[37].q[3] = CLOSECOLCHETES;
    reducoes[37].n = 4;

    reducoes[38].p = SENAO;
    reducoes[38].q = (int*) malloc(sizeof(int));
    reducoes[38].q[0] = VAZIO;
    reducoes[38].n = 0;

    reducoes[39].p = OPRREL;
    reducoes[39].q = (int*) malloc(sizeof(int));
    reducoes[39].q[0] = LESSTHAN;
    reducoes[39].n = 1;

    reducoes[40].p = OPRREL;
    reducoes[40].q = (int*) malloc(sizeof(int));
    reducoes[40].q[0] = GREATERTHAN;
    reducoes[40].n = 1;

    reducoes[41].p = OPRREL;
    reducoes[41].q = (int*) malloc(sizeof(int));
    reducoes[41].q[0] = LESSEQUALTHAN;
    reducoes[41].n = 1;

    reducoes[42].p = OPRREL;
    reducoes[42].q = (int*) malloc(sizeof(int));
    reducoes[42].q[0] = GREATEREQUALTHAN;
    reducoes[42].n = 1;

    reducoes[43].p = OPRREL;
    reducoes[43].q = (int*) malloc(sizeof(int));
    reducoes[43].q[0] = EQUAL;
    reducoes[43].n = 1;

    reducoes[44].p = OPRREL;
    reducoes[44].q = (int*) malloc(sizeof(int));
    reducoes[44].q[0] = DIFERENT;
    reducoes[44].n = 1;

    reducoes[45].p = IMPRIMIVEL;
    reducoes[45].q = (int*) malloc(2*sizeof(int));
    reducoes[45].q[0] = ID;
    reducoes[45].q[1] = INDEX;
    reducoes[45].n = 2;

    reducoes[46].p = IMPRIMIVEL;
    reducoes[46].q = (int*) malloc(sizeof(int));
    reducoes[46].q[0] = CONSTNUM;
    reducoes[46].n = 1;

    reducoes[47].p = IMPRIMIVEL;
    reducoes[47].q = (int*) malloc(sizeof(int));
    reducoes[47].q[0] = CONSTCHAR;
    reducoes[47].n = 1;

    reducoes[48].p = IMPRIMIVEL;
    reducoes[48].q = (int*) malloc(sizeof(int));
    reducoes[48].q[0] = CONSTSTRING;
    reducoes[48].n = 1;



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
    } else {
        return 0;
    }


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
                // printf("%s\n", stringVal);
                while(!inseriToken(&l, GREATEREQUALTHAN, stringVal));
                nextPos++;
            } else salvaTokenSeparadorUnico(nextChar, &l);
        } else if (nextChar == '<'){
            if (entrada[nextPos + 1] == '=') {
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                // printf("%s\n", stringVal);
                while(!inseriToken(&l, LESSEQUALTHAN, stringVal));
                nextPos++;
            } else if (entrada[nextPos + 1] == '>'){
                while(!formString(entrada, &stringVal, nextPos, nextPos + 1));
                // printf("%s\n", stringVal);
                while(!inseriToken(&l, DIFERENT, stringVal));
            } else salvaTokenSeparadorUnico(nextChar, &l);
        } else if (isSeparator(nextChar)){
            salvaTokenSeparadorUnico(nextChar, &l);
        } else if (nextChar != ' ' && nextChar != '\t')
            printErroLexico(entrada, nextPos, nextPos, line);
        nextPos++;
    }

    if (erroL > 0)
        return 0;

    //Analise Sintatica
    pilhaToken pEntrada;
    pilhaToken pTrabalho;
    inicializaPilhaToken(&pEntrada);
    inicializaPilhaToken(&pTrabalho);

    listaToken p;
    listaToken pant;

    while(!(empilhaToken(&pEntrada, $, -1)));
    for (p = l, pant = NULL; (p); pant = p, p = p->prox);
    for (;(pant); pant = pant->ant) {
        while(!(empilhaToken(&pEntrada, pant->tokenVal, -1)));
    }

    int acao;
    int estado = 0;
    pilhaToken x;

    while(topoToken(pEntrada, &x)) {
        // printf("Token lido: %d\n", x->token);
        acao = slr1(x->token, estado, tabela);
        // printf("Acao: %d\n", acao);
        if (acao == ACEITA) {
            printf("SIM\n");
            break;
        } else if (acao > 0) {
            // printf("Vai para estado: %d\n", acao);
            desempilhaToken(&pEntrada, &x);
            empilhaPTrabalho(&pTrabalho, x, estado);
            // topoToken(pTrabalho, &x);
            // printf("Empilhou na de trabalho: %d\n", x->token);
            estado = acao;
        } else if (acao < 0) {
            acao = acao*(-1);
            // printf("Aplica reducao: %d\n", acao);
            int i;
            // printf("Desempilhou:");
            for (i = 0; i < reducoes[acao].n; i++){
                desempilhaToken(&pTrabalho, &x);
                // printf(" %d", x->token);
            }
            // printf("\n");
            if(reducoes[acao].n > 0) {
                estado = x->estado;
                // printf("Voltou para o estado: %d\n", estado);
            }
            empilhaToken(&pEntrada, reducoes[acao].p, -1);
            // topoToken(pEntrada, &x);
            // printf("Empilhado na entrada: %d\n", x->token);
        } else if(acao == 0){
            printf("NAO\n");
            break;
        }
        // printf("================================\n");
    }

    return 0;
}
