#include "libraries.h"

void foreground(str command)
{
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
        wait(NULL);

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

void bg(int id)
{
    send_signal(id, SIGCONT);
}