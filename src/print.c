#include "libraries.h"

void printPrompt()
{
    // USERNAME
    register struct passwd *pw;
    register uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    if (!pw)
        errors(true, true, "Couldn't find UID!");
    str username = pw->pw_name;

    // HOSTNAME
    char hostname[HOSTNAME_MAX + 1];
    if (gethostname(hostname, HOSTNAME_MAX))
        errors(true, true, "Couldn't find hostname!");

    printf("<%s%s%s:%s> ", colour(BOLD, colour(GREEN, username)), colour(BOLD, colour(GREEN, "@")), colour(BOLD, colour(GREEN, hostname)), colour(BLUE, display_path(0)));
}

void printPrompt_time(str to_print)
{
    printPrompt();
    printf("\b\b %s> ", colour(BLUE, to_print));
}

void errors(bool to_exit, bool fatal, str message)
{
    str error = calloc(8 + strlen(message), sizeof(char));
    sprintf(error, "%s: %s", (fatal) ? "FATAL" : "ERROR", message);

    str coloured_error = colour(RED, error);
    free(error);
    if (fatal)
    {
        error = colour(BOLD, coloured_error);
        free(coloured_error);
        coloured_error = error;
    }

    perror(coloured_error);

    free(coloured_error);

    if (to_exit)
    {
        writeHist();
        exit(fatal);
    }
}