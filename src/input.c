#include "libraries.h"

void command_process(str command, int in_fd, int out_fd)
{
    if (!command || !strcmp(command, "") || !strcmp(command, " "))
        errors(false, false, "Syntax Errors near unexpected tokens! ");

    char delimit[] = " \r\n\v\f";

    str command_copy = str_copy(command);

    str token = strtok(command, delimit);

    if (token != NULL)
    {
        if (!strcmp(token, "exit") || !strcmp(token, "quit"))
        {
            addHist(token);
            quit();
        }
        else if (!strcmp(token, "pwd"))
        {
            if (strtok(NULL, delimit))
                errors(false, false, "'pwd' takes only 1 argument ");
            addHist("pwd");
            pwd();
        }
        else if (!strcmp(token, "echo"))
        {
            str sentence[INPUTLENGTH_MAX];
            int no_words = 0;
            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            assert(hist != NULL);
            strcat(hist, "echo");

            while ((token = strtok(NULL, delimit)) != NULL)
            {
                sentence[no_words++] = token;
                strcat(hist, " ");
                strcat(hist, token);
            }

            addHist(hist);
            echo(no_words, sentence);
            free(hist);
        }
        else if (!strcmp(token, "cd"))
        {
            token = strtok(NULL, delimit);

            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "cd");

            if (token == NULL)
                cd("~");
            else
            {
                if (strtok(NULL, delimit))
                    errors(false, false, "'cd' takes only 1 argument ");
                cd(token);
                strcat(hist, " ");
                strcat(hist, token);
            }
            addHist(hist);
            free(hist);
        }
        else if (!strcmp(token, "pinfo"))
        {
            token = strtok(NULL, delimit);

            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "pinfo");

            if (token == NULL)
                pinfo(0);
            else
            {
                if (strtok(NULL, delimit))
                    errors(false, false, "'pinfo' takes only 1 argument ");
                pinfo((int)atoi(token));
                strcat(hist, " ");
                strcat(hist, token);
            }
            addHist(hist);
            free(hist);
        }
        else if (!strcmp(token, "ls"))
        {
            str args[INPUTLENGTH_MAX];
            int no_args = 0;
            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "ls");
            while ((token = strtok(NULL, delimit)) != NULL)
            {
                args[no_args++] = token;
                strcat(hist, " ");
                strcat(hist, token);
            }
            addHist(hist);

            ls(no_args, args);
            free(hist);
        }
        else if (!strcmp(token, "discover"))
        {
            str args[INPUTLENGTH_MAX];
            int no_args = 0;
            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "discover");
            while ((token = strtok(NULL, delimit)) != NULL)
            {
                args[no_args++] = token;
                strcat(hist, " ");
                strcat(hist, token);
            }
            addHist(hist);

            discovery(no_args, args);
            free(hist);
        }
        else if (!strcmp(token, "history"))
        {
            if (strtok(NULL, delimit))
                errors(false, false, "'pwd' takes no argument/s ");
            history();
            addHist("history");
        }
        else
        {
            addHist(command_copy);
            writeHist(); // As these processes can change history

            if (command_copy[strlen(command_copy) - 1] == '&')
                background(command_copy);
            else
                foreground(command_copy);

            setupHist();
            return;
        }
    }

    // free(command_copy);
}

void io_process(str command)
{
    int failure = 0;
    int no_of_pipes;
    str_array pipe_split = split(command, "|", &no_of_pipes);
    int pipe_fds[2][2];
    enum pipe_actions
    {
        READ,
        WRITE
    };

    for (int i = 0; i < no_of_pipes; ++i)
    {
        if (pipe(pipe_fds[i % 2]) == -1)
            errors(true, true, "Error in accessing pipe");

        str command = strip(pipe_split[i]);
        int in_fd = STDIN_FILENO;
        int out_fd = STDOUT_FILENO;

        // if not redirecting input/output, receive/send from/to previous/next pipe
        if (!strchr(command, '<') && i)
        {
            in_fd = pipe_fds[!(i % 2)][READ];
            if (failure)
                close(pipe_fds[!(i % 2)][WRITE]);
        }
        if (!strchr(command, '>') && (i + 1 < no_of_pipes))
        {
            out_fd = pipe_fds[i % 2][WRITE];
        }

        failure = 0;
        command = redirect(command, &in_fd, &out_fd);
        if (!command)
        {
            failure = 1;
            continue;
        }

        // printf("%s\n", command);
        int stdin_bkp = dup(STDIN_FILENO);
        int stdout_bkp = dup(STDOUT_FILENO);

        dup2(in_fd, STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);

        command_process(command, in_fd, out_fd);

        if (in_fd != STDIN_FILENO)
            close(in_fd);
        if (out_fd != STDOUT_FILENO)
            close(out_fd);

        dup2(stdin_bkp, STDIN_FILENO);
        dup2(stdout_bkp, STDOUT_FILENO);
        close(stdin_bkp);
        close(stdout_bkp);
    }
}

void input()
{
    char in[INPUTLENGTH_MAX];
    str commands[INPUTLENGTH_MAX];
    int no_commands = 0;

    if(!fgets(in, sizeof(in), stdin))
        check_ctrl_d();

    replace(in, '&', "&;");

    char delimit[] = ";\n";

    str command = strtok(in, delimit);
    while (command != NULL)
    {
        if (!strcmp(command, " "))
            errors(false, false, "Syntax Errors near unexpected tokens! ");

        commands[no_commands++] = command;
        // printf("%s-\n", command);

        command = strtok(NULL, delimit);
    }

    for (int i = 0; i < no_commands; i++)
        io_process(commands[i]);

    fflush(stdin);
}