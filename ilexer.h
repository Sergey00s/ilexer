#ifndef ILEXER_H
#define ILEXER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FREAD 1
#define ARG 2

typedef struct 
{
    char *tobe_lexed;
    int pos;
    char *prog;
    char **pargs;
    char *filename;
    int type;
} COMMAND;

#endif