#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int execer2(char *str, char *prog, char **pargs, char **env)
{

    char **args = malloc(sizeof(char *) * (strlen(str) + 2));
    memcpy(args, pargs, sizeof(char *) * (strlen(str) + 2));
    memmove(args + 2, args, sizeof(char *) * (strlen(str) + 1));
    args[0] = prog;
    args[1] = str;
    int ret = execve(prog, args, env);
    fprintf(stderr, "Error: execve failed\n");
    exit(ret);
}


int main(int argc, char **argv, char **env)
{

    char *tobe_lexed = argv[1];
    char *new;
    if (argc < 3)
    {
        printf("Usage: ./ilexer 'string to be lexed' program_abs_Path\n");
        return (1);
    }
    int i;
    i = 0;
    new = malloc(sizeof(char) * strlen(tobe_lexed));
    while (*tobe_lexed != '\0')
    {
        if (*tobe_lexed == ' ' || *tobe_lexed == '\t' || *tobe_lexed == '\n')
        {
            tobe_lexed++;
            continue;
        }
        if (*tobe_lexed == '"')
        {
            tobe_lexed++;
            while (*tobe_lexed != 0 && *tobe_lexed != '"')
            {
                new[i] = *tobe_lexed;
                tobe_lexed++;
                i++;
            }
            if (*tobe_lexed == '"')
            {
                tobe_lexed++;
            }
            else
            {
                fprintf(stderr, "Error: Missing closing quote\n");
                exit(1);
            }            
            continue;
        }
        new[i] = *tobe_lexed;
        i++;
        tobe_lexed++;
    }
    new[i] = 0;

    int ret = execer2(new, argv[2], argv + 3 , env);
    return (ret);
}
