#ifndef _PRINT_H_
#define _PRINT_H_

#define HOSTNAME_MAX 512

#include "libraries.h"

void printPrompt();
void printPrompt_time(str to_print);
void errors(bool to_exit, bool fatal, str error);

#endif
