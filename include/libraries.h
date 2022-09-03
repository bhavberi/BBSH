// System Libraries 

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>
#include <linux/limits.h>
#include <time.h>

// Programmer Defined Libraries

#include "print.h"
#include "path.h"
#include "helpers.h"
#include "input.h"
#include "commands.h"

#ifndef _LIBRARIES_H_
#define _LIBRARIES_H_

// Custom Headers

#define TRUE 1
#define true 1
#define FALSE 0
#define false 0

#define bool short int

#define str char*
#define str_array char**

#endif