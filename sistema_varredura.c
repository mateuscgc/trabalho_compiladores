#include <stdio.h>
#include <stdbool.h>


_Bool verificadorCaracteres (char entrada[])
{
    int i;
    for (i = 0; entrada[i] != 0; i++)
    {
        if(entrada[i] < 9 || (entrada[i] > 10 && entrada[i] < 32) || entrada[i] > 126)
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    if (argc > 1)
    {
        FILE *program;
        program = fopen(argv[1], "r");
        if (program){
            char c;
            while ((c = getc(program)) != EOF)
                putchar(c);
            fclose(program);
        }
    }


    if(verificadorCaracteres("alo alo alo !<> + ADFE    "))
        printf("Tudo certo\n");

    return 0;
}       