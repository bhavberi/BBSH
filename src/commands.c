#include "libraries.h"

void pwd()
{
    printf("%s\n", colour(YELLOW, display_path(true)));
}

void echo(int no_words, str sentence[])
{
    int index = -1;
    while (++index < no_words)
    {
        printf("%s ", sentence[index]);
    }
    printf("\n");
}

void cd(str dir)
{
    str prev_path = display_path(true);

    if (!strcmp(dir, "."))
        return;

    if (prefix("~", dir))
        dir = long_path(dir);

    if (!strcmp(dir, "-"))
    {
        dir = get_prev_path();
    }

    chdir(dir);
    set_prev_path(prev_path);

    if (prefix("..", dir) && strcmp("~", display_path(false)) && !prefix("~", display_path(false)))
        pwd();
}

void pinfo(int pid)
{
    if (!pid)
        pid = getpid();

    str str_pid = malloc((int)(log10(pid) + 4));
    sprintf(str_pid, "%d", 42);

    int back_fore = 0;
    str status;
    str memory;
    str exe = calloc(PATHLENGTH_MAX, sizeof(char));

    // https://man7.org/linux/man-pages/man5/proc.5.html
    // https://stackoverflow.com/questions/39066998/what-are-the-meaning-of-values-at-proc-pid-stat

    str exe_general_path = "/proc/%d/exe";
    str exe_path = calloc(strlen(exe_general_path) + 10, sizeof(char));
    sprintf(exe_path, exe_general_path, pid);

    if (!(readlink(exe_path, exe, PATHLENGTH_MAX) + 1))
        exe = "Couldn't not Find";
    else
        exe = short_path(exe);

    str stat_general_path = "/proc/%d/stat";
    str stat_path = calloc(strlen(stat_general_path) + 10, sizeof(char));
    sprintf(stat_path, stat_general_path, pid);

    FILE *stat_fp = fopen(stat_path, "r");
    if (!stat_fp)
        exit(1);

    str stat_buf = calloc(STAT_BUF_SIZE, sizeof(char));
    fread(stat_buf, 1, STAT_BUF_SIZE, stat_fp);

    int no_values;
    str_array values = split(stat_buf, " ", &no_values);

    status = values[2];
    memory = values[22];

    if (atoi(values[4]) == atoi(values[7]))
        back_fore = 43;

    printf("%s%s\n", colour(YELLOW, "pid : "), colour(YELLOW, str_pid));
    printf("process status : %s%c\n", status, back_fore);
    printf("memory : %s\n", memory);
    printf("executable path : %s\n", exe);
}