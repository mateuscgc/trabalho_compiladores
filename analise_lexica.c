#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void append(char *s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len+1] = 0;
}

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

char *nextElemento(char entrada[], int *nextChar, int *line) {
    char *elemento = (char *) calloc(strlen(entrada),sizeof(char)); // temporario
    char c = entrada[(*nextChar)];
    while(c != 0){
        // printf("%d\n", *nextChar);
        switch(c){
            case 9://tab
                ++(*nextChar);
                c = 0;
                break;
            case 10://new line
                (*line)++;
                ++(*nextChar);
                c = 0;
                break;
            case ' ':
            case ',':
            case '.':
            case '+':
            case '-':
            case '%':
            case '*':
            case '/':
            case '(':
            case ')':
            case '[':
            case ']':
            case '=':
            case '<':
            case '>':
            case '\\':
                if (elemento[0] == '\'' || elemento[0] == '\"'){
                    append(elemento, c);
                    c = entrada[++(*nextChar)];
                } else if(elemento[0] == 0){
                    append(elemento, c);
                    ++(*nextChar);
                    c = 0;
                } else{
                    c = 0;
                }
                break;

            case '\'':
                if (elemento[0] == 0 || elemento[0] == '\"' || entrada[(*nextChar) - 1] == '\\'){
                    append(elemento, c);
                    c = entrada[++(*nextChar)];
                } else if (elemento[0] == '\''){
                    append(elemento, c);
                    ++(*nextChar);
                    c = 0;
                } else {
                    c = 0;
                }
                break;

            case '\"':
                if (elemento[0] == 0 || elemento[0] == '\'' || entrada[(*nextChar) - 1] == '\\'){
                    append(elemento, c);
                    c = entrada[++(*nextChar)];
                } else if (elemento[0] == '\"'){
                    append(elemento, c);
                    ++(*nextChar);
                    c = 0;
                } else {
                    c = 0;
                }
                break;

            default:
                append(elemento, c);
                c = entrada[++(*nextChar)];
                break;
        }
    }
    return elemento;
}

_Bool checkFirst(char e[], char c){
    return e[0] == c ? true : false;
}
_Bool checkLast(char e[], char c){
    return e[strlen(e) - 1] == c ? true : false;
}

int checkInside(char e[], int start, char c){
    int i;
    for (i = 0; i < strlen(e); i++)
        if (e[i] == c)
            return i;
    return -1;
}

void printErro(char e[], int l){
    printf("LINHA %d: %s\n", l, e);
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

    int nextChar = 0;
    int line = 1;
    int contraBarra;
    int length, i;
    char erro[3];
    _Bool number = true;
    while(nextChar < strlen(entrada)){
        char *elemento = nextElemento(entrada, &nextChar, &line);
        // printf("%s --> %d\n", elemento, line);
        length = strlen(elemento);
        if (length != 0){
            // Checagem de Erros
            if(checkFirst(elemento, '\"') && !checkLast(elemento, '\"') ) {
                printErro(elemento, line);

            } else if(checkFirst(elemento, '\'') && !checkLast(elemento, '\'') ) {
                printErro(elemento, line);

            } else if ( checkFirst(elemento, '\'') && checkLast(elemento, '\'') ){
                if (elemento[length - 2] == '\\')
                    printErro(elemento, line);
                if (length <= 2)
                    printErro(elemento, line);
                if( length > 4 || (length > 3 && elemento[1] != '\\'))
                    printErro(elemento, line);
                if(elemento[1] == '\\'){
                    if (length == 3)
                        strncpy(erro, &elemento[1], 1);
                        printErro(erro, line);
                    if (elemento[2] != 't' && elemento[2] != 'n' && elemento[2] != '\\' && elemento[2] != '\'' && elemento[2] != '\"') {
                        strncpy(erro, &elemento[1], 1);
                        printErro(erro, line);
                    }
                } else if(elemento[1] == '\"') {
                    strncpy(erro, &elemento[1], 1);
                    printErro(erro, line);
                }

            } else if ( checkFirst(elemento, '\"') && checkLast(elemento, '\"') ){
                if (elemento[length - 2] == '\\')
                    printErro(elemento, line);
                if (length <= 2)
                    printErro(elemento, line);
                contraBarra = 0;
                
                for (i = 0; i < length; i++)
                    if(elemento[i] == '\\')
                        contraBarra++;
                if ((length - contraBarra) > 256)
                    printErro (elemento, line);

                for (i = 1; i < (length - 1); i++)
                    if( elemento[i] == '\'' || elemento[i] == '\"'){
                        if(elemento[i - 1] != '\\'){
                            strncpy(erro, &elemento[i], 1);
                            printErro(erro, line);
                        }
                    }
                
                for (i = 0; i < length; i++)
                    if(elemento[i] == '\\')
                        if (elemento[i + 1] != 't' && elemento[i + 1] != 'n' && elemento[i + 1] != '\\' && elemento[i + 1] != '\'' && elemento[i + 1] != '\"'){
                            strncpy(erro, &elemento[i], 2);
                            printErro(erro, line);
                        }

            } else { // não é limitado por '' ou ""
                number = true;
                for (i = 0; i < length; i++){
                    if(!isalpha(elemento[i]) && !isdigit(elemento[i]) && elemento[i] != ',' && elemento[i] != '.' && elemento[i] != '['
                        && elemento[i] != ']' && elemento[i] != '+' && elemento[i] != '-' && elemento[i] != '*' && elemento[i] != '/'
                        && elemento[i] != '%' && elemento[i] != '(' && elemento[i] != ')' && elemento[i] != '=' && elemento[i] != '<'
                        && elemento[i] != '>' && elemento[i] != ' ') { // Verifica se os caracteres não são nem letras nem números && elemento[i] != '' && elemento[i] != '' && elemento[i] != '' && elemento[i] != ''
                        strncpy(erro, &elemento[i], 1); // Imprime o erro de caracter invalido
                        printErro(erro, line);
                        number = false;
                    } else {
                        if(isalpha(elemento[i])){  // Verifica se tem alguma letra no elemento 
                            if(!isalpha(elemento[0])) {// e então verifica se o primeiro é letra
                                printErro(elemento, line); // se não for printa o erro de um ID invalido
                                break;
                            } else // Se não
                                number = false; // Macar que não é um número
                        }                        
                    }
                }
                if(number && length > 10) // Verifica se é um número e se length > 10
                    printErro(elemento, line); // Imprime o erro de numero maior que o limite
            }
        }
        
    }

    return 0;
}