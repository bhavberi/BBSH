#include "libraries.h"

int file_stats_comp(const void *pa, const void *pb)
{
    file_stats *a = (file_stats *)pa;
    file_stats *b = (file_stats *)pb;

    return (strcasecmp(a->name, b->name) > 0);
}

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

void ls(int no_words, str args[])
{
    int dir_count = 0;
    str *dirs = calloc(no_words, sizeof(str));

    int a = 0;
    int l = 0;

    for (int i = 0; i < no_words; i++)
    {
        if (args[i][0] == '-')
        {
            if (a && l)
                continue;

            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] == 'a')
                    a = 1;
                else if (args[i][j] == 'l')
                    l = 1;
            }
        }
        else
            dirs[dir_count++] = long_path(args[i]);
    }

    if (dir_count == 0)
    {
        dirs[dir_count] = "./";
        dir_count++;
    }

    for (int i = 0; i < dir_count; i++)
    {
        if (i > 0)
            printf("\n");
        if (dir_count > 1)
            printf("%s:\n", dirs[i]);

        struct dirent *entry;
        DIR *dir = opendir(dirs[i]);
        if (!dir)
            exit(1);

        int entry_count = 0;
        while (entry = readdir(dir))
            if (a || (!a && entry->d_name[0] != '.'))
                entry_count++;

        file_stats col_entries[entry_count];

        int len_hls = 1;
        int len_owner = 1;
        int len_group = 1;
        int len_size = 1;

        double total_block_size = 0;

        int j = 0;

        dir = opendir(dirs[i]);
        while (entry = readdir(dir))
        {
            if (!a && entry->d_name[0] == '.')
                continue;

            struct stat stats;
            str path = calloc(strlen(dirs[i]) + strlen(entry->d_name) + 2, sizeof(char));
            sprintf(path, "%s/%s", dirs[i], entry->d_name);

            if (lstat(path, &stats) == 0)
            {
                str file_type = S_ISREG(stats.st_mode)    ? "-"
                                : S_ISLNK(stats.st_mode)  ? "l"
                                : S_ISDIR(stats.st_mode)  ? "d"
                                : S_ISCHR(stats.st_mode)  ? "c"
                                : S_ISBLK(stats.st_mode)  ? "b"
                                : S_ISFIFO(stats.st_mode) ? "p"
                                : S_ISSOCK(stats.st_mode) ? "s"
                                                          : "-";

                col_entries[j].name = entry->d_name;

                col_entries[j].colored_name = colour(file_type[0] == 'd'   ? BLUE
                                                     : file_type[0] == 'l' ? CYAN
                                                                           : WHITE,
                                                     col_entries[j].name);

                total_block_size += stats.st_blocks;

                col_entries[j].hls = calloc(16, sizeof(char));
                sprintf(col_entries[j].hls, "%lu", stats.st_nlink);

                col_entries[j].size = calloc(16, sizeof(char));
                sprintf(col_entries[j].size, "%ld", stats.st_size);

                str file_modified_format = "%b %d %R";

                if (labs(time(NULL) - stats.st_mtim.tv_sec) > (6 * 2629743))
                {
                    file_modified_format = "%b %d  %Y";
                }

                str file_modified = calloc(32, sizeof(char));
                strftime(file_modified, 32, file_modified_format,
                         localtime((long *)&stats.st_mtim));

                str file_perms = calloc(16, sizeof(char));
                strcat(file_perms, (stats.st_mode & S_IRUSR) ? "r" : "-");
                strcat(file_perms, (stats.st_mode & S_IWUSR) ? "w" : "-");
                strcat(file_perms, (stats.st_mode & S_IXUSR) ? "x" : "-");
                strcat(file_perms, (stats.st_mode & S_IRGRP) ? "r" : "-");
                strcat(file_perms, (stats.st_mode & S_IWGRP) ? "w" : "-");
                strcat(file_perms, (stats.st_mode & S_IXGRP) ? "x" : "-");
                strcat(file_perms, (stats.st_mode & S_IROTH) ? "r" : "-");
                strcat(file_perms, (stats.st_mode & S_IWOTH) ? "w" : "-");
                strcat(file_perms, (stats.st_mode & S_IXOTH) ? "x" : "-");

                str file_link = "";
                if (file_type[0] == 'l')
                {
                    file_link = calloc(PATH_MAX, sizeof(char));
                    if (readlink(path, file_link, PATH_MAX) == -1)
                    {
                        // exit(throw_custom_error("ls: unable to read symlink pathname", -1));
                        exit(1);
                    }
                }

                col_entries[j].type_perms = calloc(strlen(file_type) + strlen(file_perms), sizeof(char));
                sprintf(col_entries[j].type_perms, "%s%s", file_type, file_perms);

                str file_name_link = calloc(5 + strlen(col_entries[j].colored_name) + PATHLENGTH_MAX, sizeof(char));
                strcat(file_name_link, col_entries[j].colored_name);
                strcat(file_name_link, file_type[0] == 'l' ? " -> " : "");
                strcat(file_name_link, file_link);

                col_entries[j].owner = getpwuid(stats.st_uid)->pw_name;
                col_entries[j].group = getpwuid(stats.st_gid)->pw_name;
                col_entries[j].modified = file_modified;
                col_entries[j].name_link = file_name_link;

                size_t len;

                len = strlen(col_entries[j].hls);
                len_hls = len > len_hls ? len : len_hls;

                len = strlen(col_entries[j].owner);
                len_owner = len > len_owner ? len : len_owner;

                len = strlen(col_entries[j].group);
                len_group = len > len_group ? len : len_group;

                len = strlen(col_entries[j].size);
                len_size = len > len_size ? len : len_size;
            }
            j++;

            free(path);
        }

        qsort(col_entries, entry_count, sizeof(file_stats), file_stats_comp);

        if (l)
            printf("total %d\n", (int)ceil(total_block_size / 2));

        for (int j = 0; j < entry_count; j++)
        {
            if (l)
                printf("%s %*s %*s %*s %*s %s ", col_entries[j].type_perms, len_hls, col_entries[j].hls,
                       len_owner, col_entries[j].owner, len_group, col_entries[j].group, len_size, col_entries[j].size, col_entries[j].modified);

            printf("%s\n", col_entries[j].name_link);

            free(col_entries[j].hls);
            free(col_entries[j].size);
            free(col_entries[j].modified);
            free(col_entries[j].name_link);
            free(col_entries[j].type_perms);
        }
    }

    free(dirs);
}