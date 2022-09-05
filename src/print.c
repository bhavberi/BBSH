#include "libraries.h"

void printPrompt()
{
    // USERNAME
    register struct passwd *pw;
    register uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    // if (!pw)
    // {
    //     fprintf(stderr, "Cannot find username for UID %u\n", (unsigned)uid);
    //     exit(EXIT_FAILURE);
    // }
    str username = pw->pw_name;

    // HOSTNAME
    char hostname[HOSTNAME_MAX + 1];
    gethostname(hostname, HOSTNAME_MAX);
    // {
    //     throw_fatal_error("Unable to retrieve hostname");
    // }

    printf("<%s%s%s:%s> ", colour(BOLD,colour(GREEN, username)), colour(BOLD,colour(GREEN, "@")), colour(BOLD,colour(GREEN, hostname)), colour(BLUE, display_path(0)));
}

void printPrompt_time(str to_print)
{
    printPrompt();
    printf("\b\b%s> ", colour(BLUE, to_print));
}