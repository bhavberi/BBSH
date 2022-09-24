#include "libraries.h"

jobpool jobs = NULL;
jobpool fg = NULL;
int no_jobs = 0;

int comp_jobs(const void *a, const void *b)
{
    return strcasecmp(((jobpool)a)->name, ((jobpool)b)->name);
}

void new_job(pid_t pid, str name)
{
    jobpool new = calloc(1, sizeof(job_pool));
    if (new == NULL)
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

void ended_job(pid_t pid, int status, bool to_print)
{
    jobpool i = jobs;
    jobpool prev = NULL;
    while (i)
    {
        if (i->pid == pid)
        {
            time_t end = time(NULL);
            time_t time_spent = (end - i->start);

            if (to_print == true)
            {
                fprintf(stderr, "\n%s with pid = %d exited %snormally", i->name, i->pid, status ? "" : "ab");
                fprintf(stderr, " # After %ld seconds\n", time_spent);
            }

            if (prev)
                prev->next = i->next;
            else
                jobs = i->next;

            // no_jobs--;
            free(i->name);
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
        free(prev->name);
        free(prev);
    }
    no_jobs = 0;
}

pid_t get_job_pid(int id)
{
    jobpool job = jobs;

    while (job)
    {
        if (job->id == id)
            return job->pid;
        job = job->next;
    }

    return -1;
}

str get_job_name(int id)
{
    jobpool job = jobs;

    while (job)
    {
        if (job->id == id)
            return job->name;
        job = job->next;
    }

    return NULL;
}

void print_bg_jobs(int no_words, str args[])
{
    int r = 0, s = 0;

    for (int i = 0; i < no_words; i++)
    {
        if (args[i][0] == '-')
        {
            if (r && s)
                continue;

            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] == 'r')
                    r = 1;
                else if (args[i][j] == 's')
                    s = 1;
                else
                {
                    errors(false, false, "Wrong flags/arguments.");
                    return;
                }
            }
        }
        else
        {
            errors(false, false, "Wrong flags/arguments.");
            return;
        }
    }

    if (r + s == 0)
        r = s = 1;

    job_pool jobslist[no_jobs];
    int jobslist_iter = 0;

    str stat_general_path = "/proc/%d/stat";
    str stat_path = calloc(strlen(stat_general_path) + 10, sizeof(char));
    assert(stat_path != NULL);
    str stat_buf = calloc(STAT_BUF_SIZE, sizeof(char));
    assert(stat_buf != NULL);

    jobpool job = jobs;
    while (job)
    {
        sprintf(stat_path, stat_general_path, job->pid);

        FILE *stat_fp = fopen(stat_path, "r");
        if (!stat_fp)
        {
            errors(true, false, "Couldn't access the /proc/pid/stat File!\nExiting...");
            return;
        }

        fread(stat_buf, 1, STAT_BUF_SIZE, stat_fp);

        int no_values;
        str_array values = split(stat_buf, " ", &no_values);

        job->running = (strncmp(values[2], "T", 1)) ? true : false;

        jobslist[jobslist_iter++] = *job;

        job = job->next;
    }
    free(stat_path);
    free(stat_buf);

    qsort(jobslist, no_jobs, sizeof(job_pool), comp_jobs);

    for (int i = 0; i < no_jobs; ++i)
    {
        if (r && jobslist[i].running)
            printf("[%d] Running %s [%d]\n", jobslist[i].id, strip(jobslist[i].name), jobslist[i].pid);
        if (s && !jobslist[i].running)
            printf("[%d] Stopped %s [%d]\n", jobslist[i].id, strip(jobslist[i].name), jobslist[i].pid);
    }
}

void newfgjob(pid_t pid, str name, time_t start)
{
    fg = calloc(1, sizeof(job_pool));
    if (fg == NULL)
        errors(true, true, "Can't allocate Memory for JobPool.");

    fg->pid = pid;
    fg->name = name;
    fg->start = (start) ? start : time(NULL);
    fg->next = jobs;
    fg->id = -1;
}

void endfgjob()
{
    free(fg);
    fg = NULL;
}