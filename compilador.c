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

_Bool verificadorCaracteres (char entrada[]) {
    int i;
    for (i = 0; entrada[i] != 0; i++) {
        if(entrada[i] < 9 || (entrada[i] > 10 && entrada[i] < 32) || entrada[i] > 126){
            return false;
        }
    }
    return true;
}

_Bool isLetter(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 || c <= 122))
        return true
    return false
}
_Bool isNumber(char c) {
    if (c >= 48 && c <= 57)
        return true
    return false
}
_Bool isSeparator(char elemento){
    if( elemento == ',' || elemento == '.' || elemento == '[' || elemento == ']'
        || elemento == '+' || elemento == '-' || elemento == '*' || elemento == '/'
        || elemento == '%' || elemento == '(' || elemento == ')' || elemento == '='
        || elemento == '<' || elemento == '>' || elemento == ' ' || elemento == '\t')
            return true;
    return false;
}

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
        if (isLetter(nextChar))
            nextPos = checkIdentificador(entrada, nextPos, line);
        else if (isNumber(nextChar))
            nextPos = checkNumber(entrada, nextPos, line);
        else if (nextChar == '\'')
            nextPos = checkConstChar(entrada, nextPos, line);
        else if (nextChar == '\"')
            nextPos = checkString(entrada, nextPos, line);
        else if (nextChar == '\n')
            line++;
        else if (nextChar == '>')
            if (entrada[nextPos + 1] == '=')
                //salva token
        else if (nextChar == '<')
            if (entrada[nextPos + 1] == '=' || entrada[nextPos + 1] == '>')
                //salva token
        else if (isSeparator(nextChar))
            //salva token
        else if (nextChar != ' ' && nextChar != '\t')
            printErroLexico(nextChar, line);
        nextPos++;
    }

    return 0
}    



