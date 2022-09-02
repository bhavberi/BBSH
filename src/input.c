#include "libraries.h"

void command_process(str command)
{
    char delimit[] = " \t\r\n\v\f";

    str token = strtok(command, delimit);

    if (token != NULL)
    {
        if (!strcmp(token, "exit"))
            exit(0);

        else if (!strcmp(token, "pwd"))
        {
            pwd();
            return;
        }
        else if (!strcmp(token, "echo"))
        {
            str sentence[INPUTLENGTH_MAX];
            int no_words = 0;
            while ((token = strtok(NULL, delimit)) != NULL)
            {
                sentence[no_words++] = token;
            }
            echo(no_words, sentence);
            return;
        }
        else if (!strcmp(token, "cd"))
        {
            token = strtok(NULL, delimit);
            
            if(strtok(NULL, delimit))
                exit(1);
            
            if (token == NULL)
                cd("~");
            else
                cd(token);
            return;
        }
        else if (!strcmp(token, "pinfo"))
        {
            token = strtok(NULL, delimit);

            if (strtok(NULL, delimit))
                exit(1);

            if (token == NULL)
                pinfo(0);
            else
                pinfo((int)atoi(token));
            
            return;
        }
    }
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