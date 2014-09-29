#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void append(char *s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len+1] = 0;
}

_Bool verificadorCaracteres (char entrada[]) {
    int i;
    for (i = 0; entrada[i] != 0; i++)
    {   //Quebra de linha aparecendo como 13 que na tabela ascii é indicado como carriage return
        if(entrada[i] < 9 || (entrada[i] > 10 && entrada[i] < 32 && entrada[i] != 13) || entrada[i] > 126){
            return false;
        }
    }
    return true;
}

char *nextElemento(char entrada[], int *nextChar) {
    char *elemento = (char *) calloc(500,sizeof(char)); // temporario
    char c = entrada[(*nextChar)++];
    while(c != 0){
        // printf("%d - ", *nextChar);
        // printf("%c\n", c);
        switch(c){
            case 9://tab
            case 10://new line
            case 13://new line bugado
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
            case 92:// contra-barra '\'
                if (elemento[0] == 39 || elemento[0] == 34){
                    append(elemento, c);
                    c = entrada[(*nextChar)++];
                } else {
                    c = 0;
                }
                break;

            case 39: // aspas simples '
                if (elemento[0] == 0 || elemento[0] == 34){
                    append(elemento, c);
                    c = entrada[(*nextChar)++];
                } else if (elemento[0] == 39){
                    append(elemento, c);
                    c = 0;
                } else {
                    c = 0;
                }
                break;

            case 34: // aspas duplas "
                if (elemento[0] == 0 || elemento[0] == 39){
                    append(elemento, c);
                    c = entrada[(*nextChar)++];
                } else if (elemento[0] == 34){
                    append(elemento, c);
                    c = 0;
                } else {
                    c = 0;
                }
                break;

            default:
                append(elemento, c);
                c = entrada[(*nextChar)++];
            break;
        }
    }
    return elemento;
}




int main(int argc, char const *argv[]) {
    char entrada[500]; // temporario
    entrada[0] = 0;

    if (argc > 1)
    {
        FILE *program;
        program = fopen(argv[1], "r");
        if (program){
            char c;
            while ((c = getc(program)) != EOF){
                append(entrada, c);                
            }
            fclose(program);
        }
    }


    if(!verificadorCaracteres(entrada)){
        printf("ARQUIVO INVÁLIDO!\n");
        return 0;
    }

    int nextChar = 0;
    while(nextChar < strlen(entrada)){
        char *elemento = nextElemento(entrada, &nextChar);

        printf("%s\n", elemento);
        // printf("%d\n", (int) strlen(elemento));
    }

    return 0;
}