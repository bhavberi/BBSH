#include "libraries.h"

bool prefix(const str pre, const str s)
{
    return strncmp(pre, s, strlen(pre)) == 0;
}

str str_copy(str to_copy)
{
    return strdup(to_copy);
}

str colour(str colour, str s)
{
    str s_new = calloc(strlen(s) + 2 * strlen(RESET) + 3, sizeof(char));
    sprintf(s_new, "%s%s%s", colour, s, RESET);

    return s_new;
}

str strip(str s)
{
    int l = strlen(s);
    while (isspace(s[l - 1]))
        --l;
    while (*s && isspace(*s))
    {
        ++s;
        --l;
    }
    return strndup(s, l);
}

str_array split(str s1, str delimit, int *token_count)
{
    str s = strip(str_copy(s1));
    str token = strtok(str_copy(s), delimit);
    *token_count = 0;

    while (token)
    {
        (*token_count)++;
        token = strtok(NULL, delimit);
    }

    if (s[strlen(s) - 1] == delimit[0])
        token_count--;

    str_array tokens = calloc(*token_count, sizeof(str));
    token = strtok(str_copy(s), delimit);

    for (int i = 0; i < (*token_count); i++)
    {
        tokens[i] = calloc(strlen(token), sizeof(char));
        tokens[i] = token;
        token = strtok(NULL, delimit);
    }

    return tokens;
}

str replace(str s, char to_replace, str by_replace)
{
    s = strip(s);

    int no_chars = 0;
    size_t l = strlen(s);
    for (int i = 0; i < l; ++i)
        no_chars += (to_replace == s[i]);

    str new = calloc(strlen(s) + no_chars + 2, sizeof(char));
    int iter = 0;

    for (int i = 0; i < l; ++i)
    {
        if (to_replace == s[i])
            strcat(new, by_replace);
        else
            new[strlen(new)] = s[i];
    }

    // free(s);
    s = new;

    return new;
}

int dirs_comp(const void *pa, const void *pb)
{
    str *a = (str *)pa;
    str *b = (str *)pb;

    str last_file_path = strrchr(*a, '/');
    str name = last_file_path ? last_file_path + 1 : *a;

    str last_file_path1 = strrchr(*b, '/');
    str name1 = last_file_path1 ? last_file_path1 + 1 : *b;

    // printf("%s %s - %s %s\n", *a, name, *b, name1);

    return (strcasecmp(name, name1) > 0);
}

int files_comp(const void *pa, const void *pb)
{
    str *a = (str *)pa;
    str *b = (str *)pb;

    return (strcasecmp(*a, *b) > 0);
}

int no_of_done = 0;

void main_loop(int time_included, ...)
{
    setbuf(stdout, NULL);
    if (time_included)
    {
        va_list list;
        va_start(list, time_included);
        printPrompt_time(va_arg(list, str));
        va_end(list);
    }
    else
        printPrompt();

    input();

    if (no_of_done >= 10)
    {
        writeHist();
        no_of_done = 0;
    }
    fflush(stdout);
}

// open file, assign fd for redirecting from/to it, return command
str redirect(str command, int *in_fd, int *out_fd)
{
    str start = command;
    str end = strrchr(command, '\0');

    // output redirection
    str out_ptr = strchr(command, '>');
    if (!(long long)out_ptr)
        out_ptr = end;

    // input redirection
    str in_ptr = strchr(command, '<');
    if (!(long long)in_ptr)
        in_ptr = start;

    // reassign output fd
    if (out_ptr++ != strrchr(command, '\0'))
    {
        int append = (out_ptr[0] == '>');
        out_ptr += append;

        int out_size = end - out_ptr;
        str out_path = calloc(out_size, sizeof(char));
        strncpy(out_path, strip(out_ptr), out_size);

        if (!append)
            fclose(fopen(strip(out_path), "w"));

        *out_fd = open(strip(out_path), O_WRONLY | O_CREAT | (append ? O_APPEND : 0), 0644);
        if (*out_fd < 0)
        {
            errors(0, 1, "Can't Redirect Files");
            return NULL;
        }

        // reassign end of command pointer
        end = out_ptr - (1 + append);
    }

    // reassign input fd
    if (in_ptr++ != command)
    {
        int in_size = out_ptr - in_ptr - 2;
        str in_path = calloc(in_size, sizeof(char));
        strncpy(in_path, strip(in_ptr), in_size);
        strip(in_path);

        *in_fd = open(strip(in_path), O_RDONLY, 0644);
        if (*in_fd < 0)
        {
            errors(0, 1, "Can't Redirect Files");
            return NULL;
        }

        // reassign end of command pointer
        end = in_ptr - 1;
    }

    // regenerate command
    int new_command_size = end - start;
    str new_command = calloc(new_command_size, sizeof(char));
    strncpy(new_command, strip(start), (end - start));

    return new_command;
}