#include "libraries.h"

pid_t shell_pid;

void handle_signal()
{
    struct sigaction action;

    memset(&action, 0, sizeof(action));
    action.sa_handler = child_process_end;
    action.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &action, NULL);

    memset(&action, 0, sizeof(action));
    action.sa_handler = interrupt;
    action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &action, NULL);

    memset(&action, 0, sizeof(action));
    action.sa_handler = check_ctrl_d;
    action.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &action, NULL);

    memset(&action, 0, sizeof(action));
    action.sa_handler = fg_to_bg;
    action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &action, NULL);
}

void interrupt(int _)
{
    printf("\n");
    printPrompt();
    fflush(stdout);
    return;
}

void child_process_end(int _)
{
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        status = WIFEXITED(status);
        ended_job(pid, status,true);
        printPrompt();
        fflush(stdout);
    }
}

void check_ctrl_d()
{
    if (feof(stdin))
    {
        fprintf(stdout, "SHELL EXIT\n");
        fflush(stdout);
        fflush(stdin);
        quit();
    }
}

void fg_to_bg(int _)
{
    if (fg && fg->pid > 0)
    {
        new_job(fg->pid, fg->name);
         
        if (kill(fg->pid, SIGSTOP))
        {
            str errorMessage = "Unable to send SIGSTOP signal to process with pid=%d";
            str error = malloc((strlen(errorMessage) + sizeof(int) + 3) * sizeof(char));
            assert(error != NULL);

            sprintf(error, errorMessage, fg->pid);
            errors(false, false, error);
            if (error)
                free(error);
        }

        setpgid(fg->pid, 0);
        endfgjob();
    }
}