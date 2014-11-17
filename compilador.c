#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

int checkIdentificador(char *e,int pos){
	int aux = pos + 1;
	while (isalpha(e[aux]) || isNumber(e[aux])){
    	aux++;
    }
	//salvatoken
	return aux - 1;
}
int checkNumber(char *e, int pos, int l){
	int aux = pos + 1;
    bool letter = false;
	while (isNumber(e[aux]) || isalpha(e[aux])) {
		if (isalpha(e[aux]))
            letter = true;
        aux++;
	}
	//salvatoken
	if (aux - pos > 10 || letter)
        printErroLexico(e, pos, aux-1, l);
	return aux - 1;
}

int checkConstChar(char *e,int pos, int l){
	int aux = pos + 1;
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
                //salvatoken
            }
        } else {
            printErroLexico(e, pos, aux, l);
        }
    } else if (len == 1){
        if (e[aux-1] == '\\' || e[aux-1] == '\"') {
            printErroLexico(e, pos, aux, l);
        } else {
            //salvatoken
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

    while (entrada[nextPos] != 0){//sempre retornar o index do ultimo char usado para que aqui seja incrementado        
        nextChar = entrada[nextPos];
        if (isalpha(nextChar)) {
            nextPos = checkIdentificador(entrada, nextPos);
        } else if (isNumber(nextChar)) {
            nextPos = checkNumber(entrada, nextPos, line);
        } else if (nextChar == '\''){
            nextPos = checkConstChar(entrada, nextPos, line);
        } else if (nextChar == '\"'){
            nextPos = checkString(entrada, nextPos, line);
        } else if (nextChar == '\n'){
            line++;
        } else if (nextChar == '>'){
            if (entrada[nextPos + 1] == '=');
                //salva token
        } else if (nextChar == '<'){
            if (entrada[nextPos + 1] == '=' || entrada[nextPos + 1] == '>');
                //salva token
        } else if (isSeparator(nextChar)){
            //salva token
        } else if (nextChar != ' ' && nextChar != '\t')
            printErroLexico(entrada, nextPos, nextPos, line);
        nextPos++;
    }

    return 0;
}