#include "libraries.h"

void command_process(str command)
{
    if (!command || !strcmp(command, ""))
        exit(1);

    char delimit[] = " \t\r\n\v\f";

    str command_copy = str_copy(command);

    str token = strtok(command, delimit);

    if (token != NULL)
    {
        if (!strcmp(token, "exit") || !strcmp(token, "quit"))
        {
            quit();
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
                    exit(1);
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
                    exit(1);
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
        else if (!strcmp(token, "history"))
        {
            if (strtok(NULL, delimit))
                exit(1);
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

void input()
{
    char in[INPUTLENGTH_MAX];
    str commands[INPUTLENGTH_MAX];
    int no_commands = 0;

    fgets(in, sizeof(in), stdin);

    replace(in, '&', "&;");

    char delimit[] = ";\n";

    str command = strtok(in, delimit);
    while (command != NULL)
    {
        if(!strcmp(command," "))
            exit(1);
        commands[no_commands++] = command;
        // printf("%s-\n", command);

        command = strtok(NULL, delimit);
    }

    for (int i = 0; i < no_commands; i++)
    {
        command_process(commands[i]);
    }

    fflush(stdin);
}