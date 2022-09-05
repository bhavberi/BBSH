#include "libraries.h"

jobpool jobs = NULL;
int no_jobs = 0;

void new_job(pid_t pid, str name)
{
    jobpool new = calloc(1, sizeof(job_pool));
    new->pid = pid;
    new->name = name;
    new->start = clock();

    new->next = jobs;
    jobs = new;

    ++no_jobs;
    printf("[%d] %d\n", no_jobs, pid);
}

void ended_job(pid_t pid, int status)
{
    jobpool i = jobs;
    jobpool prev = NULL;
    while (i)
    {
        if (i->pid == pid)
        {
            printf("\n%s with pid = %d exited %snormally", i->name, i->pid, status ? "" : "ab");
            clock_t end = clock();
            double time_spent = (double)(end - i->start) / CLOCKS_PER_SEC;
            printf(" # After %ld seconds\n", (long)time_spent);
            if (prev)
                prev->next = i->next;
            else
                jobs = i->next;
            no_jobs--;
            break;
        }
        prev = i;
        i = i->next;
    }
}

void endalljobs()
{
    jobpool i = jobs;
    while (i)
    {
        killpg(i->pid, SIGKILL);
        i = i->next;
    }
    no_jobs = 0;
}