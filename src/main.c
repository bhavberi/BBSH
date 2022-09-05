#include "libraries.h"

int main()
{
    set_root_path();
    set_prev_path(display_path(false));
    setupHist();
    handle_signal();

    while (true)
    {
        main_loop();
    }

    return 0;
}