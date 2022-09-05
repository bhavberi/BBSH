#ifndef _JOBPOOL_H_
#define _JOBPOOL_H_

#include "libraries.h"

// defining jobpool as linked list
typedef struct JOBPOOL
{
    pid_t pid;
    str name;
    time_t start;
    struct JOBPOOL *next;
} job_pool;

typedef job_pool *jobpool;

void new_job(pid_t pid, str name);
void ended_job(pid_t pid, int status);
void endalljobs();

#endif