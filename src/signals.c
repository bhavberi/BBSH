#include "libraries.h"

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

    signal(SIGTSTP, SIG_IGN);
}

void interrupt(int _)
{
    printf("\n");
    printPrompt();
    main_loop(false);
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
        ended_job(pid, status);
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