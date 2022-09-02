#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "libraries.h"

#define STAT_BUF_SIZE 2048

void pwd();
void echo(int no_words, str sentence[]);
void cd(str dir);
void pinfo(int pid);

#endif