#include "libraries.h"

#define MAX_FILES_DIRS 1024

int d, f;
str name_comp = NULL;

str_array results = NULL;
int results_iter = 0;

void discovery_rec(str dir)
{
    struct dirent *entry;
    DIR *dir_p = opendir(dir);
    if (!dir_p)
        errors(true, false, "Couldn't open the directory to read! ");

    while (entry = readdir(dir_p))
    {
        struct stat stats;
        str path = calloc(strlen(dir) + strlen(entry->d_name) + 10, sizeof(char));
        assert(path != NULL);
        sprintf(path, "%s/%s", dir, entry->d_name);

        if (lstat(path, &stats) == 0)
        {
            // printf("%s\n", entry->d_name);
            if (entry->d_name[0] == '.')
                continue;
            if (S_ISDIR(stats.st_mode))
            {
                discovery_rec(path);
                if (d && (!name_comp || !strcmp(entry->d_name, name_comp)))
                    results[results_iter++] = path;
            }
            else
            {
                if (f && (!name_comp || !strcmp(entry->d_name, name_comp)))
                    results[results_iter++] = path;
            }
        }
    }
}

void discovery(int no_words, str args[])
{
    int dir_count = 0;

    d = 0;
    f = 0;
    str dir = NULL;
    name_comp = NULL;

    results = calloc(MAX_FILES_DIRS, sizeof(str));
    assert(results != NULL);
    results_iter = 0;

    for (int i = 0; i < no_words; i++)
    {
        if (args[i][0] == '-')
        {
            if (d && f)
                continue;

            for (int j = 1; j < strlen(args[i]); j++)
            {
                if (args[i][j] == 'd')
                    d = 1;
                else if (args[i][j] == 'f')
                    f = 1;
            }
        }
        else if (args[i][0] == '"')
        {
            name_comp = calloc(strlen(args[i]), sizeof(char));
            assert(name_comp != NULL);
            sprintf(name_comp, "%.*s", (int)(strlen(args[i]) - 2), &args[i][1]);
        }
        else
            dir = long_path(args[i]);
    }

    if (d + f == 0)
        d = f = 1;

    if (!dir)
        dir = ".";

    discovery_rec(dir);
    qsort(results, results_iter, sizeof(*results), files_comp);

    for (int i = 0; i < results_iter; ++i)
    {
        printf("%s\n", results[i]);
        free(results[i]);
    }

    if (name_comp)
        free(name_comp);
    free(results);
    results = NULL;
}