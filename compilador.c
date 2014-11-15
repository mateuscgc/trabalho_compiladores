#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


long getFileSize(FILE *file){
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

// _Bool isLetter(char c) {
//     if ((c >= 65 && c <= 90) || (c >= 97 || c <= 122))
//         return true;
//     return false;
// }
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
		//printErro
	return aux - 1;
}

int checkConstChar(char *e,int pos, int l){
	int aux = pos + 1;
	while (((e[aux] != '\'' && e[aux] != '\"') || e[aux-1] == '\\')  && e[aux] != '\t' && e[aux] != '\n' && e[aux] != 0){
		aux++;
	}
    int len = aux - pos -1;
	if(e[aux] != '\'' || len >= 3){
        if(e[aux] != 0)
            //printErroTudo
            printErroLexico(e, pos, aux, l);
        else 
            //printErro-e[aux]
            printErroLexico(e, pos, aux-1, l);
	} else if(len == 2) {
        if (e[aux-1] == 't' || e[aux-1] == 'n' || e[aux-1] == '\\' || e[aux-1] == '\'' || e[aux-1] == '\"') {
            if (e[aux-2] != '\\')
                //printErroTudo;
                printErroLexico(e, pos, aux, l);
            else{
                //salvatoken
            }
        } else {
            //printErroTudo
            printErroLexico(e, pos, aux, l);
        }
    } else if (len == 1){
        if (e[aux-1] == '\\' || e[aux-1] == '\"') {
            //printErroTudo
            printErroLexico(e, pos, aux, l);
        } else {
            //salvatoken
        }
    } else {
        //printErroTudo
        printErroLexico(e, pos, aux, l);
    }
    return aux;
}

int checkString(char *e, int pos, int l){
    int aux = pos  + 1;
    int size = 0;
    while (((e[aux] != '\'' && e[aux] != '\"') || e[aux-1] == '\\')  && e[aux] != '\t' && e[aux] != '\n' && e[aux] != 0){ // O problema é "\\'" por exemplo
        if ((e[aux] != '\'' && e[aux] != '\"' && e[aux] != 't'  && e[aux] != 'n'  && e[aux] != '\\') || e[aux-1] != '\\')
            size++;
        aux++;
    }
    int len = aux - pos -1;
    if(e[aux] != '\"'){
        if(e[aux] != 0)
            //printErroTudo
            printErroLexico(e, pos, aux, l);
        else 
            //printErro-e[aux]
            printErroLexico(e, pos, aux-1, l);
    } else if (size > 257){
        //printErroTudo
        printErroLexico(e, pos, aux, l);
    } else {
        int i;
        for (i = pos+1; i < aux; i++){
            if(e[i] == '\\' && e[i+1] != '\'' && e[i+1] != '\"' && e[i+1] != '\n' && e[i+1] != '\t' && e[i+1] != '\\') {// O problema é "\\'" por exemplo
                //printErro\
                printErroLexico(e, i, i, l);
            }
        }
    }
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
        // printf("%c\n", nextChar);
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
        } else if (nextChar != ' ' && nextChar != '\t'){
            // printf("que?\n");
            printErroLexico(entrada, nextPos, nextPos, line);
        } else {
            // printf("como assim\n");
        }
        nextPos++;
    }

    return 0;
}