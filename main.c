#include "ilexer.h"

void execer2(char *str, char *prog, char **pargs, char **env)
{

    char **args = calloc((strlen(str) + 3), sizeof(char *));
    memcpy(args, pargs, sizeof(char *) * (strlen(str) + 1));
    memmove(args + 2, args, sizeof(char *) * (strlen(str) + 1));
    args[0] = prog;
    args[1] = str;
    int ret = execve(prog, args, env);
    fprintf(stderr, "Error: execve failed\n");
    exit(ret);
}


void appender(char **self, char *str)
{
    int i;
    int len;
    char *rtn;

    if (*self == NULL)
    {
        *self = strdup(str);
        return;
    }
    len = strlen(*self);
    rtn = calloc(len + strlen(str) + 1, sizeof(char));
    i = 0;
    strcpy(rtn, *self);
    strcat(rtn, str);
    free(*self);
    *self = rtn;
}

COMMAND command(char **args)
{
    COMMAND cmd;
    int i;
    cmd.type = ARG;
    if (args == NULL)
    {
        fprintf(stderr, "Error: error\n");
        exit(1);
    }
    memset(&cmd, 0, sizeof(COMMAND));
    i = 0;
    char *pargs = NULL;
    while (args[i])
    {
        if (strcmp(args[i], "-p") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Error: Missing argument for -p\n");
                exit(1);
            }
            cmd.prog = args[i + 1];
            cmd.pargs = args + i + 2;
            i += 1;
            if (pargs == NULL && cmd.type == ARG)
            {
                fprintf(stderr, "Error: Missing arguments or wrong usage\n");
                exit(1);
            }
            cmd.tobe_lexed = pargs;
            return cmd;
        }
        else if (strcmp(args[i], "-f") == 0)
        {
            cmd.filename = args[i + 1];
            cmd.type = FREAD;
            i += 1;
        }
        else if (strcmp(args[i], "-i") == 0)
        {   
            cmd.pos = atoi(args[i + 1]);
            i += 1;
        }
        else 
        {
            if (pargs != NULL)
                appender(&pargs, " ");
            appender(&pargs, args[i]);
        }
        i++;
    }
    if (cmd.prog == NULL)
    {
        fprintf(stderr, "Error: -p Missing program\n");
        exit(1);
    }
    return (cmd);
}


int len_len(char **args)
{
    int i;

    i = 0;
    while (args[i])
        i++;
    return (i);
}

void insert_arg(COMMAND *cmd)
{
    char **temp;
    int len;

    len = len_len(cmd->pargs);
    if (cmd->pos > len)
    {
        fprintf(stderr, "Error: -i position is greater than the number of arguments\n");
        exit(1);
    }
    if (cmd->pos < 0)
    {
        fprintf(stderr, "Error: -i position is less than 0\n");
        exit(1);
    }
    temp = calloc((len_len(cmd->pargs) + 3), sizeof(char *));
    memmove(temp, cmd->pargs, sizeof(char *) * (len + 1));
    memmove(temp + cmd->pos + 1, temp + cmd->pos, sizeof(char *) * (len + 1 - cmd->pos));
    temp[cmd->pos] = cmd->tobe_lexed;
    memmove(temp + 1, temp, sizeof(char *) * (len + 2));
    temp[0] = cmd->prog;
    cmd->pargs = temp;
    return;
}



int exec(COMMAND cmd, char **env)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;  
    char *buffer = NULL;

    if (cmd.type == FREAD)
    {
        fp = fopen(cmd.filename, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: File not found\n");
            exit(1);
        }
        while ((read = getline(&line, &len, fp)) != -1)
        {
            appender(&buffer, line);
        }
        fclose(fp);
        if (line)
            free(line);
        cmd.tobe_lexed = buffer;
        insert_arg(&cmd);
    } 
    else
    {
        insert_arg(&cmd);
    }
        if (access(cmd.pargs[0], F_OK) == -1)
        {
            fprintf(stderr, "Error: %s: not found\n", cmd.pargs[0]);
            exit(1);
        }
        if (access(cmd.pargs[0], X_OK) == -1)
        {
            fprintf(stderr, "Error: %s: not executable\n", cmd.pargs[0]);
            exit(1);
        }
        int ret = execve(cmd.pargs[0], cmd.pargs, env);
        fprintf(stderr, "Error: execve failed\n");
        exit(ret);
    return (1);
}


int main(int argc, char **argv, char **env)
{

    if (argc < 3)
    {
        fprintf(stderr, "Usage: ilexer [args ...] [-i pos] [-p process args ...]\n");
        return (1);
    }
    COMMAND cmd = command(argv + 1);
    exec(cmd, env);
    return (1);
}
