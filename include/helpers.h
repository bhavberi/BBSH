#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "libraries.h"

bool prefix(const str pre, const str s);
str str_copy(str to_copy);
str colour(str colour, str s);
str strip(str s);
str_array split(str s1, str delimit, int *token_count);
str replace(str s, char to_replace, str by_replace);
int dirs_comp(const void *pa, const void *pb);
int files_comp(const void *pa, const void *pb);
void main_loop(int time_included, ...);

str redirect(str command, int *in_fd, int *out_fd); // taken from v15sh4 - vsh

#endif
