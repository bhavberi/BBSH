#include "libraries.h"

// default terminal structure
struct termios terminal;

void disable_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal) == -1)
        return errors(true, true, "terminal: can't disable raw mode");
}

void enable_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &terminal) == -1)
        return errors(true, true, "terminal: can't get attributes");
    atexit(disable_raw_mode);
    struct termios raw = terminal;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        return errors(true, true, "terminal: can't enable raw mode");
}

void handle_tab(int *cursor, str input_buffer)
{
    str space_in_command = strrchr(input_buffer, ' ');

    str name_comp = space_in_command ? space_in_command + 1 : input_buffer;

    str_array results = calloc(MAX_FILES_DIRS, sizeof(str));
    assert(results != NULL);
    int results_iter = 0;

    str start = "";

    if (strrchr(name_comp, '/'))
    {
        start = str_copy(name_comp);
        str path_dir = strrchr(start, '/');
        start[path_dir - start + 1] = '\0';
    }
    else if (prefix("./", name_comp))
        start = "./";

    struct dirent *entry;
    DIR *dir_p = opendir((strcmp("", start) ? start : "."));
    if (!dir_p)
        errors(true, false, "Couldn't open the directory to read! ");

    while (entry = readdir(dir_p))
    {
        struct stat stats;
        str path = calloc(strlen(start) + strlen(entry->d_name) + 2, sizeof(char));
        assert(path != NULL);
        sprintf(path, "%s%s", start, entry->d_name);

        if (lstat(path, &stats) == 0)
        {
            if (entry->d_name[0] == '.')
                continue;
            if (S_ISDIR(stats.st_mode))
            {
                if (name_comp && prefix(name_comp, path))
                {
                    strcat(path, "/");
                    results[results_iter++] = path;
                }
            }
            else if (name_comp && prefix(name_comp, path))
            {
                strcat(path, " ");
                results[results_iter++] = path;
            }
            else
                free(path);
        }
    }

    if (results_iter == 0)
    {
        name_comp = NULL;
        free(results);
        // pass
    }
    else if (results_iter == 1)
    {
        if (cursor > 0)
            printf("\e[%dD", *cursor);

        if (!space_in_command)
        {
            input_buffer = results[0];
        }
        else
        {
            // strncpy(input_buffer, input_buffer, space_in_command - input_buffer);
            input_buffer[space_in_command - input_buffer + 1] = '\0';
            strcat(input_buffer, results[0]);
        }
        *cursor = strlen(input_buffer);
        printf("%s", input_buffer);
    }
    else
    {
        qsort(results, results_iter, sizeof(*results), files_comp);

        printf("\n");
        for (int i = 0; i < results_iter; ++i)
        {
            printf("%s\n", results[i]);
            free(results[i]);
        }

        printPrompt();
        printf("%s", input_buffer);

        // free(results);
    }
}

str get_raw_input()
{
    int cursor = 0;
    char input_char;
    str input_buffer = calloc(INPUTLENGTH_MAX, sizeof(char));

    int buffer_open = 1;
    int history_index = 0;

    enable_raw_mode();
    memset(input_buffer, '\0', INPUTLENGTH_MAX);
    while (buffer_open && (read(STDIN_FILENO, &input_char, 1) == 1))
    {
        if (iscntrl(input_char)) // check control chars
        {
            if (input_char == 10)
            {
                printf("\n");
                buffer_open = 0;
            }
            else if (input_char == 27)
            {
                // arrow keys
                char es_buffer[3];
                es_buffer[2] = 0;

                if (read(STDIN_FILENO, es_buffer, 2) == 2)
                {
                    if (es_buffer[1] == 'A')
                    {
                        if (cursor > 0)
                            printf("\e[%dD", cursor);
                        cursor = 0;
                    }
                    else if (es_buffer[1] == 'B')
                    {
                        if (cursor <= (int)strlen(input_buffer) - 1)
                            printf("\e[%dC", (int)strlen(input_buffer) - cursor);
                        cursor = (int)strlen(input_buffer);
                    }
                    else if (es_buffer[1] == 'C')
                    {
                        // move cursor right
                        if (cursor <= (int)strlen(input_buffer) - 1)
                        {
                            if (input_buffer[cursor] == 9)
                                printf("\e[8C");
                            else
                                printf("\e[1C");
                            cursor++;
                        }
                    }
                    else if (es_buffer[1] == 'D')
                    {
                        // move cursor left
                        if (cursor > 0)
                        {
                            // traverse a tab
                            if (input_buffer[cursor - 1] == 9)
                                printf("\e[8D");
                            else
                                printf("\e[1D");
                            cursor--;
                        }
                    }
                    else
                    {
                        // pass
                    }
                }
            }
            else if (input_char == 127 || input_char == 8)
            {
                if (cursor > 0)
                {
                    // removing a tab
                    if (input_buffer[cursor - 1] == 9)
                        printf("\b\b\b\b\b\b\b");
                    --cursor;
                    if (cursor == strlen(input_buffer) - 1)
                        input_buffer[cursor] = '\0';

                    printf("\b \b");
                }
            }
            else if (input_char == 9)
            {
                // input_buffer[cursor++] = input_char;
                // printf("        ");
                handle_tab(&cursor, input_buffer);
            }
            else if (input_char == 12)
            {
                // clear
                input_buffer[0] = '\0';
                cursor = 0;
                printf("\e[1;1H\e[2J");
                printPrompt();
            }
            else if (input_char == 4)
            {
                quit();
            }
            else
            {
                // others
            }
        }
        else
        {
            if (cursor == strlen(input_buffer))
            {
                input_buffer[cursor++] = input_char;
                printf("%c", input_char);
            }
            else
            {
                for (int i = (int)strlen(input_buffer); i >= cursor; i--)
                {
                    input_buffer[i + 1] = input_buffer[i];
                }
                input_buffer[cursor++] = input_char;
                if (cursor - 1)
                {
                    printf("\e[%dD\e[K%s\e[%dD", (cursor)-1, input_buffer,
                           (int)strlen(input_buffer) - (cursor));
                }
                else
                {
                    printf("\e[K%s\e[%dD", input_buffer, (int)strlen(input_buffer) - cursor);
                }
            }
        }
    }
    disable_raw_mode();

    return input_buffer;
}