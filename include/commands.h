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

// commands1.c
void pwd();
void echo(int no_words, str sentence[]);
void cd(str dir);
void pinfo(int pid);
void ls(int no_words, str args[]);

// commands2.c
void background(str command);
void foreground(str command);
void quit();
void send_signal(int id, int signal);
void bg_run(int id);
void fg_run(int id);

#endif