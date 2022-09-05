#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "libraries.h"

#define STAT_BUF_SIZE 2048

typedef struct filestats
{
    str type_perms;
    str hls;
    str owner;
    str group;
    str size;
    str modified;
    str name_link;
    str name;
    str colored_name;

} file_stats;

void pwd();
void echo(int no_words, str sentence[]);
void cd(str dir);
void pinfo(int pid);
void ls(int no_words, str args[]);
void background(str command);
void foreground(str command);

#endif