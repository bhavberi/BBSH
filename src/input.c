#include "libraries.h"

void command_process(str command)
{
    char delimit[] = " \t\r\n\v\f";

    str command1 = strdup(command);

    str token = strtok(command, delimit);

    if (token != NULL)
    {
        if (!strcmp(token, "exit") || !strcmp(token, "quit"))
        {
            addHist("exit");
            writeHist();
            exit(0);
        }
        else if (!strcmp(token, "pwd"))
        {
            if (strtok(NULL, delimit))
                exit(1);
            addHist("pwd");
            pwd();
        }
        else if (!strcmp(token, "echo"))
        {
            str sentence[INPUTLENGTH_MAX];
            int no_words = 0;
            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "echo");
            while ((token = strtok(NULL, delimit)) != NULL)
            {
                sentence[no_words++] = token;
                strcat(hist, " ");
                strcat(hist, token);
            }
            addHist(hist);
            echo(no_words, sentence);
        }
        else if (!strcmp(token, "cd"))
        {
            token = strtok(NULL, delimit);

            if (strtok(NULL, delimit))
                exit(1);

            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "cd");
            strcat(hist, " ");
            strcat(hist, token);
            addHist(hist);

            if (token == NULL)
                cd("~");
            else
                cd(token);
        }
        else if (!strcmp(token, "pinfo"))
        {
            token = strtok(NULL, delimit);

            if (strtok(NULL, delimit))
                exit(1);

            str hist = calloc(INPUTLENGTH_MAX, sizeof(char));
            strcat(hist, "pinfo");
            strcat(hist, " ");
            strcat(hist, token);
            addHist(hist);

            if (token == NULL)
                pinfo(0);
            else
                pinfo((int)atoi(token));
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
        }
        else if (!strcmp(token, "history"))
        {
            if (strtok(NULL, delimit))
                exit(1);
            history();
            addHist("history");
        }
        else
        {
            return;
        }
    }

    free(command1);
}

void input()
{
    char in[INPUTLENGTH_MAX];
    str commands[INPUTLENGTH_MAX];
    int no_commands = 0;

    fgets(in, sizeof(in), stdin);

    char delimit[] = ";\n";

    str command = strtok(in, delimit);
    while (command != NULL)
    {
        commands[no_commands++] = command;
        // printf("%s\n", commands[no_commands]);

        command = strtok(NULL, delimit);
    }

    for (int i = 0; i < no_commands; i++)
    {
        command_process(commands[i]);
    }

    fflush(stdin);
}