#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "libraries.h"

bool prefix(const str pre, const str s);
str str_copy(str to_copy);
str colour(str colour, str s);
str_array split(str s1, str delimit, int *token_count);

// COLOUR CODES

#define BOLD "\e[1m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define BLUE "\e[34m"
#define MAGENTA "\e[35m"
#define CYAN "\e[36m"
#define WHITE "\e[37m"

#define RESET "\e[0m"

#endif
