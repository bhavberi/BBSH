#include "libraries.h"

void foreground(str command)
{
    str command_copy = str_copy(command);
    str tokens[INPUTLENGTH_MAX + 1];
    int no_tokens = 0;
    char delimit[] = " \n";

    str token = strtok(command, delimit);
    while (token != NULL)
    {
        tokens[no_tokens++] = token;
        token = strtok(NULL, delimit);
    }
    tokens[no_tokens++] = NULL;

    time_t start = time(NULL);

    pid_t pid = fork();
    if (pid < 0)
        errors(true, true, "Couldn't make a foreground fork of process!");

    if (pid == 0)
    {
        if (execvp(tokens[0], tokens))
        {
            str errmsg = calloc(28 + strlen(tokens[0]), sizeof(char));
            sprintf(errmsg, "Couldn't run the command - %s", tokens[0]);
            errors(false, true, errmsg);
            free(errmsg);
            // printf("%s,%s,%s\nError: %d\n", tokens[0], tokens[1],tokens[2], pid);
        }
        // else
        // {
        //     printf("%s,%s,%s\nError: %d\n", tokens[0], tokens[1], tokens[2], pid);
        // }
    }
    else
    {
        newfgjob(pid, command_copy, start);

        int status;
        waitpid(pid, &status, WUNTRACED);
        endfgjob();
    }

    time_t end = time(NULL);
    long time_spent = end - start;
    if (time_spent >= 1)
    {
        str to_print = calloc(200, sizeof(char));
        sprintf(to_print, "took %lds", time_spent);
        main_loop(true, to_print);
    }
}

void background(str command)
{
    str command_copy = strndup(command, strlen(command) - 1);
    str tokens[INPUTLENGTH_MAX + 1];
    int no_tokens = 0;
    char delimit[] = "& \n";

    str token = strtok(command, delimit);
    while (token != NULL)
    {
        tokens[no_tokens++] = token;
        token = strtok(NULL, delimit);
    }
    tokens[no_tokens++] = NULL;

    pid_t pid = fork();
    if (pid < 0)
        errors(true, true, "Couldn't make a background fork of process!");

    if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(tokens[0], tokens))
        {
            str errmsg = calloc(28 + strlen(tokens[0]), sizeof(char));
            sprintf(errmsg, "Couldn't run the command - %s", tokens[0]);
            errors(false, true, errmsg);
            free(errmsg);
            // printf("%s,%s,%s\nError: %d\n", tokens[0], tokens[1],tokens[2], pid);
        }
        // else
        // {
        //     printf("%s,%s,%s\nError: %d\n", tokens[0], tokens[1], tokens[2], pid);
        // }
    }
    else
        new_job(pid, command_copy);
}

void quit()
{
    printf("\n");
    writeHist();
    endalljobs();
    exit(0);
}

void send_signal(int id, int signal)
{
    pid_t pid = get_job_pid(id);
    if (pid == -1)
    {
        errors(false, false, "Unable to find the process with the given id.");
        return;
    }

    if (kill(pid, signal))
    {
        str errorMessage = "Unable to send %d signal to process with pid=%d & id=%d";
        str error = malloc((strlen(errorMessage) + sizeof(int) * 3 + 3) * sizeof(char));
        assert(error != NULL);

        sprintf(error, errorMessage, signal, pid, id);
        errors(false, false, error);
        if (error)
            free(error);
    }
}

void bg_run(int id)
{
    send_signal(id, SIGCONT);
}

void fg_run(int id)
{
    pid_t pid = get_job_pid(id);
    if (pid == -1)
    {
        errors(false, true, "Unable to find the process with the given id.");
        return;
    }

    setpgid(pid, getpid());

    ended_job(pid, -1, false);

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, getpgid(pid)))
    {
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        errors(false, false, "fg: unable to give terminal control to job");
        return;
    }

    if (kill(pid, SIGCONT))
    {
        str errorMessage = "Unable to send SIGCONT signal to process with pid=%d";
        str error = malloc((strlen(errorMessage) + sizeof(int) + 3) * sizeof(char));
        assert(error != NULL);

        sprintf(error, errorMessage, pid);
        errors(false, false, error);
        if (error)
            free(error);
    }

    newfgjob(pid, get_job_name(id), time(NULL));

    int status;
    waitpid(pid, &status, WUNTRACED);

    if (tcsetpgrp(STDIN_FILENO, getpgid(0)))
    {
        errors(false, true, "fg: unable to return terminal control to shell");
    }

    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    time_t end = time(NULL);
    long time_spent = end - (fg->start);

    endfgjob();
    
    if (time_spent >= 1)
    {
        str to_print = calloc(200, sizeof(char));
        sprintf(to_print, "took %lds", time_spent);
        main_loop(true, to_print);
    }
}