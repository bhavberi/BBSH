#include "libraries.h"

int main()
{
    set_root_path();

    while (true)
    {
        setbuf(stdout, NULL);
        printPrompt();
        input();
    }

    return 0;
}