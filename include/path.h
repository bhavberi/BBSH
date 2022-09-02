#ifndef _PATH_H_
#define _PATH_H_

#include "libraries.h"

#define PATHLENGTH_MAX 1024

// extern str ROOT_PATH;

str display_path(bool full);
str long_path(str short_path);
str short_path(str long_path);

void set_root_path();

void set_prev_path(str prev_path);
str get_prev_path();

#endif