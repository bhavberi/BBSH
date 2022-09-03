#ifndef _HISTORY_H_
#define _HISTORY_H_

#include "libraries.h"

#define MAX_HIST_SIZE 20
#define HIST_DISPLAY 10
#define HIST_PATH "/var/tmp/.bbsh_hist"

typedef struct HIST{
    int no_entries;
    str *entries;
    int last_added;
} hist;

void setupHist();
void addHist(str command);
void writeHist();
void history();

#endif