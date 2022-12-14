#ifndef _JOBPOOL_H_
#define _JOBPOOL_H_

#include "libraries.h"

// defining jobpool as linked list
typedef struct JOBPOOL
{
    int id;
    pid_t pid;
    str name;
    time_t start;
    struct JOBPOOL *next;

    // Only to be used for printing
    bool running;
} job_pool;

typedef job_pool *jobpool;

extern jobpool fg;

void new_job(pid_t pid, str name);
void ended_job(pid_t pid, int status, bool to_print);
void endalljobs();
pid_t get_job_pid(int id);
str get_job_name(int id);
void print_bg_jobs(int no_words, str args[]);
void newfgjob(pid_t pid, str name, time_t start);
void endfgjob();

#endif