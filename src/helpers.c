#include "libraries.h"

bool prefix(const char *pre, const char *s)
{
    return strncmp(pre, s, strlen(pre)) == 0;
}

str str_copy(str to_copy)
{
    return strdup(to_copy);
}

str colour(str colour, str s)
{
    str s_new = calloc(strlen(s) + 2 * strlen(RESET) + 1, sizeof(char));
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
    // return strndup(s, l);
    return s;
}

str_array split(str s1, str delimit, int *token_count)
{
    str s = strip(strdup(s1));
    str token = strtok(strdup(s), delimit);
    *token_count = 0;

    while (token)
    {
        (*token_count)++;
        token = strtok(NULL, delimit);
    }

    if (s[strlen(s) - 1] == delimit[0])
        token_count--;

    str_array tokens = calloc(*token_count, sizeof(str));
    token = strtok(strdup(s), delimit);

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

    return new;
}

int no_of_done = 0;

void main_loop()
{
    setbuf(stdout, NULL);
    printPrompt();
    input();

    // if (no_of_done >= 10)
    // {
    writeHist();
    //     no_of_done = 0;
    // }
    fflush(stdout);
}