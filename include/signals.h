#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include "libraries.h"

void handle_signal();
void interrupt(int _);
void child_process_end(int _);
void check_ctrl_d();

#endif