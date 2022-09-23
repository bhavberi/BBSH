#include "libraries.h"

jobpool jobs = NULL;
int no_jobs = 0;

void new_job(pid_t pid, str name)
{
    jobpool new = calloc(1, sizeof(job_pool));
    if(new==NULL)
        errors(true, true, "Can't allocate Memory for JobPool.");

    new->pid = pid;
    new->name = name;
    new->start = time(NULL);
    new->next = jobs;

    ++no_jobs;
    printf("[%d] %d\n", no_jobs, pid);
    new->id = no_jobs;

    jobs = new;
}

void ended_job(pid_t pid, int status)
{
    jobpool i = jobs;
    jobpool prev = NULL;
    while (i)
    {
        if (i->pid == pid)
        {
            fprintf(stderr, "\n%s with pid = %d exited %snormally", i->name, i->pid, status ? "" : "ab");
            time_t end = time(NULL);
            time_t time_spent = (end - i->start);
            fprintf(stderr, " # After %ld seconds\n", time_spent);
            if (prev)
                prev->next = i->next;
            else
                jobs = i->next;
            no_jobs--;
            free(i);
            break;
        }
        prev = i;
        i = i->next;
    }
}

void endalljobs()
{
    jobpool i = jobs;
    jobpool prev = NULL;
    while (i)
    {
        killpg(i->pid, SIGKILL);
        prev = i;
        i = i->next;
        free(prev);
    }
    no_jobs = 0;
}